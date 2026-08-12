#include "adc_sensor.h"
#include "stm32c0xx_hal.h"

static ADC_HandleTypeDef hadc1;

void ADC_Sensor_Init(void)
{
    ADC_ChannelConfTypeDef config = {0};
    GPIO_InitTypeDef gpio = {0};

    __HAL_RCC_GPIOA_CLK_ENABLE();
    __HAL_RCC_ADC_CLK_ENABLE();

    /* PA0 = ADC1 channel 0 */
    gpio.Pin = GPIO_PIN_0;
    gpio.Mode = GPIO_MODE_ANALOG;
    gpio.Pull = GPIO_NOPULL;

    HAL_GPIO_Init(GPIOA, &gpio);

    hadc1.Instance = ADC1;

    hadc1.Init.ClockPrescaler = ADC_CLOCK_SYNC_PCLK_DIV2;
    hadc1.Init.Resolution = ADC_RESOLUTION_12B;
    hadc1.Init.DataAlign = ADC_DATAALIGN_RIGHT;
    hadc1.Init.ScanConvMode = ADC_SCAN_SEQ_FIXED;
    hadc1.Init.EOCSelection = ADC_EOC_SINGLE_CONV;
    hadc1.Init.LowPowerAutoWait = DISABLE;
    hadc1.Init.LowPowerAutoPowerOff = DISABLE;
    hadc1.Init.ContinuousConvMode = DISABLE;
    hadc1.Init.NbrOfConversion = 1;
    hadc1.Init.DiscontinuousConvMode = DISABLE;
    hadc1.Init.ExternalTrigConv = ADC_SOFTWARE_START;
    hadc1.Init.ExternalTrigConvEdge =
        ADC_EXTERNALTRIGCONVEDGE_NONE;
    hadc1.Init.DMAContinuousRequests = DISABLE;
    hadc1.Init.Overrun = ADC_OVR_DATA_OVERWRITTEN;
    hadc1.Init.SamplingTimeCommon1 =
        ADC_SAMPLETIME_39CYCLES_5;

    if (HAL_ADC_Init(&hadc1) != HAL_OK)
    {
        return;
    }

    config.Channel = ADC_CHANNEL_0;
    config.Rank = ADC_RANK_CHANNEL_NUMBER;
    config.SamplingTime =
        ADC_SAMPLINGTIME_COMMON_1;

    if (HAL_ADC_ConfigChannel(&hadc1, &config) != HAL_OK)
    {
        return;
    }
}

uint16_t ADC_Read_Raw(uint8_t channel)
{
    uint16_t value;

    if (channel != 0U)
    {
        return 0U;
    }

    if (HAL_ADC_Start(&hadc1) != HAL_OK)
    {
        return 0U;
    }

    if (HAL_ADC_PollForConversion(&hadc1, 100U) != HAL_OK)
    {
        HAL_ADC_Stop(&hadc1);
        return 0U;
    }

    value = (uint16_t)HAL_ADC_GetValue(&hadc1);

    HAL_ADC_Stop(&hadc1);

    return value;
}

float ADC_ConvertToVoltage(uint16_t raw_value)
{
    if (raw_value > ADC_SENSOR_MAX_VALUE)
    {
        raw_value = ADC_SENSOR_MAX_VALUE;
    }

    return ((float)raw_value * ADC_SENSOR_REFERENCE_VOLT)
           / (float)ADC_SENSOR_MAX_VALUE;
}

float ADC_GetFuelLevelPercentage(uint16_t raw_value)
{
    if (raw_value > ADC_SENSOR_MAX_VALUE)
    {
        raw_value = ADC_SENSOR_MAX_VALUE;
    }

    return ((float)raw_value * 100.0f)
           / (float)ADC_SENSOR_MAX_VALUE;
}
