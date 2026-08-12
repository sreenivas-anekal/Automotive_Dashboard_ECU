#include "adc_sensor.h"

static uint16_t adc_raw_value = 0U;

void ADC_Sensor_Init(void)
{
    adc_raw_value = 0U;
}

uint16_t ADC_Sensor_ReadRaw(void)
{
    return adc_raw_value;
}

float ADC_Sensor_GetVoltage(uint16_t raw_value)
{
    if (raw_value > ADC_SENSOR_MAX_VALUE)
    {
        raw_value = ADC_SENSOR_MAX_VALUE;
    }

    return ((float)raw_value * ADC_SENSOR_REFERENCE_VOLT)
           / (float)ADC_SENSOR_MAX_VALUE;
}

float ADC_Sensor_GetFuelPercentage(uint16_t raw_value)
{
    if (raw_value > ADC_SENSOR_MAX_VALUE)
    {
        raw_value = ADC_SENSOR_MAX_VALUE;
    }

    return ((float)raw_value * 100.0f)
           / (float)ADC_SENSOR_MAX_VALUE;
}
