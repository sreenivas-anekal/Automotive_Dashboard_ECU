#include "eeprom_storage.h"
#include <string.h>

static uint8_t eeprom_memory[EEPROM_STORAGE_SIZE];
static bool eeprom_initialized = false;

void EEPROM_Storage_Init(void)
{
    memset(eeprom_memory, 0xFF, sizeof(eeprom_memory));
    eeprom_initialized = true;
}

bool EEPROM_WriteByte(uint16_t address, uint8_t data)
{
    if (!eeprom_initialized)
        return false;

    if (address >= EEPROM_STORAGE_SIZE)
        return false;

    eeprom_memory[address] = data;

    return true;
}

bool EEPROM_ReadByte(uint16_t address, uint8_t *data)
{
    if (!eeprom_initialized)
        return false;

    if (data == NULL)
        return false;

    if (address >= EEPROM_STORAGE_SIZE)
        return false;

    *data = eeprom_memory[address];

    return true;
}

bool EEPROM_WriteBlock(
    uint16_t address,
    const uint8_t *data,
    uint16_t length)
{
    if (!eeprom_initialized)
        return false;

    if (data == NULL)
        return false;

    if (address >= EEPROM_STORAGE_SIZE)
        return false;

    if (length > (EEPROM_STORAGE_SIZE - address))
        return false;

    memcpy(
        &eeprom_memory[address],
        data,
        length
    );

    return true;
}

bool EEPROM_ReadBlock(
    uint16_t address,
    uint8_t *data,
    uint16_t length)
{
    if (!eeprom_initialized)
        return false;

    if (data == NULL)
        return false;

    if (address >= EEPROM_STORAGE_SIZE)
        return false;

    if (length > (EEPROM_STORAGE_SIZE - address))
        return false;

    memcpy(
        data,
        &eeprom_memory[address],
        length
    );

    return true;
}
