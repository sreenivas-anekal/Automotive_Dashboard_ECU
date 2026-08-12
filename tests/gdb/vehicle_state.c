#include <stdio.h>

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

static VehicleState_t current_state = STATE_OFF;

void VehicleState_Init(void)
{
    current_state = STATE_OFF;
}

VehicleState_t VehicleState_ProcessEvent(
    VehicleEvent_t event)
{
    switch (current_state)
    {
        case STATE_OFF:

            if (event == EVENT_IGNITION_ON)
                current_state = STATE_IGNITION;

            break;

        case STATE_IGNITION:

            if (event == EVENT_SELF_TEST_OK)
                current_state = STATE_IDLE;
            else if (event == EVENT_SHUTDOWN)
                current_state = STATE_OFF;

            break;

        case STATE_IDLE:

            if (event == EVENT_ENGINE_START)
                current_state = STATE_RUNNING;
            else if (event == EVENT_CRITICAL_FAULT)
                current_state = STATE_FAULT;
            else if (event == EVENT_SHUTDOWN)
                current_state = STATE_OFF;

            break;

        case STATE_RUNNING:

            if (event == EVENT_CRITICAL_FAULT)
                current_state = STATE_FAULT;
            else if (event == EVENT_SHUTDOWN)
                current_state = STATE_OFF;

            break;

        case STATE_FAULT:

            if (event == EVENT_FAULT_CLEARED)
                current_state = STATE_IGNITION;
            else if (event == EVENT_SHUTDOWN)
                current_state = STATE_OFF;

            break;

        default:
            current_state = STATE_FAULT;
            break;
    }

    return current_state;
}

static const char *StateName(VehicleState_t state)
{
    switch (state)
    {
        case STATE_OFF:     return "OFF";
        case STATE_IGNITION:return "IGNITION";
        case STATE_IDLE:    return "IDLE";
        case STATE_RUNNING: return "RUNNING";
        case STATE_FAULT:   return "FAULT";
        default:            return "UNKNOWN";
    }
}

static void Test(
    VehicleState_t start,
    VehicleEvent_t event,
    VehicleState_t expected)
{
    VehicleState_Init();

    /*
     * Reach the required starting state.
     */
    if (start == STATE_IGNITION)
    {
        VehicleState_ProcessEvent(EVENT_IGNITION_ON);
    }
    else if (start == STATE_IDLE)
    {
        VehicleState_ProcessEvent(EVENT_IGNITION_ON);
        VehicleState_ProcessEvent(EVENT_SELF_TEST_OK);
    }
    else if (start == STATE_RUNNING)
    {
        VehicleState_ProcessEvent(EVENT_IGNITION_ON);
        VehicleState_ProcessEvent(EVENT_SELF_TEST_OK);
        VehicleState_ProcessEvent(EVENT_ENGINE_START);
    }
    else if (start == STATE_FAULT)
    {
        VehicleState_ProcessEvent(EVENT_IGNITION_ON);
        VehicleState_ProcessEvent(EVENT_SELF_TEST_OK);
        VehicleState_ProcessEvent(EVENT_CRITICAL_FAULT);
    }

    VehicleState_t result =
        VehicleState_ProcessEvent(event);

    if (result == expected)
    {
        printf(
            "[PASS] %s -> %s\n",
            StateName(start),
            StateName(expected)
        );
    }
    else
    {
        printf(
            "[FAIL] %s -> Expected %s, Got %s\n",
            StateName(start),
            StateName(expected),
            StateName(result)
        );
    }
}

int main(void)
{
    printf("========================================\n");
    printf(" Automotive Dashboard ECU\n");
    printf(" Vehicle State Machine Test\n");
    printf(" STM32C031C6\n");
    printf("========================================\n");
    printf("Testing all 30 state/event combinations\n\n");

    /* OFF */
    Test(STATE_OFF, EVENT_IGNITION_ON, STATE_IGNITION);
    Test(STATE_OFF, EVENT_SELF_TEST_OK, STATE_OFF);
    Test(STATE_OFF, EVENT_ENGINE_START, STATE_OFF);
    Test(STATE_OFF, EVENT_CRITICAL_FAULT, STATE_OFF);
    Test(STATE_OFF, EVENT_FAULT_CLEARED, STATE_OFF);
    Test(STATE_OFF, EVENT_SHUTDOWN, STATE_OFF);

    /* IGNITION */
    Test(STATE_IGNITION, EVENT_IGNITION_ON, STATE_IGNITION);
    Test(STATE_IGNITION, EVENT_SELF_TEST_OK, STATE_IDLE);
    Test(STATE_IGNITION, EVENT_ENGINE_START, STATE_IGNITION);
    Test(STATE_IGNITION, EVENT_CRITICAL_FAULT, STATE_IGNITION);
    Test(STATE_IGNITION, EVENT_FAULT_CLEARED, STATE_IGNITION);
    Test(STATE_IGNITION, EVENT_SHUTDOWN, STATE_OFF);

    /* IDLE */
    Test(STATE_IDLE, EVENT_IGNITION_ON, STATE_IDLE);
    Test(STATE_IDLE, EVENT_SELF_TEST_OK, STATE_IDLE);
    Test(STATE_IDLE, EVENT_ENGINE_START, STATE_RUNNING);
    Test(STATE_IDLE, EVENT_CRITICAL_FAULT, STATE_FAULT);
    Test(STATE_IDLE, EVENT_FAULT_CLEARED, STATE_IDLE);
    Test(STATE_IDLE, EVENT_SHUTDOWN, STATE_OFF);

    /* RUNNING */
    Test(STATE_RUNNING, EVENT_IGNITION_ON, STATE_RUNNING);
    Test(STATE_RUNNING, EVENT_SELF_TEST_OK, STATE_RUNNING);
    Test(STATE_RUNNING, EVENT_ENGINE_START, STATE_RUNNING);
    Test(STATE_RUNNING, EVENT_CRITICAL_FAULT, STATE_FAULT);
    Test(STATE_RUNNING, EVENT_FAULT_CLEARED, STATE_RUNNING);
    Test(STATE_RUNNING, EVENT_SHUTDOWN, STATE_OFF);

    /* FAULT */
    Test(STATE_FAULT, EVENT_IGNITION_ON, STATE_FAULT);
    Test(STATE_FAULT, EVENT_SELF_TEST_OK, STATE_FAULT);
    Test(STATE_FAULT, EVENT_ENGINE_START, STATE_FAULT);
    Test(STATE_FAULT, EVENT_CRITICAL_FAULT, STATE_FAULT);
    Test(STATE_FAULT, EVENT_FAULT_CLEARED, STATE_IGNITION);
    Test(STATE_FAULT, EVENT_SHUTDOWN, STATE_OFF);

    printf("\n========================================\n");
    printf("RESULT: ALL 30 TESTS COMPLETED\n");
    printf("========================================\n");

    return 0;
}
