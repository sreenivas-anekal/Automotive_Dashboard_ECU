#include <stdio.h>
#include <stdint.h>
#include <stdbool.h>

#define MAX_TASKS 4U

typedef void (*TaskCallback_t)(void);

typedef struct
{
    uint32_t interval_ms;
    uint32_t last_run_ms;
    TaskCallback_t callback;
} Task_t;

static Task_t task_list[MAX_TASKS];
static uint8_t task_count = 0U;

static int task1_count = 0;
static int task2_count = 0;

void Task_Scheduler_Init(void)
{
    task_count = 0U;

    for (uint8_t i = 0U; i < MAX_TASKS; i++)
    {
        task_list[i].interval_ms = 0U;
        task_list[i].last_run_ms = 0U;
        task_list[i].callback = NULL;
    }
}

void Task_Register(
    uint8_t task_id,
    uint32_t interval_ms,
    TaskCallback_t callback)
{
    if ((task_id < MAX_TASKS) &&
        (callback != NULL))
    {
        task_list[task_id].interval_ms = interval_ms;
        task_list[task_id].last_run_ms = 0U;
        task_list[task_id].callback = callback;

        if (task_id >= task_count)
        {
            task_count = task_id + 1U;
        }
    }
}

void Task_Scheduler_Run(uint32_t current_time_ms)
{
    for (uint8_t i = 0U; i < task_count; i++)
    {
        if (task_list[i].callback != NULL)
        {
            if ((current_time_ms -
                 task_list[i].last_run_ms) >=
                task_list[i].interval_ms)
            {
                task_list[i].last_run_ms =
                    current_time_ms;

                task_list[i].callback();
            }
        }
    }
}

static int tests_passed = 0;
static int tests_failed = 0;

static void check(
    const char *name,
    bool condition)
{
    if (condition)
    {
        printf("[PASS] %s\n", name);
        tests_passed++;
    }
    else
    {
        printf("[FAIL] %s\n", name);
        tests_failed++;
    }
}

static void Task1_Callback(void)
{
    task1_count++;
}

static void Task2_Callback(void)
{
    task2_count++;
}

int main(void)
{
    printf("========================================\n");
    printf(" Automotive Dashboard ECU\n");
    printf(" Task Scheduler Unit Test\n");
    printf(" STM32C031C6\n");
    printf("========================================\n\n");

    Task_Scheduler_Init();

    printf("TASK SCHEDULER INIT DONE\n");

    Task_Register(
        0U,
        100U,
        Task1_Callback
    );

    Task_Register(
        1U,
        250U,
        Task2_Callback
    );

    /* 0 ms: neither task is due yet */
    Task_Scheduler_Run(0U);

    check(
        "Task 1 does not execute before 100 ms",
        task1_count == 0
    );

    check(
        "Task 2 does not execute before 250 ms",
        task2_count == 0
    );

    /* 50 ms: neither task is due */
    Task_Scheduler_Run(50U);

    check(
        "Task 1 waits at 50 ms",
        task1_count == 0
    );

    check(
        "Task 2 waits at 50 ms",
        task2_count == 0
    );

    /* 100 ms: Task 1 executes */
    Task_Scheduler_Run(100U);

    check(
        "Task 1 executes at 100 ms",
        task1_count == 1
    );

    check(
        "Task 2 still waits at 100 ms",
        task2_count == 0
    );

    /* 250 ms: both tasks are due */
    Task_Scheduler_Run(250U);

    check(
        "Task 1 executes again at 250 ms",
        task1_count == 2
    );

    check(
        "Task 2 executes at 250 ms",
        task2_count == 1
    );

    /* 500 ms: both tasks are due again */
    Task_Scheduler_Run(500U);

    check(
        "Task 1 executes again at 500 ms",
        task1_count == 3
    );

    check(
        "Task 2 executes again at 500 ms",
        task2_count == 2
    );

    printf("\n----------------------------------------\n");

    printf(
        "Task 1 executions: %d\n",
        task1_count
    );

    printf(
        "Task 2 executions: %d\n",
        task2_count
    );

    printf("----------------------------------------\n");

    printf(
        "Tests passed: %d\n",
        tests_passed
    );

    printf(
        "Tests failed: %d\n",
        tests_failed
    );

    if (tests_failed == 0)
    {
        printf("RESULT: ALL TESTS PASSED\n");
        return 0;
    }

    printf("RESULT: TESTS FAILED\n");
    return 1;
}
