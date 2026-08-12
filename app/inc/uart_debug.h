#ifndef UART_DEBUG_H
#define UART_DEBUG_H

#include <stdint.h>

#ifdef __cplusplus
extern "C" {
#endif

typedef enum
{
    LOG_LEVEL_INFO = 0,
    LOG_LEVEL_WARN,
    LOG_LEVEL_ERROR
} LogLevel_t;

void UART_Debug_Init(void);

void UART_Log(
    LogLevel_t level,
    const char *tag,
    const char *message
);

#ifdef __cplusplus
}
#endif

#endif /* UART_DEBUG_H */
