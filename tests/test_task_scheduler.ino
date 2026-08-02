#include "task_scheduler.h"

// Define separate task callbacks
void FastSensorTask(void) {
    Serial.println("[TASK] Fast Sensor Poll (Every 500ms)");
}

void SlowCanHeartbeatTask(void) {
    Serial.println("[TASK] Slow CAN Heartbeat (Every 2000ms)");
}

void setup() {
    Serial.begin(115200);
    delay(1000);

    Task_Scheduler_Init();
    Serial.println("=== Module 6: Task Scheduler Test ===");

    // Register tasks with different periodicities
    Task_Register(0, 500, FastSensorTask);       // Runs every 0.5 seconds
    Task_Register(1, 2000, SlowCanHeartbeatTask); // Runs every 2.0 seconds
}

void loop() {
    // Pass current system time (millis) to the scheduler engine non-blockingly
    Task_Scheduler_Run(millis());
}
