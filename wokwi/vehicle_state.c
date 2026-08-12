#include "vehicle_state.h"
#include <stdio.h>

int main(void)
{
    printf("\n");
    printf("========================================\n");
    printf(" Automotive Dashboard ECU\n");
    printf(" Vehicle State Machine\n");
    printf(" STM32C031C6\n");
    printf("========================================\n");

    VehicleState_Init();

    printf("Initial state: OFF\n");

    VehicleState_ProcessEvent(EVENT_IGNITION_ON);
    printf("Event: IGNITION_ON  -> State: IGNITION\n");

    VehicleState_ProcessEvent(EVENT_SELF_TEST_OK);
    printf("Event: SELF_TEST_OK -> State: IDLE\n");

    VehicleState_ProcessEvent(EVENT_ENGINE_START);
    printf("Event: ENGINE_START  -> State: RUNNING\n");

    VehicleState_ProcessEvent(EVENT_SHUTDOWN);
    printf("Event: SHUTDOWN      -> State: OFF\n");

    printf("----------------------------------------\n");
    printf("Vehicle state test complete.\n");

    return 0;
}
