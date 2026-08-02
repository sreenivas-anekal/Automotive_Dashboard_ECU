#ifndef TASK_SCHEDULER_H
#define TASK_SCHEDULER_H

#include <stdint.h>
#include <stdbool.h>

#ifdef __cplusplus
extern "C" {
#endif

typedef void (*TaskCallback_t)(void);

typedef struct {
    uint32_t interval_ms;     /* How often the task should run */
    uint32_t last_run_ms;     /* Timestamp of the last execution */
    TaskCallback_t callback;  /* Function pointer to the task */
} Task_t;

void Task_Scheduler_Init(void);
void Task_Register(uint8_t task_id, uint32_t interval_ms, TaskCallback_t callback);
void Task_Scheduler_Run(uint32_t current_time_ms);

#ifdef __cplusplus
}
#endif

#endif /* TASK_SCHEDULER_H */
