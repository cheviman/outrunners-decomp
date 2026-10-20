"""
Name:        structure.py
Description: turns a function's blocks and jumps into if / else, loops, switch, break and continue
Version:     0.1.0
Created:     11-10-2026
Modified:    16-10-2026
Author:      Sebastian Toyos

The control flow graph is read with dominators. A block a back edge jumps to heads a loop: its body is
every block that gets back to it without passing through it. A loop is left by break, to the block
the loop goes on at (where all its ways out meet, or the exit most of them share), or by return. A
two-way branch is an if whose two sides meet again at the block that follows it on every path (its
immediate post-dominator; inside a loop, where the ways that stay in it meet). A small block that only
ends the function (a few statements and a return) is copied into every place that jumps to it, the way
C reads: "if (x) { ...; return; }". So is a small piece of code a jump leads into when everything from
there on is small and never comes back to it (TAIL_COPY), or everything up to where the code the jump
comes from goes on (REGION_COPY): it is written again where the jump is. What still does not fit (a
jump into the middle of something already written, an irreducible loop) is a goto to a label here, and
gotos.py takes it out of the tree with a control variable.

The result is a tree of the node classes below; emit.py prints it.
"""
import copy
from dataclasses import dataclass, field

from v60 import ir

SMALL_EXIT = 4    # statements of an ending block that is copied rather than jumped to
COPY_LATCHES = True    # a jump to the few statements a loop goes round again after is written as a copy of
                        # them ending in continue or break (copied_latch): a walk over objects or voices
TAIL_COPY = 20    # statements (and block ends) of all the code a jump leads to, at most, to write it again
REGION_COPY = 16  # statements (and block ends), at most, of the code a jump leads to up to where the code the
                  # jump comes from goes on anyway, to write it again there


# ---- the tree ----

@dataclass
class Code:
    label: object
    statements: list


@dataclass
class If:
    condition: object
    then: list
    otherwise: list = field(default_factory=list)


@dataclass
class Loop:
    body: list
    kind: str = "forever"         # "forever", "while" (condition before the body) or "do" (after it)
    condition: object = None


@dataclass
class Switch:
    value: object
    cases: list                   # [(case values, body)]


@dataclass
class End:
    """The end of the function: a Return, a TailCall or an InterruptReturn."""
    end: object


@dataclass
class Goto:
    label: object


@dataclass
class Label:
    label: object


class Break:
    pass


class Continue:
    pass


@dataclass
class Mark:
    """A control variable of the C set (gotos.py): 'name' = 'value', ir.Const 0 or 1 or a condition."""
    name: str
    value: object


# ---- graph facts ----

EXIT = "exit"
LOOP_EXIT = "loop exit"


def successors(block):
    return block.successors()


def reverse_postorder(blocks, entry):
    order = []
    seen = set()
    stack = [(entry, iter(successors(blocks[entry])))]
    seen.add(entry)
    while stack:
        label, children = stack[-1]
        advanced = False
        for child in children:
            if child not in seen:
                seen.add(child)
                stack.append((child, iter(successors(blocks[child]))))
                advanced = True
                break
        if not advanced:
            order.append(label)
            stack.pop()
    order.reverse()
    return order


def dominators(order, predecessors, entry):
    """Immediate dominators (Cooper, Harvey, Kennedy)."""
    index = {label: position for position, label in enumerate(order)}
    idom = {entry: entry}

    def intersect(a, b):
        while a != b:
            while index[a] > index[b]:
                a = idom[a]
            while index[b] > index[a]:
                b = idom[b]
        return a

    changed = True
    while changed:
        changed = False
        for label in order[1:]:
            done = [p for p in predecessors[label] if p in idom and p in index]
            if not done:
                continue
            new = done[0]
            for other in done[1:]:
                new = intersect(other, new)
            if idom.get(label) != new:
                idom[label] = new
                changed = True
    return idom


def post_dominator_tree(succ, root):
    """Immediate post-dominators of the graph 'succ' (label -> the labels after it) whose every way ends
    at 'root'."""
    rsucc = {root: []}
    for label, targets in succ.items():
        rsucc.setdefault(label, [])
        for target in targets:
            rsucc.setdefault(target, []).append(label)
    order, seen, stack = [], {root}, [(root, iter(rsucc[root]))]
    while stack:
        label, children = stack[-1]
        child = next((child for child in children if child not in seen), None)
        if child is None:
            order.append(label)
            stack.pop()
            continue
        seen.add(child)
        stack.append((child, iter(rsucc[child])))
    order.reverse()
    return dominators(order, {label: list(succ.get(label, ())) for label in order}, root)


def dominates(idom, a, b):
    """True if a dominates b."""
    while True:
        if a == b:
            return True
        parent = idom.get(b)
        if parent is None or parent == b:
            return False
        b = parent


class Graph:
    def __init__(self, function):
        self.blocks = function.blocks
        self.entry = function.entry
        self.order = reverse_postorder(self.blocks, self.entry)
        self.position = {label: index for index, label in enumerate(self.order)}
        self.predecessors = {label: [] for label in self.order}
        for label in self.order:
            for successor in successors(self.blocks[label]):
                self.predecessors[successor].append(label)
        self.idom = dominators(self.order, self.predecessors, self.entry)
        self.loops = self.find_loops()
        self.ipdom = self.post_dominators()

    def find_loops(self):
        """{header: (body, latches)} for the natural loops; a back edge to a block that does not dominate
        its source (an irreducible loop) heads none."""
        latches = {}
        for label in self.order:
            for successor in successors(self.blocks[label]):
                if self.position[successor] <= self.position[label] and dominates(self.idom, successor, label):
                    latches.setdefault(successor, []).append(label)
        loops = {}
        for header, sources in latches.items():
            body = {header}
            stack = [source for source in sources if source != header]
            body |= set(stack)
            while stack:
                label = stack.pop()
                for predecessor in self.predecessors[label]:
                    if predecessor not in body:
                        body.add(predecessor)
                        stack.append(predecessor)
            loops[header] = (body, sources)
        return loops

    def post_dominators(self):
        """Immediate post-dominators, with EXIT after every block that ends the function. A loop nothing
        leaves is taken to leave from its header, so every block reaches EXIT. A small block that ends the
        function is written where it is jumped to ("if (x) { ...; return; }"), so a way into one does not
        count: what the other ways meet at is where the code goes on."""
        def written_in_place(label):
            block = self.blocks[label]
            if isinstance(block.end, (ir.Return, ir.TailCall, ir.InterruptReturn)):
                return len(block.statements) <= SMALL_EXIT
            # a short way out: code that only ends the function, small enough to be written where it is
            # jumped to ("if (hit) { ...; return; }")
            reach = self.reachable_from(label)
            return len(reach) <= 4 and all(not successors(self.blocks[inner]) or
                                           all(after in reach for after in successors(self.blocks[inner]))
                                           for inner in reach) \
                and all(label not in successors(self.blocks[inner]) for inner in reach) \
                and sum(len(self.blocks[inner].statements) + 1 for inner in reach) <= TAIL_COPY
        def kept(label):
            targets = list(successors(self.blocks[label]))
            # an ending one of the other ways also runs into is where they meet ("if (x) { ... }" and then it)
            joined = {after for target in targets for after in successors(self.blocks[target])}
            onward = [target for target in targets if not written_in_place(target) or target in joined]
            # a loop's way out stays: leaving the loop is where its body goes on
            if any(self.position[target] > self.position[label] for target in onward):
                return onward
            return targets
        succ = {label: kept(label) for label in self.order}
        for label in self.order:
            if not succ[label]:
                succ[label].append(EXIT)
        reaches = self.reaching_exit(succ)
        # a loop nothing leaves (its ways out are all written in place) is taken to leave where it goes round
        # again: its header keeps the block its branches meet at. Of such loops, one no other one comes after
        # (a loop that runs into another leaves through that one), the outermost first
        while True:
            stuck = sorted((header for header in self.loops if header not in reaches),
                           key=lambda label: self.position[label])
            if not stuck:
                break
            header = next((header for header in stuck
                           if self.reach_in(succ, header) <= self.reach_back(succ, header)), stuck[0])
            for latch in self.loops[header][1]:
                succ[latch].append(EXIT)
            reaches = self.reaching_exit(succ)
        for label in self.order:
            if label not in reaches:
                succ[label].append(EXIT)
        reverse_preds = {label: [] for label in self.order}
        reverse_preds[EXIT] = []
        for label, targets in succ.items():
            for target in targets:
                reverse_preds.setdefault(target, [])
        # the reverse graph: EXIT first, edges backwards
        rsucc = {label: [] for label in list(self.order) + [EXIT]}
        rpred = {label: [] for label in list(self.order) + [EXIT]}
        for label, targets in succ.items():
            for target in targets:
                rsucc[target].append(label)
                rpred[label].append(target)
        order = []
        seen = {EXIT}
        stack = [(EXIT, iter(rsucc[EXIT]))]
        while stack:
            label, children = stack[-1]
            advanced = False
            for child in children:
                if child not in seen:
                    seen.add(child)
                    stack.append((child, iter(rsucc[child])))
                    advanced = True
                    break
            if not advanced:
                order.append(label)
                stack.pop()
        order.reverse()
        return dominators(order, rpred, EXIT)

    def reachable_from(self, start):
        found = {start}
        stack = [start]
        while stack:
            label = stack.pop()
            for successor in successors(self.blocks[label]):
                if successor not in found:
                    found.add(successor)
                    stack.append(successor)
        return found

    @staticmethod
    def reach_in(succ, start):
        """The labels 'succ' (label -> the labels after it) leads to from 'start', with it."""
        found = {start}
        stack = [start]
        while stack:
            for target in succ.get(stack.pop(), ()):
                if target not in found:
                    found.add(target)
                    stack.append(target)
        return found

    @staticmethod
    def reach_back(succ, end):
        """The labels from which 'succ' leads to 'end', with it."""
        reverse = {}
        for label, targets in succ.items():
            for target in targets:
                reverse.setdefault(target, []).append(label)
        return Graph.reach_in(reverse, end)

    def reaching_exit(self, succ):
        reverse = {}
        for label, targets in succ.items():
            for target in targets:
                reverse.setdefault(target, []).append(label)
        found = set()
        stack = [EXIT]
        while stack:
            label = stack.pop()
            for source in reverse.get(label, []):
                if source not in found:
                    found.add(source)
                    stack.append(source)
        return found


def split_loop_entries(function, notes):
    """A loop the code enters at two places (an unrolled loop whose odd count starts at its second half)
    has no header C can write. When one of the ways in is a small block whose only way on is the other
    one, the code that comes from outside gets a copy of that block: the loop then starts at the other
    block, and the copy runs before it. 'notes' as for split_small_exits."""
    blocks = function.blocks
    changed = True
    rounds = 0
    while changed and rounds < 8:
        changed = False
        rounds += 1
        graph = Graph(function)
        for component in strongly_connected(blocks, graph.order):
            if len(component) < 2:
                continue
            entries = {label for label in component
                       if label == function.entry or any(source not in component
                                                         for source in graph.predecessors.get(label, ()))}
            if len(entries) != 2 or function.entry in entries:
                continue
            for small in sorted(entries, key=str):
                other = (entries - {small}).pop()
                block = blocks[small]
                if len(block.statements) > SMALL_EXIT or not isinstance(block.end, ir.Goto) \
                        or block.end.target != other:
                    continue
                outside = [source for source in graph.predecessors[small] if source not in component]
                made = {}
                twin = copy.copy(block)
                twin.label = ("entry", small)
                twin.statements = [copy.deepcopy(statement, made) for statement in block.statements]
                twin.end = copy.deepcopy(block.end, made)
                for note in notes:
                    for key in [key for key in note if isinstance(key, int) and key in made]:
                        note[id(made[key])] = copied_note(note[key], made)
                    if ("end", id(block)) in note:
                        note[("end", id(twin))] = note[("end", id(block))]
                blocks[twin.label] = twin
                for source in outside:
                    retarget(blocks[source].end, small, twin.label)
                changed = True
                break
            if changed:
                break


def retarget(end, old, new):
    """Makes a block end that jumps to 'old' jump to 'new' instead."""
    if isinstance(end, ir.Goto) and end.target == old:
        end.target = new
    elif isinstance(end, ir.Branch):
        if end.target == old:
            end.target = new
        if end.otherwise == old:
            end.otherwise = new
    elif isinstance(end, ir.Switch):
        end.cases = {case: new if target == old else target for case, target in end.cases.items()}


def strongly_connected(blocks, order):
    """The strongly connected components of the blocks reachable from the entry (Tarjan)."""
    index, low, on_stack, stack, found = {}, {}, set(), [], []
    counter = [0]

    def visit(start):
        work = [(start, iter(successors(blocks[start])))]
        index[start] = low[start] = counter[0]
        counter[0] += 1
        stack.append(start)
        on_stack.add(start)
        while work:
            label, children = work[-1]
            advanced = False
            for child in children:
                if child not in index:
                    index[child] = low[child] = counter[0]
                    counter[0] += 1
                    stack.append(child)
                    on_stack.add(child)
                    work.append((child, iter(successors(blocks[child]))))
                    advanced = True
                    break
                if child in on_stack:
                    low[label] = min(low[label], index[child])
            if advanced:
                continue
            work.pop()
            if work:
                low[work[-1][0]] = min(low[work[-1][0]], low[label])
            if low[label] == index[label]:
                component = set()
                while True:
                    member = stack.pop()
                    on_stack.discard(member)
                    component.add(member)
                    if member == label:
                        break
                found.append(component)

    for label in order:
        if label not in index:
            visit(label)
    return found


def copied_note(value, made):
    """A note about a node, for its copy: the nodes it names (a field's base register) are the copy's."""
    if isinstance(value, tuple):
        return tuple(made.get(id(part), part) if not isinstance(part, (int, str, bool, type(None))) else part
                     for part in value)
    return value


def split_small_exits(function, notes):
    """A small block that ends the function and is jumped to from several places gets a copy of its
    own for each (they are written where they are jumped to anyway): each copy then knows which values
    reach it, so a result handed back as it came in is not copied back. 'notes' are the dicts keyed by
    the id of a statement or ("end", id of a block) (cleanup.py's liveness, pointers.py's fields),
    filled in for the copies."""
    thread_jumps(function)
    join_conditions(function)    # "if (a || b) return": one way in after all
    predecessors = {}
    for label, block in function.blocks.items():
        for successor in successors(block):
            predecessors.setdefault(successor, []).append(label)
    for label in list(function.blocks):
        block = function.blocks[label]
        sources = predecessors.get(label, [])
        if not isinstance(block.end, (ir.Return, ir.TailCall)) or len(block.statements) > SMALL_EXIT \
                or len(sources) < 2 or label == function.entry:
            continue
        for number, source in enumerate(dict.fromkeys(sources[1:]), 2):
            # a deep copy: the twin's expressions are its own nodes, so what is noted about a node (the
            # variable a register read finds, the field a load reads) can differ between the copies
            made = {}
            twin = copy.copy(block)
            twin.label = ("exit", label, number)
            twin.statements = [copy.deepcopy(statement, made) for statement in block.statements]
            twin.end = copy.deepcopy(block.end, made)
            for note in notes:
                for key in [key for key in note if isinstance(key, int) and key in made]:
                    note[id(made[key])] = copied_note(note[key], made)
                if ("end", id(block)) in note:
                    note[("end", id(twin))] = note[("end", id(block))]
            function.blocks[twin.label] = twin
            end = function.blocks[source].end
            if isinstance(end, ir.Goto) and end.target == label:
                end.target = twin.label
            elif isinstance(end, ir.Branch):
                if end.target == label:
                    end.target = twin.label
                if end.otherwise == label:
                    end.otherwise = twin.label
            elif isinstance(end, ir.Switch):
                end.cases = {case: twin.label if target == label else target for case, target in end.cases.items()}


# ---- building the tree ----

def thread_jumps(function):
    """A jump to a block that does nothing but jump on goes straight to where that one leads (the V60
    tables of "br" instructions are made of such blocks)."""
    blocks = function.blocks

    def final(label):
        seen = set()
        while label != function.entry and label not in seen:
            block = blocks[label]
            if block.statements or not isinstance(block.end, ir.Goto):
                break
            seen.add(label)
            label = block.end.target
        return label

    for block in blocks.values():
        end = block.end
        if isinstance(end, ir.Goto):
            end.target = final(end.target)
        elif isinstance(end, ir.Branch):
            end.target = final(end.target)
            end.otherwise = final(end.otherwise)
        elif isinstance(end, ir.Switch):
            end.cases = {case: final(target) for case, target in end.cases.items()}


def join_conditions(function):
    """"if (a) goto X; if (b) goto X; ..." is one test: if (a || b). A block that only branches, that only
    one block jumps to, and that shares a target with that block, goes into its condition (|| or &&, which
    evaluate in the same order and skip what the jump skipped)."""
    blocks = function.blocks
    changed = True
    while changed:
        changed = False
        predecessors = {}
        for label, block in blocks.items():
            for successor in block.successors():
                predecessors.setdefault(successor, []).append(label)
        for label, block in blocks.items():
            end = block.end
            if not isinstance(end, ir.Branch):
                continue
            for side in ("otherwise", "target"):
                inner_label = getattr(end, side)
                inner = blocks.get(inner_label)
                if inner is None or inner_label == label or inner.statements or not isinstance(inner.end, ir.Branch) \
                        or predecessors.get(inner_label) != [label] or inner_label == function.entry:
                    continue
                shared = end.target if side == "otherwise" else end.otherwise
                c1, c2 = end.condition, inner.end.condition
                if side == "otherwise":
                    # if (c1) X; else if (c2 ...) -> one of inner's targets is X
                    if inner.end.target == shared:
                        new = Branch_parts(logical("lor", c1, c2), shared, inner.end.otherwise)
                    elif inner.end.otherwise == shared:
                        new = Branch_parts(logical("lor", c1, negate(c2)), shared, inner.end.target)
                    else:
                        continue
                else:
                    # if (c1) { if (c2 ...) } else Y -> one of inner's targets is Y
                    if inner.end.otherwise == shared:
                        new = Branch_parts(logical("land", c1, c2), inner.end.target, shared)
                    elif inner.end.target == shared:
                        new = Branch_parts(logical("land", c1, negate(c2)), inner.end.otherwise, shared)
                    else:
                        continue
                end.condition, end.target, end.otherwise = new
                del blocks[inner_label]
                changed = True
                break
            if changed:
                break


def Branch_parts(condition, target, otherwise):
    return condition, target, otherwise


def logical(name, a, b):
    return ir.op(name, (a, b), 1)


@dataclass
class LoopContext:
    header: object
    body: set            # the natural loop
    region: set          # the body, and code only the loop leads to that never comes back: written inside
    follow: object       # where the loop goes on
    in_switch: bool = False

    def inside_switch(self):
        return LoopContext(self.header, self.body, self.region, self.follow, True)


class Structurer:
    def __init__(self, function):
        thread_jumps(function)
        join_conditions(function)
        self.function = function
        self.blocks = function.blocks
        self.graph = Graph(function)
        self.emitted = set()
        self.labels = set()        # blocks something jumps to with goto
        self.pending = []          # of those, the ones not written yet
        self.meeting_ends = set()  # loop headers whose loop ends where all its ways out meet (loop_follow)
        self.loop_ipdoms = {}      # loop header -> the post-dominators inside its loop (loop_ipdom)

    def small_exit(self, label):
        block = self.blocks[label]
        return (isinstance(block.end, (ir.Return, ir.TailCall, ir.InterruptReturn))
                and len(block.statements) <= SMALL_EXIT)

    def loop_follow(self, header):
        chosen = self.natural_follow(header)
        body, _ = self.graph.loops[header]
        meet = self.graph.ipdom.get(header)
        if meet in (None, EXIT) or meet in body:
            return chosen
        if meet != chosen:
            ways = {successor for label in body for successor in successors(self.blocks[label])
                    if successor not in body and not self.small_exit(successor)}
            # a way out that does not run into the chosen end would need a goto there: the loop then ends
            # where all its ways out meet (its header's immediate post-dominator)
            if chosen is not None and all(self.post_dominates(chosen, way) for way in ways if way != chosen):
                return chosen
        # (each way out is then written in the loop up to there, ending in break)
        self.meeting_ends.add(header)
        return meet

    def post_dominates(self, later, label):
        """True if every way from 'label' to the end of the function passes through 'later'."""
        seen = set()
        while label not in (None, EXIT) and label not in seen:
            if label == later:
                return True
            seen.add(label)
            label = self.graph.ipdom.get(label)
        return False

    def natural_follow(self, header):
        body, latches = self.graph.loops[header]
        # where the step that goes round again leaves the loop ("blocks_left--; if (blocks_left != 0) go
        # round; else go on"): the loop's natural end, so that the jumps to that step read as a copy of it
        # ending in continue, and break
        ends = {target for latch in latches if isinstance(self.blocks[latch].end, ir.Branch)
                for target in successors(self.blocks[latch]) if target not in body}
        if len(ends) == 1:
            end = next(iter(ends))
            if not self.small_exit(end):
                return end
        exits = {}
        for label in body:
            for successor in successors(self.blocks[label]):
                if successor not in body:
                    exits[successor] = exits.get(successor, 0) + 1
        candidates = [label for label in exits if not self.small_exit(label)] or list(exits)
        if not candidates:
            return None
        # the exit that the most code after the loop hangs from: it reaches the most blocks
        return max(candidates, key=lambda label: (len(self.reachable(label)), exits[label], -self.graph.position[label]))

    def reachable(self, start):
        return self.graph.reachable_from(start)

    def reachable_avoiding(self, start, avoided):
        """The blocks 'start' leads to without passing through 'avoided'."""
        found = {start}
        stack = [start]
        while stack:
            label = stack.pop()
            for successor in successors(self.blocks[label]):
                if successor not in found and successor != avoided:
                    found.add(successor)
                    stack.append(successor)
        return found

    def build(self):
        tree = self.region(self.graph.entry, None, None)
        # code only gotos reach goes after the rest, each piece ending the function or jumping back
        while self.pending:
            label = self.pending.pop(0)
            if label in self.emitted:
                continue
            tree += self.region(label, None, None)
        return tree

    def copy_tail(self, target):
        """The code from 'target' on, written again: when all of it is small, never comes back to
        'target' and needs no goto of its own. None otherwise."""
        reach = self.reachable(target)
        if any(target in successors(self.blocks[label]) for label in reach):
            return None
        if sum(len(self.blocks[label].statements) + 1 for label in reach) > TAIL_COPY:
            return None
        copy = Structurer.__new__(Structurer)
        copy.function, copy.blocks, copy.graph = self.function, self.blocks, self.graph
        copy.emitted, copy.labels, copy.pending = set(), set(), []
        copy.meeting_ends, copy.loop_ipdoms = self.meeting_ends, self.loop_ipdoms
        nodes = copy.region(target, None, None)
        if copy.labels:
            return None
        return without_labels(nodes)

    def copy_until(self, target, stop, loop):
        """The code from 'target' up to 'stop' (where the code the jump comes from goes on), written again:
        when all of it is small, comes back neither to 'target' nor to code before it, and needs no goto of
        its own (a jump into a branch already written, whose code then joins this one's). None otherwise."""
        if stop is None and loop is None:
            return None    # (a tail: copy_tail's)
        reach = {target}
        stack = [target]
        while stack:
            label = stack.pop()
            for successor in successors(self.blocks[label]):
                if successor == stop or successor in reach:
                    continue
                if loop is not None and (successor == loop.header or successor == loop.follow
                                         or successor not in loop.region):
                    continue    # inside a loop: continue, break, or a way out written as such in the copy
                reach.add(successor)
                stack.append(successor)
        if any(target in successors(self.blocks[label]) for label in reach):
            return None
        if sum(len(self.blocks[label].statements) + 1 for label in reach) > REGION_COPY:
            return None
        copy = Structurer.__new__(Structurer)
        copy.function, copy.blocks, copy.graph = self.function, self.blocks, self.graph
        copy.emitted, copy.labels, copy.pending = set(), set(), []
        copy.meeting_ends, copy.loop_ipdoms = self.meeting_ends, self.loop_ipdoms
        nodes = copy.region(target, stop, loop)
        if copy.labels:
            return None
        return without_labels(nodes)

    def goto(self, target, stop=None, loop=None):
        copied = self.copy_tail(target)
        if copied is not None:
            return copied
        copied = self.copy_until(target, stop, loop)
        if copied is not None:
            return copied
        self.labels.add(target)
        if target not in self.emitted:
            self.pending.append(target)
        return [Goto(target)]

    def jump(self, target, stop, loop):
        """What going to 'target' is from inside 'loop' with the region ending at 'stop': a list of nodes,
        and the block the walk goes on at (None when the jump ends the walk)."""
        if loop is not None:
            if target == loop.header:
                return [Continue()], None
            if target not in loop.region:
                if target == loop.follow:
                    # inside a switch, break would only leave the switch
                    return (self.goto(target) if loop.in_switch else [Break()]), None
                if self.small_exit(target):
                    return self.copy_exit(target), None
                block = self.blocks[target]
                if not loop.in_switch and isinstance(block.end, ir.Goto) and block.end.target == loop.follow \
                        and len(block.statements) <= SMALL_EXIT and target != self.function.entry:
                    # a few statements on the way out of the loop: written where the jump is, then break
                    return [Code(target, list(block.statements)), Break()], None
                return self.goto(target), None
        if target == stop:
            return [], None
        if loop is not None and COPY_LATCHES:
            latch = self.copied_latch(target, loop)
            if latch is not None:
                return latch, None
        if target in self.emitted:
            if self.small_exit(target):
                return self.copy_exit(target), None
            return self.goto(target, stop, loop), None
        return [], target

    def copied_latch(self, label, loop):
        """The few statements before 'loop' goes round again (the next voice: the pointer moved on, the count
        down), written again where the body jumps to them, ending in continue (and break when the count runs
        out); None when 'label' is not such a block."""
        if loop.in_switch or label not in loop.body or label == loop.header:
            return None
        block = self.blocks[label]
        if len(block.statements) > SMALL_EXIT:
            return None
        end = block.end
        if isinstance(end, ir.Goto) and end.target == loop.header:
            return [Code(label, list(block.statements)), Continue()]

        def again(side):
            """The nodes for a side of the branch that goes round again ([continue], after the statements of
            a block of its own on the way, the Z80's extra cycles of a jump taken), or None."""
            if side == loop.header:
                return [Continue()]
            way = self.blocks.get(side)
            if way is not None and side in loop.body and isinstance(way.end, ir.Goto) \
                    and way.end.target == loop.header and len(way.statements) <= SMALL_EXIT:
                return [Code(side, list(way.statements)), Continue()]
            return None
        if isinstance(end, ir.Branch):
            for side, other, condition in ((end.target, end.otherwise, end.condition),
                                           (end.otherwise, end.target, negate(end.condition))):
                nodes = again(side)
                if nodes is not None and other == loop.follow:
                    return [Code(label, list(block.statements)), If(negate(condition), [Break()])] + nodes
        return None

    def copy_exit(self, label):
        block = self.blocks[label]
        return [Code(label, list(block.statements)), End(block.end)]

    def region(self, start, stop, loop):
        """Nodes for the code from 'start' up to 'stop' (not included) inside 'loop' (a LoopContext or
        None)."""
        out = []
        current = start
        while current is not None and current != stop:
            if (loop is not None and current not in loop.region) or current in self.emitted:
                nodes, _ = self.jump(current, stop, loop)
                out += nodes
                break
            if current in self.graph.loops and (loop is None or loop.header != current):
                current = self.loop(current, out, stop, loop)
                continue
            current = self.block(current, out, stop, loop)
        return out

    def loop(self, header, out, stop, outer):
        body, _ = self.graph.loops[header]
        follow = self.loop_follow(header)
        region = set(body)
        if header in self.meeting_ends:
            # where the ways out meet: the code on each way up to there (not what follows that, even when it
            # comes back round an outer loop) is the loop's
            after = self.reachable_avoiding(follow, header)
            region |= {label for label in self.graph.order
                       if label not in after and dominates(self.graph.idom, header, label)}
        else:
            after = self.reachable(follow) if follow is not None else set()
            for label in self.graph.order:
                if label in body or label in after or (outer is not None and label in outer.body):
                    continue
                if dominates(self.graph.idom, header, label):
                    region.add(label)
        inner = LoopContext(header, body, region, follow)
        nodes = []
        current = self.block(header, nodes, None, inner)
        if current is not None:
            nodes += self.region(current, None, inner)
        # code of the loop only gotos reach (a way out of an inner loop that comes back into this one) goes at
        # the end of its body, after the round's end, rather than after the whole function
        while True:
            label = next((label for label in self.pending if label in body and label not in self.emitted), None)
            if label is None:
                break
            self.pending.remove(label)
            if not ends_flow(nodes):
                nodes.append(Continue())
            nodes += self.region(label, None, inner)
        out.append(Loop(nodes))
        if follow is None or not breaks_out(nodes):
            # (a loop nothing leaves with a break: what follows it comes only from elsewhere, which writes it
            # or jumps to it)
            return None
        nodes, next_label = self.jump(follow, stop, outer)
        out += nodes
        return next_label

    def block(self, label, out, stop, loop):
        """Writes one block and its end; returns where the walk goes on."""
        self.emitted.add(label)
        block = self.blocks[label]
        out.append(Label(label))
        if block.statements:
            out.append(Code(label, list(block.statements)))
        end = block.end
        if isinstance(end, (ir.Return, ir.TailCall, ir.InterruptReturn)):
            out.append(End(end))
            return None
        if isinstance(end, ir.Goto):
            nodes, next_label = self.jump(end.target, stop, loop)
            out += nodes
            return next_label
        if isinstance(end, ir.Branch):
            return self.branch(label, end, out, stop, loop)
        if isinstance(end, ir.Switch):
            return self.switch(label, end, out, stop, loop)
        raise ValueError(f"block end {end!r}")

    def follow_of(self, label, stop, loop):
        """Where the two or more ways out of 'label' meet again inside the current region, or None."""
        follow = self.graph.ipdom.get(label)
        if follow in (EXIT, None, label) or (loop is not None and (follow not in loop.region
                                                                   or follow == loop.header)):
            if loop is None:
                return None
            # inside a loop, the ways that leave it (continue, break) do not meet the others: where those meet
            follow = self.loop_ipdom(loop).get(label)
            if follow in (LOOP_EXIT, None, label) or follow not in loop.region or follow == loop.header:
                return None
        if follow != stop and follow in self.emitted:
            return None
        return follow

    def loop_ipdom(self, loop):
        """Immediate post-dominators inside 'loop': its ways out (continue, break, a jump out of its region, a
        small exit) all end at LOOP_EXIT, and a block every way from which ends so (it is written where it is
        reached) is no place for the other ways to meet."""
        found = self.loop_ipdoms.get(loop.header)
        if found is not None:
            return found
        region = loop.region

        def out(target):
            return target == loop.header or target not in region or self.small_exit(target)
        after = {label: [LOOP_EXIT if out(target) else target for target in successors(self.blocks[label])]
                 or [LOOP_EXIT] for label in region}

        entries = {}
        for label, targets in after.items():
            for target in targets:
                entries.setdefault(target, set()).add(label)

        def leaving(label):
            """A short way out only one block leads to: a few blocks every way from which leaves the loop
            without meeting other code ("if (count == 0) break; continue;"), written where it is reached. (The
            step the ways of the body meet at before going round again is no such thing.)"""
            if len(entries.get(label, ())) != 1:
                return False
            reach, stack = {label}, [label]
            while stack:
                for target in after[stack.pop()]:
                    if target == LOOP_EXIT or target in reach:
                        continue
                    if target == label or len(reach) >= 4:
                        return False
                    reach.add(target)
                    stack.append(target)
            return all(all(target == LOOP_EXIT or target in reach for target in after[inner]) for inner in reach) \
                and all(label not in after[inner] for inner in reach) \
                and sum(len(self.blocks[inner].statements) + 1 for inner in reach) <= TAIL_COPY
        succ = {label: [target for target in targets if target != LOOP_EXIT and not leaving(target)]
                or [LOOP_EXIT] for label, targets in after.items()}
        reaching = Graph.reach_back(succ, LOOP_EXIT)
        for label in region:
            if label not in reaching:
                succ[label].append(LOOP_EXIT)    # (a loop inside whose ways out all end: it ends there)
        found = post_dominator_tree(succ, LOOP_EXIT)
        self.loop_ipdoms[loop.header] = found
        return found

    def branch(self, label, end, out, stop, loop):
        then_label, else_label = end.target, end.otherwise
        # an early way out: "if (x) { ...; return; }" and the walk goes on with the other side
        for exit_label, other, condition in ((then_label, else_label, end.condition),
                                            (else_label, then_label, negate(end.condition))):
            if loop is not None and exit_label == loop.follow:
                continue    # leaving the loop: a break, and the way out is written once after it
            if self.small_exit(exit_label) and not self.small_exit(other) and exit_label != stop \
                    and exit_label not in successors(self.blocks[other]):
                out.append(If(condition, self.copy_exit(exit_label)))
                nodes, next_label = self.jump(other, stop, loop)
                out += nodes
                return next_label
        follow = self.follow_of(label, stop, loop)
        region_stop = follow if follow is not None else stop
        then_nodes = self.side(then_label, region_stop, loop)
        else_nodes = self.side(else_label, region_stop, loop)
        if not then_nodes and else_nodes:
            out.append(If(negate(end.condition), else_nodes))
        else:
            out.append(If(end.condition, then_nodes, else_nodes))
        if follow is None:
            return None
        nodes, next_label = self.jump(follow, stop, loop)
        out += nodes
        return next_label

    def side(self, label, stop, loop):
        """One side of an if: the walk from 'label' until 'stop'."""
        if label == stop:
            return []
        nodes, next_label = self.jump(label, stop, loop)
        if next_label is None:
            return nodes
        return nodes + self.region(next_label, stop, loop)

    def switch(self, label, end, out, stop, loop):
        follow = self.follow_of(label, stop, loop)
        region_stop = follow if follow is not None else stop
        by_target = {}
        for case, target in end.cases.items():
            by_target.setdefault(target, []).append(case)
        cases = []
        inner = loop.inside_switch() if loop is not None else None
        for target, values in sorted(by_target.items(), key=lambda item: min(item[1])):
            body = self.side(target, region_stop, inner)
            if not ends_flow(body):
                body.append(Break())
            cases.append((sorted(values), body))
        out.append(Switch(end.address, cases))
        if follow is None:
            return None
        nodes, next_label = self.jump(follow, stop, loop)
        out += nodes
        return next_label


def without_labels(nodes):
    """'nodes' with their labels taken out (a copy must not repeat them)."""
    found = []
    for node in nodes:
        if isinstance(node, Label):
            continue
        if isinstance(node, If):
            node = If(node.condition, without_labels(node.then), without_labels(node.otherwise))
        elif isinstance(node, Loop):
            node = Loop(without_labels(node.body), node.kind, node.condition)
        elif isinstance(node, Switch):
            node = Switch(node.value, [(values, without_labels(body)) for values, body in node.cases])
        found.append(node)
    return found


def ends_flow(nodes):
    """True if control never runs off the end of 'nodes'."""
    if not nodes:
        return False
    last = nodes[-1]
    if isinstance(last, (End, Goto, Break, Continue)):
        return True
    if isinstance(last, If):
        return bool(last.otherwise) and ends_flow(last.then) and ends_flow(last.otherwise)
    if isinstance(last, Loop):
        return last.kind == "forever" and not contains_break(last.body)
    return False


def contains_break(nodes):
    for node in nodes:
        if isinstance(node, Break):
            return True
        if isinstance(node, If) and (contains_break(node.then) or contains_break(node.otherwise)):
            return True
        if isinstance(node, Switch):
            continue    # a break there leaves the switch
    return False


# ---- conditions ----

NEGATED = {"eq": "ne", "ne": "eq"}


def breaks_out(nodes):
    """True when 'nodes' (a loop's body) hold a break that leaves that loop: not one of an inner loop, nor of
    a switch (where break leaves the switch)."""
    for node in nodes:
        if isinstance(node, Break):
            return True
        if isinstance(node, If) and (breaks_out(node.then) or breaks_out(node.otherwise)):
            return True
    return False


def negate(condition):
    """The opposite condition, said directly where it can be."""
    if isinstance(condition, ir.Op):
        name, args = condition.name, condition.args
        if name in NEGATED:
            if name == "eq" and args[1] == ir.Const(0, 1) and args[0].bits == 1:
                return args[0]
            return ir.op(NEGATED[name], args, 1)
        if name == "lt":
            return ir.op("le", (args[1], args[0]), 1)
        if name == "le":
            return ir.op("lt", (args[1], args[0]), 1)
        if name == "ult":
            return ir.op("ule", (args[1], args[0]), 1)
        if name == "ule":
            return ir.op("ult", (args[1], args[0]), 1)
        if name == "lor":
            return ir.op("land", (negate(args[0]), negate(args[1])), 1)
        if name == "land":
            return ir.op("lor", (negate(args[0]), negate(args[1])), 1)
    return ir.op("eq", (condition, ir.Const(0, 1)), 1)


# ---- tidying the tree ----

def tidy(nodes, labels, in_loop=False):
    """Labels nothing jumps to go, loops get their conditions (while, do-while), a continue that ends a
    loop body goes, and so on."""
    out = []
    for node in nodes:
        if isinstance(node, Label) and node.label not in labels:
            continue
        if isinstance(node, If):
            node.then = tidy(node.then, labels, in_loop)
            node.otherwise = tidy(node.otherwise, labels, in_loop)
            if not node.then and node.otherwise:
                node = If(negate(node.condition), node.otherwise)
            if not node.then and not node.otherwise:
                continue
            # no else after a side that does not come back: "if (x) return; rest"
            if node.otherwise and ends_flow(node.then):
                out.append(If(node.condition, node.then))
                out += node.otherwise
                continue
            if node.otherwise and ends_flow(node.otherwise) and not ends_flow(node.then):
                out.append(If(negate(node.condition), node.otherwise))
                out += node.then
                continue
        elif isinstance(node, Loop):
            node.body = tidy(node.body, labels, True)
            node = loop_shape(node)
        elif isinstance(node, Switch):
            node.cases = [(values, tidy(body, labels, in_loop)) for values, body in node.cases]
        out.append(node)
    return out


def loop_shape(loop):
    body = loop.body
    # the body ends "if (c) continue; break;" or "if (c) continue;" followed by nothing: do { } while (c)
    while body and isinstance(body[-1], Continue):
        body.pop()
    # the body ends "if (c) continue; a few statements; break;": the round goes on anyway, "if (!c) { ...;
    # break; }"
    if len(body) >= 3 and isinstance(body[-1], Break):
        for position in range(len(body) - 3, -1, -1):
            node = body[position]
            if isinstance(node, If) and not node.otherwise and len(node.then) == 1 \
                    and isinstance(node.then[0], Continue):
                return loop_shape(Loop(body[:position] + [If(negate(node.condition), body[position + 1:])],
                                       "forever"))
            if not isinstance(node, Code):
                break
    # "if (c) continue; else break;" at the end: do { } while (c)
    if body and isinstance(body[-1], If) and len(body[-1].then) == 1 and len(body[-1].otherwise) == 1 \
            and not uses_continue(body[:-1]):
        last = body[-1]
        if isinstance(last.then[0], Continue) and isinstance(last.otherwise[0], Break):
            return Loop(body[:-1], "do", last.condition)
        if isinstance(last.then[0], Break) and isinstance(last.otherwise[0], Continue):
            return Loop(body[:-1], "do", negate(last.condition))
    if len(body) >= 2 and isinstance(body[-1], Break) and isinstance(body[-2], If) \
            and body[-2].then and len(body[-2].then) == 1 and isinstance(body[-2].then[0], Continue) \
            and not body[-2].otherwise and not uses_continue(body[:-2]):
        condition = body[-2].condition
        return Loop(body[:-2], "do", condition)
    if body and isinstance(body[-1], If) and not body[-1].otherwise and len(body[-1].then) == 1 \
            and isinstance(body[-1].then[0], Break) and not uses_continue(body[:-1]):
        return Loop(body[:-1], "do", negate(body[-1].condition))
    # the body starts "if (c) break;": while (!c) { }
    if body and isinstance(body[0], If) and not body[0].otherwise and len(body[0].then) == 1 \
            and isinstance(body[0].then[0], Break):
        return Loop(body[1:], "while", negate(body[0].condition))
    # the body ends "if (c) { ...; continue; } break;": the way out in the middle, "if (!c) break; ..."
    if len(body) >= 2 and isinstance(body[-1], Break) and isinstance(body[-2], If) and not body[-2].otherwise \
            and len(body[-2].then) >= 2 and isinstance(body[-2].then[-1], Continue):
        last = body[-2]
        return loop_shape(Loop(body[:-2] + [If(negate(last.condition), [Break()])] + last.then[:-1], "forever"))
    return Loop(body, "forever")


def uses_continue(nodes):
    for node in nodes:
        if isinstance(node, Continue):
            return True
        if isinstance(node, If) and (uses_continue(node.then) or uses_continue(node.otherwise)):
            return True
        if isinstance(node, Switch) and any(uses_continue(body) for _, body in node.cases):
            return True
    return False


def structure(function, control_name=None):
    """The tree for 'function', and the labels gotos still need: none when 'control_name' (label -> the name
    of a control variable for it) is given, as gotos.py then takes the gotos out."""
    structurer = Structurer(function)
    tree = tidy(structurer.build(), structurer.labels)
    fallthrough_cases(tree)
    drop_fallthrough_gotos(tree)
    group_cases(tree)
    if control_name is not None and goto_targets(tree):
        from . import gotos    # (it rewrites this module's tree)
        tree = gotos.eliminate(tree, control_name)
    labels = goto_targets(tree)
    return drop_labels(tree, labels), labels


def group_cases(nodes):
    """Cases that only go on in the same function (a table of jumps to the state code: "case 0: case 1:
    player_move(); return;") are one case with all their values."""
    for node in nodes:
        if isinstance(node, If):
            group_cases(node.then)
            group_cases(node.otherwise)
        elif isinstance(node, Loop):
            group_cases(node.body)
        elif isinstance(node, Switch):
            for _, body in node.cases:
                group_cases(body)
            grouped = []
            for values, body in node.cases:
                key = tail_target(body)
                same = next((index for index, (_, other) in enumerate(grouped)
                             if key is not None and tail_target(other) == key), None)
                if same is None:
                    grouped.append((list(values), body))
                else:
                    grouped[same] = (sorted(grouped[same][0] + list(values)), grouped[same][1])
            node.cases = grouped


def tail_target(body):
    """(target, entry) of a case body that is only a tail call, or None."""
    nodes = [node for node in body if not isinstance(node, Label)]
    if len(nodes) == 1 and isinstance(nodes[0], End) and isinstance(nodes[0].end, ir.TailCall):
        return nodes[0].end.target, nodes[0].end.entry
    return None


def fallthrough_cases(nodes):
    """A case that is only a goto into the middle of another case, where that one's code runs on into
    it (the original lays case 3's code just before case 2's), goes right after that case, which then
    falls through into it the way C cases do: "case 3: ...; case 2: ...; break;"."""
    for node in nodes:
        if isinstance(node, If):
            fallthrough_cases(node.then)
            fallthrough_cases(node.otherwise)
        elif isinstance(node, Loop):
            fallthrough_cases(node.body)
        elif isinstance(node, Switch):
            for _, body in node.cases:
                fallthrough_cases(body)
            changed = True
            while changed:
                changed = False
                for b_index, (b_values, b_body) in enumerate(node.cases):
                    if len(b_body) != 1 or not isinstance(b_body[0], Goto):
                        continue
                    label = b_body[0].label
                    for a_index, (a_values, a_body) in enumerate(node.cases):
                        position = next((index for index, inner in enumerate(a_body)
                                         if isinstance(inner, Label) and inner.label == label), None)
                        if a_index == b_index or position is None:
                            continue
                        head, tail = a_body[:position], a_body[position:]
                        node.cases[a_index] = (a_values, head)
                        moved = (b_values, tail)
                        del node.cases[b_index]
                        node.cases.insert(node.cases.index((a_values, head)) + 1, moved)
                        changed = True
                        break
                    if changed:
                        break


def drop_labels(nodes, labels):
    """The tree without the labels no goto jumps to (and the ifs left with nothing to do)."""
    out = []
    for node in nodes:
        if isinstance(node, Label) and node.label not in labels:
            continue
        if isinstance(node, If):
            node.then = drop_labels(node.then, labels)
            node.otherwise = drop_labels(node.otherwise, labels)
            if not node.then and not node.otherwise:
                continue
            if not node.then:
                node = If(negate(node.condition), node.otherwise)
        elif isinstance(node, Loop):
            node.body = drop_labels(node.body, labels)
        elif isinstance(node, Switch):
            node.cases = [(values, drop_labels(body, labels)) for values, body in node.cases]
        out.append(node)
    return out


def drop_fallthrough_gotos(nodes, follow=frozenset()):
    """A goto that ends a branch, to a label the code reaches anyway when the branch ends (the label just
    after the if), goes. 'follow': the labels falling off the end of 'nodes' reaches."""
    for position, node in enumerate(nodes):
        after = set()
        rest = position + 1
        while rest < len(nodes) and isinstance(nodes[rest], Label):
            after.add(nodes[rest].label)
            rest += 1
        if rest == len(nodes):
            after |= follow
        if isinstance(node, If):
            drop_fallthrough_gotos(node.then, frozenset(after))
            drop_fallthrough_gotos(node.otherwise, frozenset(after))
        elif isinstance(node, Loop):
            drop_fallthrough_gotos(node.body)
        elif isinstance(node, Switch):
            for _, body in node.cases:
                drop_fallthrough_gotos(body)
    if nodes and isinstance(nodes[-1], Goto) and nodes[-1].label in follow:
        nodes.pop()


def goto_targets(nodes):
    """The labels the gotos of a tree jump to."""
    found = set()
    for node in nodes:
        if isinstance(node, Goto):
            found.add(node.label)
        elif isinstance(node, If):
            found |= goto_targets(node.then) | goto_targets(node.otherwise)
        elif isinstance(node, Loop):
            found |= goto_targets(node.body)
        elif isinstance(node, Switch):
            for _, body in node.cases:
                found |= goto_targets(body)
    return found
