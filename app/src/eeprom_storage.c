#include "eeprom_storage.h"
#include <string.h>
#include <stdio.h>

#define EEPROM_SIM_SIZE 512
#define ODOMETER_ADDR   0x00

/* Simulated EEPROM storage array in RAM/Flash */
static uint8_t simulated_eeprom[EEPROM_SIM_SIZE] = {0};

void EEPROM_Storage_Init(void) {
    // Initialize or load simulated non-volatile storage
    // For simulation, let's inject a default starting odometer value (e.g., 12345 km) if empty
    uint32_t default_km = 12345;
    uint32_t stored_km;
    
    EEPROM_ReadData(ODOMETER_ADDR, (uint8_t*)&stored_km, sizeof(uint32_t));
    if (stored_km == 0xFFFFFFFF || stored_km == 0x00000000) {
        EEPROM_SaveOdometer(default_km);
    }
}

void EEPROM_WriteData(uint16_t address, const uint8_t* data, uint16_t length) {
    if (address + length <= EEPROM_SIM_SIZE && data != NULL) {
        memcpy(&simulated_eeprom[address], data, length);
        printf("[EEPROM] Wrote %u bytes to address 0x%03X\n", length, address);
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
