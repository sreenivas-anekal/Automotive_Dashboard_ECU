#include <stdio.h>
#include <stdint.h>

#include "../app/inc/adc_sensor.h"

static int tests_passed = 0;
static int tests_failed = 0;

static void check_float(
    const char *test_name,
    float actual,
    float expected,
    float tolerance)
{
    float difference = actual - expected;

    if (difference < 0.0f)
    {
        difference = -difference;
    }

    if (difference <= tolerance)
    {
        printf("[PASS] %s\n", test_name);
        tests_passed++;
    }
    else
    {
        printf(
            "[FAIL] %s | Expected: %.3f | Actual: %.3f\n",
            test_name,
            expected,
            actual
        );

        tests_failed++;
    }
}

int main(void)
{
    printf("========================================\n");
    printf(" Automotive Dashboard ECU\n");
    printf(" ADC Sensor Module Unit Test\n");
    printf("========================================\n\n");

    /* Voltage conversion tests */

    check_float(
        "ADC 0 -> 0.000 V",
        ADC_ConvertToVoltage(0),
        0.0f,
        0.001f
    );

    check_float(
        "ADC 1024 -> approximately 0.825 V",
        ADC_ConvertToVoltage(1024),
        0.825f,
        0.002f
    );

    check_float(
        "ADC 2048 -> approximately 1.650 V",
        ADC_ConvertToVoltage(2048),
        1.650f,
        0.002f
    );

    check_float(
        "ADC 3072 -> approximately 2.475 V",
        ADC_ConvertToVoltage(3072),
        2.475f,
        0.002f
    );

    check_float(
        "ADC 4095 -> 3.300 V",
        ADC_ConvertToVoltage(4095),
        3.300f,
        0.001f
    );

    /* Fuel percentage tests */

    check_float(
        "ADC 0 -> 0.0% fuel",
        ADC_GetFuelLevelPercentage(0),
        0.0f,
        0.01f
    );

    check_float(
        "ADC 1024 -> approximately 25.0% fuel",
        ADC_GetFuelLevelPercentage(1024),
        25.0f,
        0.02f
    );

    check_float(
        "ADC 2048 -> approximately 50.0% fuel",
        ADC_GetFuelLevelPercentage(2048),
        50.0f,
        0.02f
    );

    check_float(
        "ADC 3072 -> approximately 75.0% fuel",
        ADC_GetFuelLevelPercentage(3072),
        75.0f,
        0.02f
    );

    check_float(
        "ADC 4095 -> 100.0% fuel",
        ADC_GetFuelLevelPercentage(4095),
        100.0f,
        0.01f
    );

    /* Boundary test */

    check_float(
        "ADC 5000 -> clamped to 100.0% fuel",
        ADC_GetFuelLevelPercentage(5000),
        100.0f,
        0.01f
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
