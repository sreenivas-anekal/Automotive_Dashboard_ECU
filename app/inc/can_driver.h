#ifndef CAN_DRIVER_H
#define CAN_DRIVER_H

#include <stdint.h>
#include <stdbool.h>

#define CAN_MAX_DATA_LENGTH 8U

typedef struct
{
    uint32_t id;
    uint8_t data[CAN_MAX_DATA_LENGTH];
    uint8_t length;
} CAN_Message_t;

void CAN_Driver_Init(void);

bool CAN_SendMessage(const CAN_Message_t *message);

bool CAN_ReceiveMessage(CAN_Message_t *message);

#endif
