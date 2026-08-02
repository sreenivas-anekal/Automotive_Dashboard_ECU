#ifndef CAN_DRIVER_H
#define CAN_DRIVER_H

#include <stdint.h>
#include <stdbool.h>

#ifdef __cplusplus
extern "C" {
#endif

/* Standard CAN Frame structure */
typedef struct {
    uint32_t id;          /* CAN Identifier (e.g., 0x100 for Vehicle Status) */
    uint8_t dlc;          /* Data Length Code (number of bytes, 0 to 8) */
    uint8_t data[8];      /* Payload bytes */
} CanMessage_t;

void CAN_Driver_Init(uint32_t baudrate);
bool CAN_Transmit(const CanMessage_t* msg);
bool CAN_Receive(CanMessage_t* msg);

#ifdef __cplusplus
}
#endif

#endif /* CAN_DRIVER_H */
