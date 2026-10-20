"""
Name:        gotos.py
Description: takes out of structure.py's tree the gotos it leaves, with control variables
Version:     0.1.0
Created:     16-10-2026
Modified:    16-10-2026
Author:      Sebastian Toyos

What the structurer cannot write as if, loops, switch, break and continue (a loop entered at two places,
a way out of two loops at once, branches whose code joins halfway) it leaves as a goto to a label. Those go
here, one at a time, by moves on the tree that keep what runs and in which order (Erosa and Hendren, "Taming
control flow: a structured approach to eliminating goto statements", 1994). A control variable of the C, a
bool named after the label, carries the jump across the braces. It is true only on the way from a goto to
its label: the label clears it.

- the code after a label at the top level of a branch, that the jumps reach from another branch of the
  same if or switch, goes after the if or switch: it runs when the variable is set ("if (x_overlap) {
  ... }"), and the branch that ran into it sets it ("x_overlap = true;");
- a goto leaves the statement it is in: its condition goes into the variable ("found = x;"), the rest of
  its list runs when the variable is clear ("if (!found) { ... }"), a loop or a switch is left with break,
  and after the statement the goto stands again ("if (found) goto L;");
- a goto goes into the statement its label is in: the if takes "found ||" in its condition (a loop
  "while (found || ...)"), and the goto, at the start of the branch, goes on moving; when the label comes
  before the goto, a loop around the two takes the goto back to before the label's statement;
- a goto next to its label: a jump forward is an if around what it skips, a jump back a do-while.

Then what nothing needs goes: a setting nothing reads before the next one (the clearing at a label the code
never comes back to), a variable that is true wherever it is read, and "if (x) found = true; else { ...;
found = y; }" reads "found = x; if (!found) { ...; found = y; }".
"""
from v60 import ir

from .structure import Break, Continue, End, Goto, If, Label, Loop, Mark, Switch, contains_break, negate

TRUE = ir.Const(1, 1)
FALSE = ir.Const(0, 1)
MOVES = 10000    # moves for one goto, at most (each takes it a level out or in: far fewer)


class GotoError(Exception):
    pass


fresh_names = set()    # the names the loops new_loop makes have used in the tree being done


def fresh(name):
    """'name', or name_2, name_3... the first the tree has not used yet."""
    found, number = name, 2
    while found in fresh_names:
        found, number = f"{name}_{number}", number + 1
    fresh_names.add(found)
    return found


# ---- the tree ----

def children(node):
    """The lists of nodes inside 'node'."""
    if isinstance(node, If):
        return [node.then, node.otherwise]
    if isinstance(node, Loop):
        return [node.body]
    if isinstance(node, Switch):
        return [body for _, body in node.cases]
    return []


def walk(nodes):
    for node in nodes:
        yield node
        for inner in children(node):
            yield from walk(inner)


def find(nodes, test, path=()):
    """The way down to the first node 'test' accepts: [(list, index), ...] from 'nodes' to the list it is
    in; None when there is none."""
    for index, node in enumerate(nodes):
        here = path + ((nodes, index),)
        if test(node):
            return list(here)
        for inner in children(node):
            found = find(inner, test, here)
            if found is not None:
                return found
    return None


def is_goto(node):
    """A goto as a statement of its own: "if (condition) goto L;"."""
    return isinstance(node, If) and len(node.then) == 1 and isinstance(node.then[0], Goto) and not node.otherwise


def normalize(nodes):
    """Every goto as a statement of its own (is_goto)."""
    for index, node in enumerate(nodes):
        if isinstance(node, Goto):
            nodes[index] = If(TRUE, [node])
        elif not is_goto(node):
            for inner in children(node):
                normalize(inner)


def ends(nodes):
    """True if control never runs off the end of 'nodes' (structure.ends_flow, with "if (true) goto L;")."""
    if not nodes:
        return False
    last = nodes[-1]
    if isinstance(last, (End, Goto, Break, Continue)) or (is_goto(last) and last.condition == TRUE):
        return True
    if isinstance(last, If):
        return bool(last.otherwise) and ends(last.then) and ends(last.otherwise)
    if isinstance(last, Loop):
        return last.kind == "forever" and not contains_break(last.body)
    return False


def control(name):
    return ir.Control(name)


def read_names(expression):
    """The control variables an expression reads."""
    if expression is None:
        return frozenset()
    return frozenset(inner.name for inner in ir.walk(expression) if isinstance(inner, ir.Control))


def reads(node, name):
    """True if the condition or value of 'node' itself (not of the nodes inside it) reads the variable."""
    value = node.condition if isinstance(node, (If, Loop)) else node.value if isinstance(node, Mark) else None
    return name in read_names(value)


# ---- eliminating ----

def eliminate(tree, control_name):
    """'tree' without gotos; control_name(label) is the name of the control variable for jumps to it."""
    normalize(tree)
    fresh_names.clear()
    names = {}
    for node in walk(tree):
        if is_goto(node) and node.then[0].label not in names:
            names[node.then[0].label] = control_name(node.then[0].label)
    # each variable is clear but on the way from a goto to its label: the label clears it
    for label, name in names.items():
        path = find(tree, lambda node: isinstance(node, Label) and node.label == label)
        if path is None:
            raise GotoError(f"goto {label!r} with no label")
        nodes, index = path[-1]
        nodes.insert(index + 1, Mark(name, FALSE))
    # the last goto first: one that jumps back into a branch that never runs on (the code after the branch
    # is then its else) is taken out before a goto in that branch changes that
    while True:
        found = [node for node in walk(tree) if is_goto(node)]
        if not found:
            break
        goto = found[-1]
        label = goto.then[0].label
        move(tree, goto, label, names[label])
    simplify(tree)
    return tree


def move(tree, goto, label, name):
    """Moves one goto (a statement of its own, 'goto') until it and 'label' stand in one list, then takes
    it out."""
    size = sum(1 for _ in walk(tree))
    for _ in range(MOVES):
        if sum(1 for _ in walk(tree)) > 4 * size + 100:
            raise GotoError(f"goto {label!r}: the tree keeps growing")
        goto_path = find(tree, lambda node: node is goto)
        label_path = find(tree, lambda node: isinstance(node, Label) and node.label == label)
        nodes, index = goto_path[-1]
        if nodes is label_path[-1][0]:
            beside(nodes, index, label_path[-1][1], goto)
            return
        if attach_rest(goto_path, label_path):
            continue
        if lift_tail(tree, goto_path, label_path, label, name):
            continue
        depth = next((position for position, (inner, _) in enumerate(label_path[:-1]) if inner is nodes), None)
        if depth is not None:
            # the label is inside a statement of the goto's list
            into(nodes, index, label_path, depth, goto, name)
            continue
        out_of(goto_path, goto, name)
    raise GotoError(f"goto {label!r} does not settle")


def marked(nodes, index, goto, name):
    """The goto at nodes[index] as "name = condition; if (name) goto L;": the index of the goto then."""
    if goto.condition == control(name):
        return index
    nodes.insert(index, Mark(name, goto.condition))
    goto.condition = control(name)
    return index + 1


def beside(nodes, index, label_index, goto):
    """The goto at nodes[index] and its label at nodes[label_index], in one list."""
    if index < label_index:
        # forward: what it skips runs when it does not jump
        skipped = nodes[index + 1:label_index]
        nodes[index:label_index] = [If(negate_condition(goto.condition), skipped)] if skipped else []
        return
    # back: what lies between the label and the goto, again while it jumps
    body = nodes[label_index + 1:index]
    nodes[label_index + 1:index + 1] = new_loop(body, goto.condition)


def negate_condition(condition):
    if condition == TRUE:
        return FALSE
    if condition == FALSE:
        return TRUE
    return negate(condition)


def new_loop(body, condition):
    """'body' in a new do-while loop on 'condition'. A break or continue in it that belonged to a loop around
    (or a switch, for break) leaves the new loop and is asked again after it, with a variable of its own."""
    exits = []
    rewrite_exits(body, exits, {}, in_switch=False)
    nodes = [Loop(body, "do", condition)]
    for kind, name in exits:
        nodes = [Mark(name, FALSE)] + nodes + [If(control(name), [Break() if kind == "break" else Continue()])]
    return nodes


def rewrite_exits(nodes, exits, names, in_switch):
    """Turns the break and continue of 'nodes' that a new loop around them would take into "x = true;
    break;", noting (kind, variable) in 'exits' ('names': kind -> the variable, one of its own for each new
    loop)."""
    index = 0
    while index < len(nodes):
        node = nodes[index]
        index += 1
        kind = "break" if isinstance(node, Break) and not in_switch else \
            "continue" if isinstance(node, Continue) else None
        if kind is not None:
            name = names.get(kind)
            if name is None:
                name = names[kind] = fresh("leave_loop" if kind == "break" else "next_round")
                exits.append((kind, name))
            nodes[index - 1:index] = [Mark(name, TRUE), Break()]
            index += 1    # (past the new break: it is the new loop's)
            continue
        if isinstance(node, If):
            rewrite_exits(node.then, exits, names, in_switch)
            rewrite_exits(node.otherwise, exits, names, in_switch)
        elif isinstance(node, Switch):
            for _, body in node.cases:
                rewrite_exits(body, exits, names, True)
    # (inside a loop of their own, break and continue are that loop's)


def out_of(goto_path, goto, name):
    """Takes the goto out of the statement it is in."""
    nodes, index = goto_path[-1]
    parent, parent_index = goto_path[-2]
    statement = parent[parent_index]
    index = marked(nodes, index, goto, name)
    rest = nodes[index + 1:]
    del nodes[index:]
    if isinstance(statement, (Loop, Switch)):
        nodes.append(If(control(name), [Break()]))    # (out of a switch, too)
        nodes.extend(rest)
    elif rest:
        nodes.append(If(negate(control(name)), rest))
    parent[parent_index + 1:parent_index + 1] = [goto]


def into(nodes, index, label_path, depth, goto, name):
    """Takes the goto into the statement of its list its label is in."""
    statement_index = label_path[depth][1]
    if statement_index < index:
        lift(nodes, index, statement_index, goto, name)
        return
    statement = nodes[statement_index]
    branch = label_path[depth + 1][0]
    index = marked(nodes, index, goto, name)
    statement_index = next(position for position, node in enumerate(nodes) if node is statement)
    skipped = nodes[index + 1:statement_index]
    nodes[index:statement_index] = [If(negate(control(name)), skipped)] if skipped else []
    if isinstance(statement, If):
        if branch is statement.then:
            statement.condition = logical("lor", control(name), statement.condition)
        else:
            statement.condition = logical("land", negate(control(name)), statement.condition)
    elif isinstance(statement, Loop):
        if statement.kind == "while":
            statement.condition = logical("lor", control(name), statement.condition)
    else:
        raise GotoError("a goto into a switch")
    branch.insert(0, goto)


def lift(nodes, index, statement_index, goto, name):
    """The label is in a statement before the goto: a loop around the two, its first statement the goto, which
    then jumps forward into it."""
    index = marked(nodes, index, goto, name)
    body = nodes[statement_index:index]    # (with the variable set where the goto was)
    nodes[statement_index:index + 1] = new_loop([goto] + body, control(name))


def logical(operator, a, b):
    if operator == "lor" and b == TRUE or operator == "land" and b == FALSE:
        return b
    return ir.op(operator, (a, b), 1)


def attach_rest(goto_path, label_path):
    """"if (x) { L: ...; <ends> } rest" with a goto to L somewhere in the rest -> the rest is the if's else
    (the branch never runs on into it): the goto is then in a branch of the if its label is in (lift_tail)."""
    if len(label_path) < 2:
        return False
    nodes, statement_index = label_path[-2]
    statement = nodes[statement_index]
    if not isinstance(statement, If) or statement.otherwise or label_path[-1][0] is not statement.then \
            or not ends(statement.then):
        return False
    position = next((index for inner, index in goto_path if inner is nodes), None)
    if position is None or position <= statement_index:
        return False
    statement.otherwise = nodes[statement_index + 1:]
    del nodes[statement_index + 1:]
    return True


def lift_tail(tree, goto_path, label_path, label, name):
    """A label at the top level of a branch of an if or a switch, the goto in another branch of it: the code
    from the label on goes after the if or switch, run when the variable is set; the branch that ran into the
    label sets it."""
    if len(label_path) < 2:
        return False
    branch, label_index = label_path[-1]
    parent, statement_index = label_path[-2]
    statement = parent[statement_index]
    if not isinstance(statement, (If, Switch)):
        return False
    # the goto inside the statement, not in this branch
    if any(inner is branch for inner, _ in goto_path) or not any(
            inner is parent and position == statement_index for inner, position in goto_path):
        return False
    tail = branch[label_index + 1:]
    if isinstance(statement, Switch):
        if tail and isinstance(tail[-1], Break):
            tail = tail[:-1]
        elif not ends(tail):
            return False    # (a case that runs on into the next one)
        if switch_breaks(tail):
            return False    # (a break inside the tail would leave something else after it)
    head = branch[:label_index]
    falls = not ends(head)
    branch[:] = head + ([Mark(name, TRUE)] if falls else [])
    if isinstance(statement, Switch) and falls:
        branch.append(Break())
    parent[statement_index + 1:statement_index + 1] = [Label(label), If(control(name), tail)]
    # every goto to the label now sets the variable first
    while True:
        path = find(tree, lambda node: is_goto(node) and node.then[0].label == label
                    and node.condition != control(name))
        if path is None:
            break
        nodes, index = path[-1]
        marked(nodes, index, nodes[index], name)
    return True


def switch_breaks(nodes):
    """True if a break among 'nodes' (not in a loop or switch of their own) would leave a switch they are in."""
    for node in nodes:
        if isinstance(node, Break):
            return True
        if isinstance(node, If) and (switch_breaks(node.then) or switch_breaks(node.otherwise)):
            return True
    return False


# ---- what the variables need ----

def live_in(nodes, after, leave, again, notes):
    """The variables some way from the start of 'nodes' reads before setting: 'after' those live where the
    list ends, 'leave' and 'again' where a break and a continue go. 'notes' gets, for each setting, whether
    what it sets is read (id of the Mark -> True)."""
    live = after
    for node in reversed(nodes):
        live = live_node(node, live, leave, again, notes)
    return live


def live_node(node, after, leave, again, notes):
    if isinstance(node, Mark):
        notes[id(node)] = node.name in after
        return (after - {node.name}) | read_names(node.value)
    if isinstance(node, If):
        return read_names(node.condition) | live_in(node.then, after, leave, again, notes) \
            | live_in(node.otherwise, after, leave, again, notes)
    if isinstance(node, Loop):
        test = read_names(node.condition)
        start = frozenset()
        while True:
            if node.kind == "while":
                top = test | after | start    # the test, then the body or out
                body = live_in(node.body, top, after, top, notes)
                new = body
                result = top | body
            elif node.kind == "do":
                bottom = test | after | start    # the test at the end, then round again or out
                new = live_in(node.body, bottom, after, bottom, notes)
                result = new
            else:
                new = live_in(node.body, start, after, start, notes)
                result = new
            if new == start:
                return result
            start = new
    if isinstance(node, Switch):
        live = frozenset()
        following = after
        for _, body in reversed(node.cases):
            following = live_in(body, following, after, again, notes)    # (a case runs on into the next)
            live |= following
        return live
    if isinstance(node, End):
        return frozenset()
    if isinstance(node, Break):
        return leave
    if isinstance(node, Continue):
        return again
    return after


def drop_unread(tree):
    """Takes out the settings whose value nothing reads; True if one went."""
    notes = {}
    live_in(tree, frozenset(), frozenset(), frozenset(), notes)
    dead = {key for key, read in notes.items() if not read}
    if not dead:
        return False

    def drop(nodes):
        nodes[:] = [node for node in nodes if not (isinstance(node, Mark) and id(node) in dead)]
        for node in nodes:
            for inner in children(node):
                drop(inner)
    drop(tree)
    return True


# ---- simplifying ----

def simplify(tree):
    changed = True
    while changed:
        changed = drop_unread(tree)
        names = {node.name for node in walk(tree) if isinstance(node, Mark)}
        entry = live_in(tree, frozenset(), frozenset(), frozenset(), {})
        for name in sorted(names):
            marks = [node for node in walk(tree) if isinstance(node, Mark) and node.name == name]
            if name not in entry and all(mark.value == TRUE for mark in marks):
                # true wherever it is read
                replace_reads(tree, name)
                remove_marks(tree, name)
                changed = True
        changed |= tidy(tree)


def remove_marks(nodes, name):
    nodes[:] = [node for node in nodes if not (isinstance(node, Mark) and node.name == name)]
    for node in nodes:
        for inner in children(node):
            remove_marks(inner, name)


def replace_reads(nodes, name):
    for node in nodes:
        if isinstance(node, (If, Loop)) and node.condition is not None:
            node.condition = substitute(node.condition, name)
        for inner in children(node):
            replace_reads(inner, name)


def substitute(expression, name, value=TRUE):
    """'expression' with the variable 'name' known to be 'value' (TRUE or FALSE), folded."""
    if isinstance(expression, ir.Control) and expression.name == name:
        return value
    if isinstance(expression, ir.Op):
        args = tuple(substitute(arg, name, value) for arg in expression.args)
        if expression.name in ("lor", "land"):
            absorbing, neutral = (TRUE, FALSE) if expression.name == "lor" else (FALSE, TRUE)
            if absorbing in args:
                return absorbing
            rest = [arg for arg in args if arg != neutral]
            if not rest:
                return neutral
            if len(rest) == 1:
                return rest[0]
            return ir.op(expression.name, tuple(rest), expression.bits)
        if expression.name in ("eq", "ne") and args[1] == FALSE and args[0] in (TRUE, FALSE):
            return (TRUE if args[0] == FALSE else FALSE) if expression.name == "eq" else args[0]
        return ir.op(expression.name, args, expression.bits)
    return expression


def sets_on_every_way(node, name):
    """True if every way that falls out of 'node' has set the variable, none reading it first (a way that
    leaves the loop, the switch or the function falls out of nothing)."""
    if isinstance(node, Mark):
        return node.name == name and not reads(node, name)
    if isinstance(node, (End, Break, Continue, Goto)):
        return True
    if isinstance(node, If):
        return not reads(node, name) and list_sets(node.then, name) and list_sets(node.otherwise, name)
    return False


def list_sets(nodes, name):
    """True if every way that falls off the end of 'nodes' has set the variable, none reading it first."""
    for node in nodes:
        if sets_on_every_way(node, name):
            return True
        if reads(node, name) or any(reads(inner, name) for inner in walk(children_of(node))):
            return False
    return False


def children_of(node):
    return [inner for nodes in children(node) for inner in nodes]


def tidy(nodes):
    """Folds what the moves leave: ifs on constant conditions, empty ifs, loops that always go round, a test
    right after a setting to a constant, and "if (x) v = true; else { ...; v = y; }" into "v = x; if (!v) {
    ...; v = y; }"."""
    changed = False
    index = 0
    while index < len(nodes):
        node = nodes[index]
        for inner in children(node):
            changed |= tidy(inner)
        if isinstance(node, If):
            if node.condition == TRUE:
                nodes[index:index + 1] = node.then
                changed = True
                continue
            if node.condition == FALSE:
                nodes[index:index + 1] = node.otherwise
                changed = True
                continue
            if not node.then and not node.otherwise:
                del nodes[index]
                changed = True
                continue
            if not node.then:
                node.condition, node.then, node.otherwise = negate(node.condition), node.otherwise, []
                changed = True
            if len(node.then) == 1 and isinstance(node.then[0], Mark) and node.then[0].value == TRUE \
                    and node.otherwise and list_sets(node.otherwise, node.then[0].name):
                name = node.then[0].name
                nodes[index:index + 1] = [Mark(name, node.condition), If(negate(control(name)), node.otherwise)]
                changed = True
                continue
        if isinstance(node, Loop) and node.kind == "do" and node.condition == TRUE:
            node.kind, node.condition = "forever", None
            changed = True
        if isinstance(node, Loop) and node.kind == "do" and isinstance(node.condition, ir.Control) \
                and node.body and isinstance(node.body[-1], Mark) and node.body[-1].name == node.condition.name \
                and node.body[-1].value == TRUE:
            node.kind, node.condition = "forever", None
            changed = True
        if isinstance(node, Mark) and node.value in (TRUE, FALSE) and index + 1 < len(nodes) \
                and isinstance(nodes[index + 1], If) and reads(nodes[index + 1], node.name):
            # "found = true; if (found) break;": the test knows the value
            nodes[index + 1].condition = substitute(nodes[index + 1].condition, node.name, node.value)
            changed = True
        index += 1
    return changed
