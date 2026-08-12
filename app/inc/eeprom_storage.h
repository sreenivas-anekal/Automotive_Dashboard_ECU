#ifndef EEPROM_STORAGE_H
#define EEPROM_STORAGE_H

#include <stdint.h>
#include <stdbool.h>

#define EEPROM_STORAGE_SIZE 256U

#ifdef __cplusplus
extern "C" {
#endif

void EEPROM_Storage_Init(void);

bool EEPROM_WriteByte(
    uint16_t address,
    uint8_t data
);

bool EEPROM_ReadByte(
    uint16_t address,
    uint8_t *data
);

bool EEPROM_WriteBlock(
    uint16_t address,
    const uint8_t *data,
    uint16_t length
);

bool EEPROM_ReadBlock(
    uint16_t address,
    uint8_t *data,
    uint16_t length
);

#ifdef __cplusplus
}
#endif

#endif /* EEPROM_STORAGE_H */
