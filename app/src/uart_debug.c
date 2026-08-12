#include "uart_debug.h"
#include <stdio.h>

void UART_Debug_Init(void)
{
    /*
     * UART hardware initialization is handled
     * by the platform-specific startup code.
     */
}

void UART_Log(
    LogLevel_t level,
    const char *tag,
    const char *message)
{
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
