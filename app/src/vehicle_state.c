#include "vehicle_state.h"

/*
 * Current vehicle state is intentionally private to this module.
 * Other modules interact with the state machine through its public API.
 */
static VehicleState_t current_state = STATE_OFF;


void VehicleState_Init(void)
{
    current_state = STATE_OFF;
}


VehicleState_t VehicleState_ProcessEvent(VehicleEvent_t event)
{
    switch (current_state)
    {
        case STATE_OFF:

            if (event == EVENT_IGNITION_ON)
            {
                current_state = STATE_IGNITION;
            }

            break;


        case STATE_IGNITION:

            if (event == EVENT_SELF_TEST_OK)
            {
                current_state = STATE_IDLE;
            }
            else if (event == EVENT_SHUTDOWN)
            {
                current_state = STATE_OFF;
            }

            break;


        case STATE_IDLE:

            if (event == EVENT_ENGINE_START)
            {
                current_state = STATE_RUNNING;
            }
            else if (event == EVENT_CRITICAL_FAULT)
            {
                current_state = STATE_FAULT;
            }
            else if (event == EVENT_SHUTDOWN)
            {
                current_state = STATE_OFF;
            }

            break;


        case STATE_RUNNING:

            if (event == EVENT_VEHICLE_MOVE)
            {
                /*
                 * Vehicle movement does not change the operating state.
                 * Vehicle motion data will be handled by other modules.
                 */
            }
            else if (event == EVENT_CRITICAL_FAULT)
            {
                current_state = STATE_FAULT;
            }
            else if (event == EVENT_SHUTDOWN)
            {
                current_state = STATE_OFF;
            }

            break;


        case STATE_FAULT:

            if (event == EVENT_FAULT_CLEARED)
            {
                /*
                 * Return to IGNITION so that the normal startup/self-test
                 * sequence must be completed again.
                 */
                current_state = STATE_IGNITION;
            }

            break;


        default:

            /*
             * Defensive fallback.
             * An invalid internal state is treated as a fault condition.
             */
            current_state = STATE_FAULT;

            break;
    }

    return current_state;
}


VehicleState_t VehicleState_GetCurrent(void)
{
    return current_state;
}
