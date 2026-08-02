#include "task_scheduler.h"
#include <stddef.h>

#define MAX_TASKS 4

static Task_t task_list[MAX_TASKS];
static uint8_t task_count = 0;

void Task_Scheduler_Init(void) {
    task_count = 0;
    for (uint8_t i = 0; i < MAX_TASKS; i++) {
        task_list[i].interval_ms = 0;
        task_list[i].last_run_ms = 0;
        task_list[i].callback = NULL;
    }
}

void Task_Register(uint8_t task_id, uint32_t interval_ms, TaskCallback_t callback) {
    if (task_id < MAX_TASKS && callback != NULL) {
        task_list[task_id].interval_ms = interval_ms;
        task_list[task_id].last_run_ms = 0;
        task_list[task_id].callback = callback;
        if (task_id >= task_count) {
            task_count = task_id + 1;
        }
    }
}

void Task_Scheduler_Run(uint32_t current_time_ms) {
    for (uint8_t i = 0; i < task_count; i++) {
        if (task_list[i].callback != NULL) {
            /* Check if the interval has elapsed (handles millis() rollover cleanly) */
            if ((current_time_ms - task_list[i].last_run_ms) >= task_list[i].interval_ms) {
                task_list[i].last_run_ms = current_time_ms;
                task_list[i].callback(); /* Execute the task */
            }
        }
    }
}
