#include "can_driver.h"

void setup() {
    Serial.begin(115200);
    delay(1000);

    CAN_Driver_Init(500000); // 500 kbps CAN bus
    Serial.println("=== Module 4: CAN Bus Driver Test ===");

    // Construct a sample CAN message broadcasting vehicle state and fuel level
    CanMessage_t ecu_msg;
    ecu_msg.id = 0x101;     // ECU Dashboard Status ID
    ecu_msg.dlc = 3;        // 3 bytes of data
    ecu_msg.data[0] = 0x02; // State: IDLE
    ecu_msg.data[1] = 0x50; // Fuel Level: 80% (0x50 hex = 80 dec)
    ecu_msg.data[2] = 0x00; // No faults

    // Transmit message
    CAN_Transmit(&ecu_msg);
    Serial.println("=== Module 4 Test Complete Successfully! ===");
}

void loop() {
    // Periodically broadcast a heartbeat CAN frame every 2 seconds
    CanMessage_t heartbeat;
    heartbeat.id = 0x7FF;
    heartbeat.dlc = 2;
    heartbeat.data[0] = 0xAA;
    heartbeat.data[1] = 0x55;

    CAN_Transmit(&heartbeat);
    delay(2000);
}
