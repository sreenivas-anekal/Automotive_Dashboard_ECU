#include <stdio.h>
#include <stdint.h>
#include <stdbool.h>
#include <string.h>

#define CAN_MAX_DATA_LENGTH 8U
#define CAN_SOF 0xAAU
#define CAN_FRAME_SIZE 13U

typedef struct
{
    uint32_t id;
    uint8_t data[CAN_MAX_DATA_LENGTH];
    uint8_t length;
} CAN_Message_t;


/* ---------------------------------------------------------
 * Test-side frame construction
 * --------------------------------------------------------- */

static uint8_t CAN_CalculateChecksum(
    const uint8_t *data,
    uint8_t length)
{
    uint8_t checksum = 0U;

    for (uint8_t i = 0U; i < length; i++)
    {
        checksum ^= data[i];
    }

    return checksum;
}

static bool CAN_BuildFrame(
    const CAN_Message_t *message,
    uint8_t *frame)
{
    if (message == NULL || frame == NULL)
    {
        return false;
    }

    if (message->id > 0x7FFU)
    {
        return false;
    }

    if (message->length > CAN_MAX_DATA_LENGTH)
    {
        return false;
    }

    frame[0] = CAN_SOF;

    frame[1] =
        (uint8_t)((message->id >> 8) & 0xFFU);

    frame[2] =
        (uint8_t)(message->id & 0xFFU);

    frame[3] = message->length;

    memset(&frame[4], 0, 8U);

    memcpy(
        &frame[4],
        message->data,
        message->length
    );

    frame[12] =
        CAN_CalculateChecksum(&frame[1], 11U);

    return true;
}

static bool CAN_ValidateFrame(
    const uint8_t *frame)
{
    if (frame == NULL)
    {
        return false;
    }

    if (frame[0] != CAN_SOF)
    {
        return false;
    }

    if (frame[3] > CAN_MAX_DATA_LENGTH)
    {
        return false;
    }

    if (frame[12] !=
        CAN_CalculateChecksum(&frame[1], 11U))
    {
        return false;
    }

    return true;
}


/* ---------------------------------------------------------
 * Test framework
 * --------------------------------------------------------- */

static int tests_passed = 0;
static int tests_failed = 0;

static void check(
    const char *test_name,
    bool result)
{
    if (result)
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


/* ---------------------------------------------------------
 * Tests
 * --------------------------------------------------------- */

int main(void)
{
    uint8_t frame[CAN_FRAME_SIZE];

    printf("========================================\n");
    printf(" Automotive Dashboard ECU\n");
    printf(" CAN Driver Unit Test\n");
    printf("========================================\n\n");


    /* Valid standard CAN message */

    CAN_Message_t message1 =
    {
        .id = 0x123U,
        .length = 8U,
        .data =
        {
            0x10U,
            0x20U,
            0x30U,
            0x40U,
            0x50U,
            0x60U,
            0x70U,
            0x80U
        }
    };

    check(
        "Valid 0x123 CAN frame accepted",
        CAN_BuildFrame(&message1, frame)
    );

    check(
        "Valid CAN frame passes validation",
        CAN_ValidateFrame(frame)
    );


    /* Maximum valid 11-bit identifier */

    CAN_Message_t message2 =
    {
        .id = 0x7FFU,
        .length = 1U,
        .data = {0xAAU}
    };

    check(
        "Maximum 11-bit CAN ID 0x7FF accepted",
        CAN_BuildFrame(&message2, frame)
    );


    /* Invalid CAN identifier */

    CAN_Message_t message3 =
    {
        .id = 0x800U,
        .length = 1U,
        .data = {0xAAU}
    };

    check(
        "CAN ID 0x800 rejected",
        !CAN_BuildFrame(&message3, frame)
    );


    /* Zero-byte payload */

    CAN_Message_t message4 =
    {
        .id = 0x100U,
        .length = 0U,
        .data = {0}
    };

    check(
        "Zero-byte CAN payload accepted",
        CAN_BuildFrame(&message4, frame)
    );


    /* Maximum 8-byte payload */

    CAN_Message_t message5 =
    {
        .id = 0x200U,
        .length = 8U,
        .data =
        {
            1U, 2U, 3U, 4U,
            5U, 6U, 7U, 8U
        }
    };

    check(
        "Maximum 8-byte CAN payload accepted",
        CAN_BuildFrame(&message5, frame)
    );


    /* Invalid payload length */

    CAN_Message_t message6 =
    {
        .id = 0x300U,
        .length = 9U,
        .data =
        {
            1U, 2U, 3U, 4U,
            5U, 6U, 7U, 8U
        }
    };

    check(
        "9-byte CAN payload rejected",
        !CAN_BuildFrame(&message6, frame)
    );


    /* NULL message */

    check(
        "NULL message rejected",
        !CAN_BuildFrame(NULL, frame)
    );


    /* NULL frame buffer */

    check(
        "NULL frame buffer rejected",
        !CAN_BuildFrame(&message1, NULL)
    );


    /* Corrupted frame */

    CAN_BuildFrame(&message1, frame);

    frame[5] ^= 0xFFU;

    check(
        "Corrupted CAN frame rejected",
        !CAN_ValidateFrame(frame)
    );


    /* Invalid start byte */

    CAN_BuildFrame(&message1, frame);

    frame[0] = 0x55U;

    check(
        "Invalid start-of-frame rejected",
        !CAN_ValidateFrame(frame)
    );


    /* Valid checksum */

    CAN_BuildFrame(&message1, frame);

    check(
        "Valid checksum accepted",
        CAN_ValidateFrame(frame)
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
