#include <stdio.h>
#include "vehicle_state.h"

static int tests_passed = 0;
static int tests_failed = 0;

static const char *state_to_string(VehicleState_t state)
{
    switch (state)
    {
        case STATE_OFF:
            return "OFF";

        case STATE_IGNITION:
            return "IGNITION";

        case STATE_IDLE:
            return "IDLE";

        case STATE_RUNNING:
            return "RUNNING";

        case STATE_FAULT:
            return "FAULT";

        default:
            return "UNKNOWN";
    }
}

static const char *event_to_string(VehicleEvent_t event)
{
    switch (event)
    {
        case EVENT_IGNITION_ON:
            return "IGNITION_ON";

        case EVENT_SELF_TEST_OK:
            return "SELF_TEST_OK";

        case EVENT_ENGINE_START:
            return "ENGINE_START";

        case EVENT_VEHICLE_MOVE:
            return "VEHICLE_MOVE";

        case EVENT_CRITICAL_FAULT:
            return "CRITICAL_FAULT";

        case EVENT_FAULT_CLEARED:
            return "FAULT_CLEARED";

        case EVENT_SHUTDOWN:
            return "SHUTDOWN";

        default:
            return "UNKNOWN";
    }
}

static void check_transition(
    VehicleState_t initial_state,
    VehicleEvent_t event,
    VehicleState_t expected)
{
    VehicleState_t actual;

    VehicleState_Init();

    /*
     * Drive the state machine into the required initial state.
     */
    switch (initial_state)
    {
        case STATE_OFF:
            break;

        case STATE_IGNITION:
            VehicleState_ProcessEvent(EVENT_IGNITION_ON);
            break;

        case STATE_IDLE:
            VehicleState_ProcessEvent(EVENT_IGNITION_ON);
            VehicleState_ProcessEvent(EVENT_SELF_TEST_OK);
            break;

        case STATE_RUNNING:
            VehicleState_ProcessEvent(EVENT_IGNITION_ON);
            VehicleState_ProcessEvent(EVENT_SELF_TEST_OK);
            VehicleState_ProcessEvent(EVENT_ENGINE_START);
            break;

        case STATE_FAULT:
            VehicleState_ProcessEvent(EVENT_IGNITION_ON);
            VehicleState_ProcessEvent(EVENT_SELF_TEST_OK);
            VehicleState_ProcessEvent(EVENT_CRITICAL_FAULT);
            break;

        default:
            break;
    }

    actual = VehicleState_ProcessEvent(event);

    if (actual == expected)
    {
        printf(
            "[PASS] %s + %s -> %s\n",
            state_to_string(initial_state),
            event_to_string(event),
            state_to_string(actual)
        );

        tests_passed++;
    }
    else
    {
        printf(
            "[FAIL] %s + %s | Expected: %s | Actual: %s\n",
            state_to_string(initial_state),
            event_to_string(event),
            state_to_string(expected),
            state_to_string(actual)
        );

        tests_failed++;
    }
}

int main(void)
{
    printf("========================================\n");
    printf(" Vehicle State Machine Unit Test\n");
    printf("========================================\n\n");


    /* ============================================================
     * STATE: OFF
     * ============================================================ */

    check_transition(
        STATE_OFF,
        EVENT_IGNITION_ON,
        STATE_IGNITION
    );

    check_transition(
        STATE_OFF,
        EVENT_SELF_TEST_OK,
        STATE_OFF
    );

    check_transition(
        STATE_OFF,
        EVENT_ENGINE_START,
        STATE_OFF
    );

    check_transition(
        STATE_OFF,
        EVENT_VEHICLE_MOVE,
        STATE_OFF
    );

    check_transition(
        STATE_OFF,
        EVENT_CRITICAL_FAULT,
        STATE_OFF
    );

    check_transition(
        STATE_OFF,
        EVENT_FAULT_CLEARED,
        STATE_OFF
    );

    check_transition(
        STATE_OFF,
        EVENT_SHUTDOWN,
        STATE_OFF
    );


    /* ============================================================
     * STATE: IGNITION
     * ============================================================ */

    check_transition(
        STATE_IGNITION,
        EVENT_IGNITION_ON,
        STATE_IGNITION
    );

    check_transition(
        STATE_IGNITION,
        EVENT_SELF_TEST_OK,
        STATE_IDLE
    );

    check_transition(
        STATE_IGNITION,
        EVENT_ENGINE_START,
        STATE_IGNITION
    );

    check_transition(
        STATE_IGNITION,
        EVENT_VEHICLE_MOVE,
        STATE_IGNITION
    );

    check_transition(
        STATE_IGNITION,
        EVENT_CRITICAL_FAULT,
        STATE_IGNITION
    );

    check_transition(
        STATE_IGNITION,
        EVENT_FAULT_CLEARED,
        STATE_IGNITION
    );

    check_transition(
        STATE_IGNITION,
        EVENT_SHUTDOWN,
        STATE_OFF
    );


    /* ============================================================
     * STATE: IDLE
     * ============================================================ */

    check_transition(
        STATE_IDLE,
        EVENT_IGNITION_ON,
        STATE_IDLE
    );

    check_transition(
        STATE_IDLE,
        EVENT_SELF_TEST_OK,
        STATE_IDLE
    );

    check_transition(
        STATE_IDLE,
        EVENT_ENGINE_START,
        STATE_RUNNING
    );

    check_transition(
        STATE_IDLE,
        EVENT_VEHICLE_MOVE,
        STATE_IDLE
    );

    check_transition(
        STATE_IDLE,
        EVENT_CRITICAL_FAULT,
        STATE_FAULT
    );

    check_transition(
        STATE_IDLE,
        EVENT_FAULT_CLEARED,
        STATE_IDLE
    );

    check_transition(
        STATE_IDLE,
        EVENT_SHUTDOWN,
        STATE_OFF
    );


    /* ============================================================
     * STATE: RUNNING
     * ============================================================ */

    check_transition(
        STATE_RUNNING,
        EVENT_IGNITION_ON,
        STATE_RUNNING
    );

    check_transition(
        STATE_RUNNING,
        EVENT_SELF_TEST_OK,
        STATE_RUNNING
    );

    check_transition(
        STATE_RUNNING,
        EVENT_ENGINE_START,
        STATE_RUNNING
    );

    check_transition(
        STATE_RUNNING,
        EVENT_VEHICLE_MOVE,
        STATE_RUNNING
    );

    check_transition(
        STATE_RUNNING,
        EVENT_CRITICAL_FAULT,
        STATE_FAULT
    );

    check_transition(
        STATE_RUNNING,
        EVENT_FAULT_CLEARED,
        STATE_RUNNING
    );

    check_transition(
        STATE_RUNNING,
        EVENT_SHUTDOWN,
        STATE_OFF
    );


    /* ============================================================
     * STATE: FAULT
     * ============================================================ */

    check_transition(
        STATE_FAULT,
        EVENT_IGNITION_ON,
        STATE_FAULT
    );

    check_transition(
        STATE_FAULT,
        EVENT_SELF_TEST_OK,
        STATE_FAULT
    );

    check_transition(
        STATE_FAULT,
        EVENT_ENGINE_START,
        STATE_FAULT
    );

    check_transition(
        STATE_FAULT,
        EVENT_VEHICLE_MOVE,
        STATE_FAULT
    );

    check_transition(
        STATE_FAULT,
        EVENT_CRITICAL_FAULT,
        STATE_FAULT
    );

    check_transition(
        STATE_FAULT,
        EVENT_FAULT_CLEARED,
        STATE_IGNITION
    );

    check_transition(
        STATE_FAULT,
        EVENT_SHUTDOWN,
        STATE_FAULT
    );


    /* ============================================================
     * FINAL RESULT
     * ============================================================ */

    printf("\n");
    printf("========================================\n");
    printf("Tests passed: %d\n", tests_passed);
    printf("Tests failed: %d\n", tests_failed);

    if (tests_failed == 0)
    {
        printf("RESULT: ALL 35 TESTS PASSED\n");
    }
    else
    {
        printf("RESULT: TEST FAILURE\n");
    }

    printf("========================================\n");

    return (tests_failed == 0) ? 0 : 1;
}
