#include <stdio.h>
#include <stdint.h>
#include <stdbool.h>
#include <string.h>

/* ============================================================
 * VEHICLE STATE
 * ============================================================ */

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
    EVENT_SHUTDOWN,
    EVENT_FAULT
} VehicleEvent_t;

static VehicleState_t vehicle_state = STATE_OFF;

static void VehicleState_Init(void)
{
    vehicle_state = STATE_OFF;
}

static void VehicleState_ProcessEvent(
    VehicleEvent_t event)
{
    switch (vehicle_state)
    {
        case STATE_OFF:
            if (event == EVENT_IGNITION_ON)
                vehicle_state = STATE_IGNITION;
            break;

        case STATE_IGNITION:
            if (event == EVENT_SELF_TEST_OK)
                vehicle_state = STATE_IDLE;
            else if (event == EVENT_FAULT)
                vehicle_state = STATE_FAULT;
            break;

        case STATE_IDLE:
            if (event == EVENT_ENGINE_START)
                vehicle_state = STATE_RUNNING;
            else if (event == EVENT_FAULT)
                vehicle_state = STATE_FAULT;
            break;

        case STATE_RUNNING:
            if (event == EVENT_SHUTDOWN)
                vehicle_state = STATE_OFF;
            else if (event == EVENT_FAULT)
                vehicle_state = STATE_FAULT;
            break;

        case STATE_FAULT:
            if (event == EVENT_SHUTDOWN)
                vehicle_state = STATE_OFF;
            break;

        default:
            vehicle_state = STATE_FAULT;
            break;
    }
}

static VehicleState_t VehicleState_GetCurrent(void)
{
    return vehicle_state;
}

/* ============================================================
 * ADC SENSOR
 * ============================================================ */

#define ADC_SENSOR_MAX_VALUE      4095U
#define ADC_SENSOR_REFERENCE_VOLT 3.3f

static void ADC_Sensor_Init(void)
{
}

static float ADC_Sensor_GetVoltage(
    uint16_t raw_value)
{
    if (raw_value > ADC_SENSOR_MAX_VALUE)
        raw_value = ADC_SENSOR_MAX_VALUE;

    return ((float)raw_value *
            ADC_SENSOR_REFERENCE_VOLT) /
           (float)ADC_SENSOR_MAX_VALUE;
}

static float ADC_Sensor_GetFuelPercentage(
    uint16_t raw_value)
{
    if (raw_value > ADC_SENSOR_MAX_VALUE)
        raw_value = ADC_SENSOR_MAX_VALUE;

    return ((float)raw_value * 100.0f) /
           (float)ADC_SENSOR_MAX_VALUE;
}

/* ============================================================
 * CAN DRIVER
 * ============================================================ */

#define CAN_MAX_DATA_LENGTH 8U
#define CAN_STANDARD_ID_MAX 0x7FFU

typedef struct
{
    uint16_t id;
    uint8_t length;
    uint8_t data[CAN_MAX_DATA_LENGTH];
} CAN_Message_t;

static bool can_initialized = false;

static void CAN_Driver_Init(void)
{
    can_initialized = true;
}

static bool CAN_SendMessage(
    const CAN_Message_t *message)
{
    if (!can_initialized)
        return false;

    if (message == NULL)
        return false;

    if (message->length > CAN_MAX_DATA_LENGTH)
        return false;

    if (message->id > CAN_STANDARD_ID_MAX)
        return false;

    return true;
}

/* ============================================================
 * EEPROM
 * ============================================================ */

#define EEPROM_STORAGE_SIZE 256U

static uint8_t eeprom_memory[
    EEPROM_STORAGE_SIZE];

static bool eeprom_initialized = false;

static void EEPROM_Storage_Init(void)
{
    memset(
        eeprom_memory,
        0xFF,
        sizeof(eeprom_memory)
    );

    eeprom_initialized = true;
}

static bool EEPROM_WriteByte(
    uint16_t address,
    uint8_t data)
{
    if (!eeprom_initialized)
        return false;

    if (address >= EEPROM_STORAGE_SIZE)
        return false;

    eeprom_memory[address] = data;

    return true;
}

/* ============================================================
 * TASK SCHEDULER
 * ============================================================ */

#define MAX_TASKS 4U

typedef void (*TaskCallback_t)(void);

typedef struct
{
    uint32_t interval_ms;
    uint32_t last_run_ms;
    TaskCallback_t callback;
} Task_t;

static Task_t task_list[MAX_TASKS];
static uint8_t task_count = 0U;

static void Task_Scheduler_Init(void)
{
    task_count = 0U;

    for (uint8_t i = 0U;
         i < MAX_TASKS;
         i++)
    {
        task_list[i].interval_ms = 0U;
        task_list[i].last_run_ms = 0U;
        task_list[i].callback = NULL;
    }
}

static void Task_Register(
    uint8_t task_id,
    uint32_t interval_ms,
    TaskCallback_t callback)
{
    if (task_id >= MAX_TASKS ||
        callback == NULL)
        return;

    task_list[task_id].interval_ms =
        interval_ms;

    task_list[task_id].last_run_ms = 0U;

    task_list[task_id].callback =
        callback;

    if (task_id >= task_count)
        task_count = task_id + 1U;
}

static void Task_Scheduler_Run(
    uint32_t current_time_ms)
{
    for (uint8_t i = 0U;
         i < task_count;
         i++)
    {
        if (task_list[i].callback != NULL)
        {
            if ((current_time_ms -
                 task_list[i].last_run_ms)
                >= task_list[i].interval_ms)
            {
                task_list[i].last_run_ms =
                    current_time_ms;

                task_list[i].callback();
            }
        }
    }
}

/* ============================================================
 * UART DEBUG
 * ============================================================ */

typedef enum
{
    LOG_LEVEL_INFO = 0,
    LOG_LEVEL_WARN,
    LOG_LEVEL_ERROR
} LogLevel_t;

static bool uart_initialized = false;

static void UART_Debug_Init(void)
{
    uart_initialized = true;
}

static void UART_Log(
    LogLevel_t level,
    const char *tag,
    const char *message)
{
    if (!uart_initialized ||
        tag == NULL ||
        message == NULL)
        return;

    switch (level)
    {
        case LOG_LEVEL_INFO:
            printf("[INFO] [%s]: %s\n",
                   tag,
                   message);
            break;

        case LOG_LEVEL_WARN:
            printf("[WARN] [%s]: %s\n",
                   tag,
                   message);
            break;

        case LOG_LEVEL_ERROR:
            printf("[ERROR] [%s]: %s\n",
                   tag,
                   message);
            break;

        default:
            break;
    }
}

/* ============================================================
 * ECU RUNTIME
 * ============================================================ */

static uint16_t simulated_adc = 2048U;

static float fuel_percentage = 0.0f;

static uint32_t sensor_task_runs = 0U;
static uint32_t communication_task_runs = 0U;

static void Sensor_Task(void)
{
    float voltage;

    sensor_task_runs++;

    voltage =
        ADC_Sensor_GetVoltage(
            simulated_adc
        );

    fuel_percentage =
        ADC_Sensor_GetFuelPercentage(
            simulated_adc
        );

    printf(
        "[SENSOR] ADC=%u | Voltage=%.3f V | Fuel=%.1f %%\n",
        simulated_adc,
        voltage,
        fuel_percentage
    );
}

static void Communication_Task(void)
{
    CAN_Message_t message;

    uint16_t fuel_x10;

    communication_task_runs++;

    memset(
        &message,
        0,
        sizeof(message)
    );

    message.id = 0x201U;
    message.length = 2U;

    fuel_x10 =
        (uint16_t)(
            fuel_percentage * 10.0f
        );

    message.data[0] =
        (uint8_t)(
            (fuel_x10 >> 8) & 0xFFU
        );

    message.data[1] =
        (uint8_t)(
            fuel_x10 & 0xFFU
        );

    if (CAN_SendMessage(&message))
    {
        UART_Log(
            LOG_LEVEL_INFO,
            "CAN",
            "Fuel status transmitted"
        );
    }
    else
    {
        UART_Log(
            LOG_LEVEL_ERROR,
            "CAN",
            "Fuel transmission failed"
        );
    }
}

/* ============================================================
 * MAIN INTEGRATION TEST
 * ============================================================ */

int main(void)
{
    uint8_t stored_fuel;

    printf("\n");
    printf("========================================\n");
    printf(" Automotive Dashboard ECU\n");
    printf(" ECU Integration Test\n");
    printf(" Standalone GDB Simulation\n");
    printf("========================================\n\n");

    /* Initialization */

    UART_Debug_Init();
    VehicleState_Init();
    ADC_Sensor_Init();
    CAN_Driver_Init();
    EEPROM_Storage_Init();
    Task_Scheduler_Init();

    UART_Log(
        LOG_LEVEL_INFO,
        "ECU",
        "All modules initialized"
    );

    /* Vehicle startup */

    VehicleState_ProcessEvent(
        EVENT_IGNITION_ON
    );

    VehicleState_ProcessEvent(
        EVENT_SELF_TEST_OK
    );

    VehicleState_ProcessEvent(
        EVENT_ENGINE_START
    );

    printf(
        "[STATE] Current state: %d\n",
        VehicleState_GetCurrent()
    );

    /* Register periodic tasks */

    Task_Register(
        0U,
        100U,
        Sensor_Task
    );

    Task_Register(
        1U,
        250U,
        Communication_Task
    );

    UART_Log(
        LOG_LEVEL_INFO,
        "SCHED",
        "Periodic tasks registered"
    );

    /* Simulated ECU runtime */

    for (uint32_t time_ms = 0U;
         time_ms <= 1000U;
         time_ms += 50U)
    {
        Task_Scheduler_Run(
            time_ms
        );
    }

    /* Store final fuel value */

    stored_fuel =
        (uint8_t)fuel_percentage;

    if (EEPROM_WriteByte(
            0x10U,
            stored_fuel))
    {
        UART_Log(
            LOG_LEVEL_INFO,
            "EEPROM",
            "Fuel value stored"
        );
    }

    printf("\n");
    printf("----------------------------------------\n");

    printf(
        "Sensor task executions: %lu\n",
        sensor_task_runs
    );

    printf(
        "Communication executions: %lu\n",
        communication_task_runs
    );

    printf(
        "Final fuel level: %.1f %%\n",
        fuel_percentage
    );

    printf(
        "Final vehicle state: %d\n",
        VehicleState_GetCurrent()
    );

    printf(
        "Stored fuel value: %u %%\n",
        stored_fuel
    );

    printf("----------------------------------------\n");

    UART_Log(
        LOG_LEVEL_INFO,
        "ECU",
        "Integration test complete"
    );

    return 0;
}
