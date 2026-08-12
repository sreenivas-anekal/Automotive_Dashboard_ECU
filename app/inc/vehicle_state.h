#ifndef VEHICLE_STATE_H
#define VEHICLE_STATE_H

#ifdef __cplusplus
extern "C" {
#endif

/**
 * @brief Vehicle operating states.
 */
typedef enum
{
    STATE_OFF = 0,
    STATE_IGNITION,
    STATE_IDLE,
    STATE_RUNNING,
    STATE_FAULT
} VehicleState_t;

/**
 * @brief Events that can cause vehicle state transitions.
 */
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

/**
 * @brief Initialize the vehicle state machine.
 *
 * Sets the initial state to STATE_OFF.
 */
void VehicleState_Init(void);

/**
 * @brief Process an event and update the current vehicle state.
 *
 * Invalid events for the current state are ignored and the
 * current state is retained.
 *
 * @param event Event to process.
 * @return Updated vehicle state.
 */
VehicleState_t VehicleState_ProcessEvent(VehicleEvent_t event);

/**
 * @brief Get the current vehicle state.
 *
 * @return Current vehicle state.
 */
VehicleState_t VehicleState_GetCurrent(void);

#ifdef __cplusplus
}
#endif

#endif /* VEHICLE_STATE_H */
