#include "vehicle_state.h"

/* Static variable tracking current state, encapsulated within this file */
static VehicleState_t current_state = STATE_OFF;

void VehicleState_Init(void) {
    current_state = STATE_OFF;
}

VehicleState_t VehicleState_ProcessEvent(VehicleEvent_t event) {
    switch (current_state) {
        case STATE_OFF:
            if (event == EVENT_IGNITION_ON) {
                current_state = STATE_IGNITION;
            }
            break;

        case STATE_IGNITION:
            if (event == EVENT_SELF_TEST_OK) {
                current_state = STATE_IDLE;
            } else if (event == EVENT_SHUTDOWN) {
                current_state = STATE_OFF;
            }
            break;

        case STATE_IDLE:
            if (event == EVENT_ENGINE_START) {
                current_state = STATE_RUNNING;
            } else if (event == EVENT_CRITICAL_FAULT) {
                current_state = STATE_FAULT;
            } else if (event == EVENT_SHUTDOWN) {
                current_state = STATE_OFF;
            }
            break;

        case STATE_RUNNING:
            if (event == EVENT_CRITICAL_FAULT) {
                current_state = STATE_FAULT;
            } else if (event == EVENT_VEHICLE_MOVE) {
                /* Stays running, handled via data updates */
            }
            break;

        case STATE_FAULT:
            if (event == EVENT_FAULT_CLEARED) {
                current_state = STATE_IGNITION; /* Force re-init check */
            }
            break;

        default:
            /* Safety fallback */
            current_state = STATE_FAULT;
            break;
    }

    return current_state;
}

VehicleState_t VehicleState_GetCurrent(void) {
    return current_state;
}
