#include <stdio.h>
#include <stdint.h>
#include <stdbool.h>
#include <string.h>

#define CAN_MAX_DATA_LENGTH    8U
#define CAN_STANDARD_ID_MAX    0x7FFU

typedef struct
{
    uint16_t id;
    uint8_t length;
    uint8_t data[CAN_MAX_DATA_LENGTH];
} CAN_Message_t;

static bool can_initialized = false;

static CAN_Message_t tx_message;
static CAN_Message_t rx_message;

void CAN_Driver_Init(void)
{
    can_initialized = true;

    memset(&tx_message, 0, sizeof(tx_message));
    memset(&rx_message, 0, sizeof(rx_message));
}

bool CAN_SendMessage(const CAN_Message_t *message)
{
    if (!can_initialized)
    {
        return false;
    }

    if (message == NULL)
    {
        return false;
    }

    if (message->length > CAN_MAX_DATA_LENGTH)
    {
        return false;
    }

    if (message->id > CAN_STANDARD_ID_MAX)
    {
        return false;
    }

    memcpy(
        &tx_message,
        message,
        sizeof(CAN_Message_t)
    );

    memcpy(
        &rx_message,
        &tx_message,
        sizeof(CAN_Message_t)
    );

    return true;
}

bool CAN_ReceiveMessage(CAN_Message_t *message)
{
    if (!can_initialized)
    {
        return false;
    }

    if (message == NULL)
    {
        return false;
    }

    memcpy(
        message,
        &rx_message,
        sizeof(CAN_Message_t)
    );

    return true;
}

static int tests_passed = 0;
static int tests_failed = 0;

static void check(
    const char *test_name,
    bool condition)
{
    if (condition)
    {
        printf("[PASS] %s\n", test_name);
        tests_passed++;
    }
    else
    {
        printf("[FAIL] %s\n", test_name);
        tests_failed++;
    }
}

int main(void)
{
    CAN_Message_t tx;
    CAN_Message_t rx;

    printf("========================================\n");
    printf(" Automotive Dashboard ECU\n");
    printf(" CAN Driver Unit Test\n");
    printf(" STM32C031C6\n");
    printf(" Simulated CAN Transport\n");
    printf("========================================\n\n");

    CAN_Driver_Init();

    printf("CAN DRIVER INIT DONE\n");

    memset(&tx, 0, sizeof(tx));
    memset(&rx, 0, sizeof(rx));

    tx.id = 0x123U;
    tx.length = 8U;

    tx.data[0] = 0x10U;
    tx.data[1] = 0x20U;
    tx.data[2] = 0x30U;
    tx.data[3] = 0x40U;
    tx.data[4] = 0x50U;
    tx.data[5] = 0x60U;
    tx.data[6] = 0x70U;
    tx.data[7] = 0x80U;

    check(
        "CAN transmit succeeds",
        CAN_SendMessage(&tx)
    );

    check(
        "CAN receive succeeds",
        CAN_ReceiveMessage(&rx)
    );

    check(
        "CAN ID preserved",
        rx.id == 0x123U
    );

    check(
        "CAN DLC preserved",
        rx.length == 8U
    );

    check(
        "CAN payload preserved",
        memcmp(
            tx.data,
            rx.data,
            CAN_MAX_DATA_LENGTH
        ) == 0
    );

    {
        CAN_Message_t invalid_message;

        memset(
            &invalid_message,
            0,
            sizeof(invalid_message)
        );

        invalid_message.id = 0x800U;
        invalid_message.length = 8U;

        check(
            "Invalid standard CAN ID rejected",
            !CAN_SendMessage(&invalid_message)
        );
    }

    {
        CAN_Message_t invalid_message;

        memset(
            &invalid_message,
            0,
            sizeof(invalid_message)
        );

        invalid_message.id = 0x123U;
        invalid_message.length = 9U;

        check(
            "Invalid DLC rejected",
            !CAN_SendMessage(&invalid_message)
        );
    }

    printf("\n----------------------------------------\n");

    printf(
        "CAN TX: SUCCESS\n"
    );

    printf(
        "ID: 0x%03X DLC: %u\n",
        rx.id,
        rx.length
    );

    printf("DATA:");

    for (uint8_t i = 0U; i < rx.length; i++)
    {
        printf(
            " %02X",
            rx.data[i]
        );
    }

    printf("\n");

    printf("----------------------------------------\n");
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
