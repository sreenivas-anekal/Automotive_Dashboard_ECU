#include "vehicle_state.h"
#include "uart_debug.h"

void setup() {
    Serial.begin(115200);
    delay(1000);
    
    VehicleState_Init();
    Serial.println("=== Module 1: Vehicle State Machine Test ===");

    char buf[64];
    sprintf(buf, "Initial State: %d (Expected: 0 -> OFF)", VehicleState_GetCurrent());
    Serial.println(buf);

    VehicleState_ProcessEvent(EVENT_IGNITION_ON);
    sprintf(buf, "After IGNITION_ON: %d (Expected: 1)", VehicleState_GetCurrent());
    Serial.println(buf);

    VehicleState_ProcessEvent(EVENT_SELF_TEST_OK);
    sprintf(buf, "After SELF_TEST_OK: %d (Expected: 2)", VehicleState_GetCurrent());
    Serial.println(buf);
}

void loop() {}
