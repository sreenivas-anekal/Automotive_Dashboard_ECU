/**
 * @file master_ecu_integrated.ino
 * @brief Integrated Automotive Dashboard ECU Firmware (Fixed Preprocessor Order)
 */

#include <stdint.h>
#include <stdbool.h>
#include <string.h>
#include <stdio.h>

// ==========================================
// 0. DATA TYPES & ENUMS (Declared First)
// ==========================================
typedef enum {
    STATE_VEHICLE_OFF = 0,
    STATE_VEHICLE_IDLE,
    STATE_VEHICLE_READY,
    STATE_VEHICLE_DRIVING,
    STATE_VEHICLE_FAULT
} VehicleState_t;

typedef enum {
    EVENT_IGNITION_ON = 0,
    EVENT_SELF_TEST_OK,
    EVENT_START_DRIVING,
    EVENT_STOP_DRIVING,
    EVENT_FAULT_DETECTED,
    EVENT_IGNITION_OFF
} VehicleEvent_t;

typedef enum {
    LOG_LEVEL_INFO = 0,
    LOG_LEVEL_WARN,
    LOG_LEVEL_ERROR
} LogLevel_t;

typedef struct {
    uint32_t id;
    uint8_t dlc;
    uint8_t data[8];
} CanMessage_t;

typedef void (*TaskCallback_t)(void);

typedef struct {
    uint32_t interval_ms;
    uint32_t last_run_ms;
    TaskCallback_t callback;
} Task_t;

// ==========================================
// 1. VEHICLE STATE MACHINE MODULE
// ==========================================
static VehicleState_t current_state = STATE_VEHICLE_OFF;

void VehicleState_Init(void) {
    current_state = STATE_VEHICLE_OFF;
}

VehicleState_t VehicleState_GetCurrent(void) {
    return current_state;
}

void VehicleState_ProcessEvent(VehicleEvent_t event) {
    switch (current_state) {
        case STATE_VEHICLE_OFF:
            if (event == EVENT_IGNITION_ON) current_state = STATE_VEHICLE_IDLE;
            break;
        case STATE_VEHICLE_IDLE:
            if (event == EVENT_SELF_TEST_OK) current_state = STATE_VEHICLE_READY;
            else if (event == EVENT_IGNITION_OFF) current_state = STATE_VEHICLE_OFF;
            break;
        case STATE_VEHICLE_READY:
            if (event == EVENT_START_DRIVING) current_state = STATE_VEHICLE_DRIVING;
            else if (event == EVENT_IGNITION_OFF) current_state = STATE_VEHICLE_OFF;
            break;
        case STATE_VEHICLE_DRIVING:
            if (event == EVENT_STOP_DRIVING) current_state = STATE_VEHICLE_READY;
            else if (event == EVENT_IGNITION_OFF) current_state = STATE_VEHICLE_OFF;
            break;
        case STATE_VEHICLE_FAULT:
            if (event == EVENT_IGNITION_OFF) current_state = STATE_VEHICLE_OFF;
            break;
        default:
            break;
    }
    if (event == EVENT_FAULT_DETECTED) {
        current_state = STATE_VEHICLE_FAULT;
    }
}

// ==========================================
// 2. UART DEBUG LOGGER MODULE
// ==========================================
void UART_Debug_Init(void) {}

void UART_Log(LogLevel_t level, const char* tag, const char* message) {
    const char* level_str = "INFO";
    if (level == LOG_LEVEL_WARN) level_str = "WARN";
    else if (level == LOG_LEVEL_ERROR) level_str = "ERROR";

    char log_buf[128];
    sprintf(log_buf, "[%s] [%s] %s\n", level_str, tag, message);
    Serial.print(log_buf);
}

// ==========================================
// 3. ADC SENSOR DRIVER MODULE
// ==========================================
#define ADC_MAX_VAL 4095.0f
#define ADC_VREF    3.3f

void ADC_Sensor_Init(void) {
    pinMode(34, INPUT);
}

uint16_t ADC_Read_Raw(uint8_t channel) {
    (void)channel;
    return (uint16_t)analogRead(34);
}

float ADC_ConvertToVoltage(uint16_t raw_value) {
    return ((float)raw_value / ADC_MAX_VAL) * ADC_VREF;
}

float ADC_GetFuelLevelPercentage(uint16_t raw_value) {
    float percentage = ((float)raw_value / ADC_MAX_VAL) * 100.0f;
    if (percentage > 100.0f) percentage = 100.0f;
    if (percentage < 0.0f) percentage = 0.0f;
    return percentage;
}

// ==========================================
// 4. CAN BUS COMMUNICATION MODULE
// ==========================================
void CAN_Driver_Init(uint32_t baudrate) {
    (void)baudrate;
}

bool CAN_Transmit(const CanMessage_t* msg) {
    if (msg == NULL) return false;
    
    char can_buf[64];
    sprintf(can_buf, "[CAN TX] ID: 0x%03X | DLC: %u | Data: ", (unsigned int)msg->id, msg->dlc);
    Serial.print(can_buf);
    for (uint8_t i = 0; i < msg->dlc && i < 8; i++) {
        char hex_byte[4];
        sprintf(hex_byte, "%02X ", msg->data[i]);
        Serial.print(hex_byte);
    }
    Serial.println();
    return true;
}

bool CAN_Receive(CanMessage_t* msg) {
    (void)msg;
    return false;
}

// ==========================================
// 5. EEPROM NON-VOLATILE STORAGE MODULE
// ==========================================
#define EEPROM_SIM_SIZE 512
#define ODOMETER_ADDR   0x00

static uint8_t simulated_eeprom[EEPROM_SIM_SIZE] = {0};

void EEPROM_Storage_Init(void) {
    uint32_t default_km = 12345;
    uint32_t stored_km;
    memcpy(&stored_km, &simulated_eeprom[ODOMETER_ADDR], sizeof(uint32_t));
    if (stored_km == 0xFFFFFFFF || stored_km == 0x00000000) {
        memcpy(&simulated_eeprom[ODOMETER_ADDR], &default_km, sizeof(uint32_t));
    }
}

void EEPROM_WriteData(uint16_t address, const uint8_t* data, uint16_t length) {
    if (address + length <= EEPROM_SIM_SIZE && data != NULL) {
        memcpy(&simulated_eeprom[address], data, length);
    }
}

void EEPROM_ReadData(uint16_t address, uint8_t* buffer, uint16_t length) {
    if (address + length <= EEPROM_SIM_SIZE && buffer != NULL) {
        memcpy(buffer, &simulated_eeprom[address], length);
    }
}

uint32_t EEPROM_GetOdometer(void) {
    uint32_t mileage = 0;
    EEPROM_ReadData(ODOMETER_ADDR, (uint8_t*)&mileage, sizeof(uint32_t));
    return mileage;
}

void EEPROM_SaveOdometer(uint32_t mileage) {
    EEPROM_WriteData(ODOMETER_ADDR, (const uint8_t*)&mileage, sizeof(uint32_t));
}

// ==========================================
// 6. TASK SCHEDULER MODULE
// ==========================================
#define MAX_TASKS 4

static Task_t task_list[MAX_TASKS];
static uint8_t task_count = 0;

void Task_Scheduler_Init(void) {
    task_count = 0;
    for (uint8_t i = 0; i < MAX_TASKS; i++) {
        task_list[i].interval_ms = 0;
        task_list[i].last_run_ms = 0;
        task_list[i].callback = NULL;
    }
}

void Task_Register(uint8_t task_id, uint32_t interval_ms, TaskCallback_t callback) {
    if (task_id < MAX_TASKS && callback != NULL) {
        task_list[task_id].interval_ms = interval_ms;
        task_list[task_id].last_run_ms = 0;
        task_list[task_id].callback = callback;
        if (task_id >= task_count) {
            task_count = task_id + 1;
        }
    }
}

void Task_Scheduler_Run(uint32_t current_time_ms) {
    for (uint8_t i = 0; i < task_count; i++) {
        if (task_list[i].callback != NULL) {
            if ((current_time_ms - task_list[i].last_run_ms) >= task_list[i].interval_ms) {
                task_list[i].last_run_ms = current_time_ms;
                task_list[i].callback();
            }
        }
    }
}

// ==========================================
// MASTER APPLICATION TASKS & ENTRY POINTS
// ==========================================
void ECU_SensorTask(void) {
    uint16_t raw_adc = ADC_Read_Raw(0);
    float fuel_pct = ADC_GetFuelLevelPercentage(raw_adc);

    char log_buf[64];
    sprintf(log_buf, "Fuel Level: %.1f%% (Raw: %u)", fuel_pct, raw_adc);
    UART_Log(LOG_LEVEL_INFO, "SENS", log_buf);
}

void ECU_TelemetryTask(void) {
    uint16_t raw_adc = ADC_Read_Raw(0);
    float fuel_pct = ADC_GetFuelLevelPercentage(raw_adc);
    uint32_t odometer = EEPROM_GetOdometer();

    CanMessage_t msg;
    msg.id = 0x101;
    msg.dlc = 4;
    msg.data[0] = (uint8_t)VehicleState_GetCurrent();
    msg.data[1] = (uint8_t)fuel_pct;
    msg.data[2] = (uint8_t)(odometer & 0xFF);
    msg.data[3] = (uint8_t)((odometer >> 8) & 0xFF);

    CAN_Transmit(&msg);
}

void ECU_HeartbeatTask(void) {
    uint32_t current_odo = EEPROM_GetOdometer();
    EEPROM_SaveOdometer(current_odo + 1);

    char log_buf[64];
    sprintf(log_buf, "ECU Heartbeat OK. Current Odometer: %u km", (unsigned int)EEPROM_GetOdometer());
    UART_Log(LOG_LEVEL_INFO, "ECU", log_buf);
}

void setup() {
    Serial.begin(115200);
    delay(1000);

    UART_Debug_Init();
    VehicleState_Init();
    ADC_Sensor_Init();
    CAN_Driver_Init(500000);
    EEPROM_Storage_Init();
    Task_Scheduler_Init();

    UART_Log(LOG_LEVEL_INFO, "INIT", "=== Automotive Dashboard ECU Booting Up ===");

    VehicleState_ProcessEvent(EVENT_IGNITION_ON);
    VehicleState_ProcessEvent(EVENT_SELF_TEST_OK);

    Task_Register(0, 500,  ECU_SensorTask);
    Task_Register(1, 1000, ECU_TelemetryTask);
    Task_Register(2, 3000, ECU_HeartbeatTask);

    UART_Log(LOG_LEVEL_INFO, "INIT", "=== ECU Initialization Complete ===");
}

void loop() {
    Task_Scheduler_Run(millis());
}
