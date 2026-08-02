#ifndef EEPROM_STORAGE_H
#define EEPROM_STORAGE_H

#include <stdint.h>
#include <stdbool.h>

#ifdef __cplusplus
extern "C" {
#endif

void EEPROM_Storage_Init(void);
void EEPROM_WriteData(uint16_t address, const uint8_t* data, uint16_t length);
void EEPROM_ReadData(uint16_t address, uint8_t* buffer, uint16_t length);
uint32_t EEPROM_GetOdometer(void);
void EEPROM_SaveOdometer(uint32_t mileage);

#ifdef __cplusplus
}
#endif

#endif /* EEPROM_STORAGE_H */
