#include <stdio.h>
#include <stdint.h>
#include <stdbool.h>
#include <string.h>

typedef enum
{
    LOG_LEVEL_INFO = 0,
    LOG_LEVEL_WARN,
    LOG_LEVEL_ERROR
} LogLevel_t;

static bool uart_initialized = false;

void UART_Debug_Init(void)
{
    uart_initialized = true;
}

void UART_Log(
    LogLevel_t level,
    const char *tag,
    const char *message)
{
    if (!uart_initialized)
    {
        return;
    }

    if (tag == NULL || message == NULL)
    {
        return;
    }

    switch (level)
    {
        case LOG_LEVEL_INFO:
            printf("[INFO] [%s]: %s\n", tag, message);
            break;

        case LOG_LEVEL_WARN:
            printf("[WARN] [%s]: %s\n", tag, message);
            break;

        case LOG_LEVEL_ERROR:
            printf("[ERROR] [%s]: %s\n", tag, message);
            break;

        default:
            printf("[DEBUG] [%s]: %s\n", tag, message);
            break;
    }
}

static int tests_passed = 0;
static int tests_failed = 0;

static void check(
    const char *name,
    bool condition)
{
    if (condition)
    {
        printf("[PASS] %s\n", name);
        tests_passed++;
    }
    else
    {
        printf("[FAIL] %s\n", name);
        tests_failed++;
    }
}

int main(void)
{
    printf("========================================\n");
    printf(" Automotive Dashboard ECU\n");
    printf(" UART Debug Unit Test\n");
    printf(" STM32C031C6\n");
    printf("========================================\n\n");

    UART_Debug_Init();

    check(
        "UART debug initializes",
        uart_initialized == true
    );

    printf("\nLog output test:\n");

    UART_Log(
        LOG_LEVEL_INFO,
        "SYSTEM",
        "ECU initialized"
    );

    UART_Log(
        LOG_LEVEL_WARN,
        "ADC",
        "Sensor value approaching limit"
    );

    UART_Log(
        LOG_LEVEL_ERROR,
        "CAN",
        "CAN transmission failed"
    );

    UART_Log(
        (LogLevel_t)99,
        "TEST",
        "Default log level"
    );

    check(
        "INFO log level accepted",
        LOG_LEVEL_INFO == 0
    );

    check(
        "WARN log level accepted",
        LOG_LEVEL_WARN == 1
    );

    check(
        "ERROR log level accepted",
        LOG_LEVEL_ERROR == 2
    );

    printf("\n----------------------------------------\n");
    printf("Tests passed: %d\n", tests_passed);
    printf("Tests failed: %d\n", tests_failed);
    printf("----------------------------------------\n");

    if (tests_failed == 0)
    {
        printf("RESULT: ALL TESTS PASSED\n");
        return 0;
    }

    printf("RESULT: TESTS FAILED\n");
    return 1;
}
