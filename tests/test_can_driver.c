#include <stdio.h>
#include <stdint.h>
#include <stdbool.h>

#include "../app/inc/can_driver.h"

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

static bool is_valid_message(
    const CAN_Message_t *message)
{
    if (message == NULL)
    {
        return false;
    }

    if (message->id > CAN_STANDARD_ID_MAX)
    {
        return false;
    }

    if (message->length > CAN_MAX_DATA_LENGTH)
    {
        return false;
    }

    return true;
}

int main(void)
{
    CAN_Message_t message = {0};

    printf("========================================\n");
    printf(" Automotive Dashboard ECU\n");
    printf(" CAN Message Interface Unit Test\n");
    printf("========================================\n\n");

    /* Standard 11-bit ID */

    message.id = 0x123U;
    message.length = 8U;

    check(
        "Standard 11-bit CAN ID accepted",
        is_valid_message(&message)
    );

    /* Maximum valid ID */

    message.id = CAN_STANDARD_ID_MAX;

    check(
        "Maximum CAN ID 0x7FF accepted",
        is_valid_message(&message)
    );

    /* Invalid ID */

    message.id = 0x800U;

    check(
        "CAN ID above 0x7FF rejected",
        !is_valid_message(&message)
    );

    /* Restore valid ID */

    message.id = 0x123U;

    /* Zero-byte payload */

    message.length = 0U;

    check(
        "Zero-byte payload accepted",
        is_valid_message(&message)
    );

    /* Maximum payload */

    message.length = CAN_MAX_DATA_LENGTH;

    check(
        "8-byte payload accepted",
        is_valid_message(&message)
    );

    /* Invalid payload */

    message.length = 9U;

    check(
        "Payload above 8 bytes rejected",
        !is_valid_message(&message)
    );

    /* NULL message */

    check(
        "NULL message rejected",
        !is_valid_message(NULL)
    );

    printf("\n========================================\n");
    printf("Tests passed: %d\n", tests_passed);
    printf("Tests failed: %d\n", tests_failed);
    printf("========================================\n");

    if (tests_failed == 0)
    {
        printf("RESULT: ALL TESTS PASSED\n");
        return 0;
    }

    printf("RESULT: TESTS FAILED\n");
    return 1;
}
