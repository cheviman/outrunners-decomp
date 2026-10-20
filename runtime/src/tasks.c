/**
 *    Name:        tasks.c
 *    Description: native stacks for the game's tasks (host/tasks.h): fibers on Windows, ucontext elsewhere
 *    Version:     0.1.0
 *    Created:     20-10-2026
 *    Modified:    20-10-2026
 *    Author:      Sebastian Toyos
 *
 *    license:BSD-3-Clause
 */
#include "host/tasks.h"

#include <stdio.h>
#include <stdlib.h>

/* the C of a task nests as deep as the game's calls do, with the locals of the generated code */
#define TASK_STACK_SIZE (16u << 20)

#ifdef _WIN32

#define WIN32_LEAN_AND_MEAN
#include <windows.h>

struct host_task
{
	void *fiber;
	void (*start)(void);
};

static host_task main_task;
static host_task *current;

host_task *host_task_main(void)
{
	if (!main_task.fiber)
		main_task.fiber = IsThreadAFiber() ? GetCurrentFiber() : ConvertThreadToFiber(NULL);
	if (!main_task.fiber)
	{
		fprintf(stderr, "The main task could not be set up\n");
		exit(3);
	}
	if (!current)
		current = &main_task;
	return &main_task;
}

static VOID CALLBACK run_task(LPVOID parameter)
{
	((host_task *)parameter)->start();
	fprintf(stderr, "A task came to its end\n");
	exit(3);
}

host_task *host_task_create(void (*start)(void))
{
	host_task *task = calloc(1, sizeof(*task));

	if (task)
	{
		task->start = start;
		task->fiber = CreateFiberEx(64u << 10, TASK_STACK_SIZE, 0, run_task, task);
	}
	if (!task || !task->fiber)
	{
		fprintf(stderr, "A task could not be created\n");
		exit(3);
	}
	return task;
}

void host_task_switch(host_task *to)
{
	if (to == current)
		return;
	current = to;
	SwitchToFiber(to->fiber);
}

#else

#include <ucontext.h>

struct host_task
{
	ucontext_t context;
	void *stack;
	void (*start)(void);
};

static host_task main_task;
static host_task *current;

host_task *host_task_main(void)
{
	if (!current)
		current = &main_task;
	return &main_task;
}

static void run_task(void)
{
	current->start();
	fprintf(stderr, "A task came to its end\n");
	exit(3);
}

host_task *host_task_create(void (*start)(void))
{
	host_task *task = calloc(1, sizeof(*task));

	if (task)
		task->stack = malloc(TASK_STACK_SIZE);
	if (!task || !task->stack || getcontext(&task->context) != 0)
	{
		fprintf(stderr, "A task could not be created\n");
		exit(3);
	}
	task->start = start;
	task->context.uc_stack.ss_sp = task->stack;
	task->context.uc_stack.ss_size = TASK_STACK_SIZE;
	task->context.uc_link = NULL;
	makecontext(&task->context, run_task, 0);
	return task;
}

void host_task_switch(host_task *to)
{
	host_task *from = current;

	if (to == from)
		return;
	current = to;
	swapcontext(&from->context, &to->context);
}

#endif

host_task *host_task_current(void)
{
	return current;
}
