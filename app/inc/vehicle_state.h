#ifndef VEHICLE_STATE_H
#define VEHICLE_STATE_H

#include <stdint.h>
#include <stdbool.h>

#ifdef __cplusplus
extern "C" {
#endif

typedef enum
{
    STATE_OFF = 0,
    STATE_IGNITION,
    STATE_IDLE,
    STATE_RUNNING,
    STATE_FAULT
} VehicleState_t;

typedef enum
{
    EVENT_IGNITION_ON = 0,
    EVENT_SELF_TEST_OK,
    EVENT_ENGINE_START,
    EVENT_VEHICLE_MOVE,
    EVENT_CRITICAL_FAULT,
    EVENT_FAULT_CLEARED,
    EVENT_SHUTDOWN
} VehicleEvent_t;

void VehicleState_Init(void);

VehicleState_t VehicleState_ProcessEvent(
    VehicleEvent_t event
);

VehicleState_t VehicleState_GetCurrent(void);

#ifdef __cplusplus
}
#endif

#endif /* VEHICLE_STATE_H */
