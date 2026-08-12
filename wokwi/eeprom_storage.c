#include <stdio.h>
#include <stdint.h>
#include <stdbool.h>
#include <string.h>

#define EEPROM_STORAGE_SIZE 256U

static uint8_t eeprom_memory[EEPROM_STORAGE_SIZE];
static bool eeprom_initialized = false;

void EEPROM_Storage_Init(void)
{
    memset(eeprom_memory, 0xFF, sizeof(eeprom_memory));
    eeprom_initialized = true;
}

bool EEPROM_WriteByte(uint16_t address, uint8_t data)
{
    if (!eeprom_initialized ||
        address >= EEPROM_STORAGE_SIZE)
        return false;

    eeprom_memory[address] = data;

    return true;
}

bool EEPROM_ReadByte(uint16_t address, uint8_t *data)
{
    if (!eeprom_initialized ||
        data == NULL ||
        address >= EEPROM_STORAGE_SIZE)
        return false;

    *data = eeprom_memory[address];

    return true;
}

bool EEPROM_WriteBlock(
    uint16_t address,
    const uint8_t *data,
    uint16_t length)
{
    if (!eeprom_initialized ||
        data == NULL ||
        address >= EEPROM_STORAGE_SIZE)
        return false;

    if (length > EEPROM_STORAGE_SIZE - address)
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
    if (!eeprom_initialized ||
        data == NULL ||
        address >= EEPROM_STORAGE_SIZE)
        return false;

    if (length > EEPROM_STORAGE_SIZE - address)
        return false;

    memcpy(
        data,
        &eeprom_memory[address],
        length
    );

    return true;
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
    uint8_t value = 0U;

    uint8_t write_block[] =
    {
        0x11, 0x22, 0x33, 0x44, 0x55
    };

    uint8_t read_block[5] = {0};

    printf("========================================\n");
    printf(" Automotive Dashboard ECU\n");
    printf(" EEPROM Storage Unit Test\n");
    printf(" STM32C031C6\n");
    printf(" Simulated EEPROM\n");
    printf("========================================\n\n");

    EEPROM_Storage_Init();

    printf("EEPROM INIT DONE\n");

    check(
        "EEPROM write byte",
        EEPROM_WriteByte(0x10U, 0x5AU)
    );

    check(
        "EEPROM read byte",
        EEPROM_ReadByte(0x10U, &value)
    );

    check(
        "EEPROM byte value preserved",
        value == 0x5AU
    );

    check(
        "EEPROM write block",
        EEPROM_WriteBlock(
            0x20U,
            write_block,
            5U
        )
    );

    check(
        "EEPROM read block",
        EEPROM_ReadBlock(
            0x20U,
            read_block,
            5U
        )
    );

    check(
        "EEPROM block data preserved",
        memcmp(
            write_block,
            read_block,
            5U
        ) == 0
    );

    check(
        "Invalid address rejected",
        !EEPROM_WriteByte(
            EEPROM_STORAGE_SIZE,
            0xAAU
        )
    );

    check(
        "Invalid block rejected",
        !EEPROM_WriteBlock(
            250U,
            write_block,
            10U
        )
    );

    printf("\n----------------------------------------\n");

    printf(
        "EEPROM READ BYTE: 0x%02X\n",
        value
    );

    printf("EEPROM READ BLOCK:");

    for (uint8_t i = 0U; i < 5U; i++)
    {
        printf(" %02X", read_block[i]);
    }

    printf("\n");
    printf("----------------------------------------\n");

    printf("Tests passed: %d\n", tests_passed);
    printf("Tests failed: %d\n", tests_failed);

    if (tests_failed == 0)
    {
        printf("RESULT: ALL TESTS PASSED\n");
        return 0;
    }

    printf("RESULT: TESTS FAILED\n");
    return 1;
}
