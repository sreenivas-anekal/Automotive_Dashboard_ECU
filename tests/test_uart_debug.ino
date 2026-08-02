#include "uart_debug.h"

void setup() {
    Serial.begin(115200);
    delay(1000);

    UART_Debug_Init();
    Serial.println("=== Module 2: UART Debug Logger Test ===");

    UART_Log(LOG_LEVEL_INFO, "MAIN", "This is an info log message.");
    UART_Log(LOG_LEVEL_WARN, "SENS", "This is a warning log message.");
    UART_Log(LOG_LEVEL_ERROR, "ECU", "This is an error log message.");
}

void loop() {}
