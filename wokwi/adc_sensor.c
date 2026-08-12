#include "main.h"
#include "adc_sensor.h"

#include <stdio.h>

ADC_HandleTypeDef hadc1;
UART_HandleTypeDef huart2;

void SystemClock_Config(void);
static void MX_GPIO_Init(void);
static void MX_ADC1_Init(void);
static void MX_USART2_UART_Init(void);

static void UART_Print(const char *message);

int main(void)
{
    HAL_Init();

    SystemClock_Config();
    MX_GPIO_Init();
    MX_ADC1_Init();
    MX_USART2_UART_Init();

    UART_Print("\r\n");
    UART_Print("========================================\r\n");
    UART_Print(" Automotive Dashboard ECU\r\n");
    UART_Print(" ADC Sensor Test\r\n");
    UART_Print(" STM32C031C6\r\n");
    UART_Print(" PA0 / ADC1 Channel 0\r\n");
    UART_Print("========================================\r\n");

    UART_Print("UART OK\r\n");

    ADC_Sensor_Init();

    UART_Print("ADC INIT DONE\r\n");

    while (1)
    {
        uint16_t raw_value;
        float voltage;
        float fuel_percentage;
        char buffer[100];

        HAL_ADC_Start(&hadc1);

        HAL_ADC_PollForConversion(
            &hadc1,
            HAL_MAX_DELAY
        );

        raw_value =
            (uint16_t)HAL_ADC_GetValue(&hadc1);

        HAL_ADC_Stop(&hadc1);

        voltage =
            ADC_Sensor_GetVoltage(raw_value);

        fuel_percentage =
            ADC_Sensor_GetFuelPercentage(raw_value);

        sprintf(
            buffer,
            "ADC Raw: %u | Voltage: %.3f V | Fuel: %.1f %%\r\n",
            raw_value,
            voltage,
            fuel_percentage
        );

        UART_Print(buffer);

        HAL_Delay(1000);
    }
}

static void UART_Print(const char *message)
{
    HAL_UART_Transmit(
        &huart2,
        (uint8_t *)message,
        strlen(message),
        HAL_MAX_DELAY
    );
}

void SystemClock_Config(void)
{
    RCC_OscInitTypeDef osc = {0};
    RCC_ClkInitTypeDef clk = {0};

    osc.OscillatorType = RCC_OSCILLATORTYPE_HSI;
    osc.HSIState = RCC_HSI_ON;
    osc.HSIDiv = RCC_HSI_DIV1;
    osc.HSICalibrationValue =
        RCC_HSICALIBRATION_DEFAULT;

    if (HAL_RCC_OscConfig(&osc) != HAL_OK)
    {
        Error_Handler();
    }

    clk.ClockType =
        RCC_CLOCKTYPE_HCLK |
        RCC_CLOCKTYPE_SYSCLK |
        RCC_CLOCKTYPE_PCLK1;

    clk.SYSCLKSource = RCC_SYSCLKSOURCE_HSI;
    clk.SYSCLKDivider = RCC_SYSCLK_DIV1;
    clk.AHBCLKDivider = RCC_HCLK_DIV1;
    clk.APB1CLKDivider = RCC_APB1_DIV1;

    if (HAL_RCC_ClockConfig(
            &clk,
            FLASH_LATENCY_1) != HAL_OK)
    {
        Error_Handler();
    }
}

static void MX_ADC1_Init(void)
{
    ADC_ChannelConfTypeDef channel = {0};

    __HAL_RCC_ADC_CLK_ENABLE();

    hadc1.Instance = ADC1;
    hadc1.Init.ClockPrescaler =
        ADC_CLOCK_ASYNC_DIV1;
    hadc1.Init.Resolution =
        ADC_RESOLUTION_12B;
    hadc1.Init.DataAlign =
        ADC_DATAALIGN_RIGHT;
    hadc1.Init.ScanConvMode =
        ADC_SCAN_DISABLE;
    hadc1.Init.EOCSelection =
        ADC_EOC_SINGLE_CONV;
    hadc1.Init.LowPowerAutoWait = DISABLE;
    hadc1.Init.ContinuousConvMode = DISABLE;
    hadc1.Init.NbrOfConversion = 1;
    hadc1.Init.DiscontinuousConvMode = DISABLE;
    hadc1.Init.ExternalTrigConv =
        ADC_SOFTWARE_START;
    hadc1.Init.ExternalTrigConvEdge =
        ADC_EXTERNALTRIGCONVEDGE_NONE;
    hadc1.Init.DMAContinuousRequests = DISABLE;
    hadc1.Init.Overrun = ADC_OVR_DATA_PRESERVED;
    hadc1.Init.SamplingTimeCommon1 =
        ADC_SAMPLETIME_39CYCLES_5;

    if (HAL_ADC_Init(&hadc1) != HAL_OK)
    {
        Error_Handler();
    }

    channel.Channel = ADC_CHANNEL_0;
    channel.Rank = ADC_REGULAR_RANK_1;
    channel.SamplingTime =
        ADC_SAMPLINGTIME_COMMON_1;

    if (HAL_ADC_ConfigChannel(
            &hadc1,
            &channel) != HAL_OK)
    {
        Error_Handler();
    }
}

static void MX_GPIO_Init(void)
{
    GPIO_InitTypeDef gpio = {0};

    __HAL_RCC_GPIOA_CLK_ENABLE();

    gpio.Pin = GPIO_PIN_0;
    gpio.Mode = GPIO_MODE_ANALOG;
    gpio.Pull = GPIO_NOPULL;

    HAL_GPIO_Init(GPIOA, &gpio);
}

static void MX_USART2_UART_Init(void)
{
    GPIO_InitTypeDef gpio = {0};

    __HAL_RCC_GPIOA_CLK_ENABLE();
    __HAL_RCC_USART2_CLK_ENABLE();

    gpio.Pin = GPIO_PIN_2 | GPIO_PIN_3;
    gpio.Mode = GPIO_MODE_AF_PP;
    gpio.Pull = GPIO_NOPULL;
    gpio.Speed = GPIO_SPEED_FREQ_VERY_HIGH;
    gpio.Alternate = GPIO_AF1_USART2;

    HAL_GPIO_Init(GPIOA, &gpio);

    huart2.Instance = USART2;

    huart2.Init.BaudRate = 115200;
    huart2.Init.WordLength = UART_WORDLENGTH_8B;
    huart2.Init.StopBits = UART_STOPBITS_1;
    huart2.Init.Parity = UART_PARITY_NONE;
    huart2.Init.Mode = UART_MODE_TX_RX;
    huart2.Init.HwFlowCtl = UART_HWCONTROL_NONE;
    huart2.Init.OverSampling = UART_OVERSAMPLING_16;
    huart2.Init.OneBitSampling =
        UART_ONE_BIT_SAMPLE_DISABLE;
    huart2.Init.ClockPrescaler =
        UART_PRESCALER_DIV1;

    huart2.AdvancedInit.AdvFeatureInit =
        UART_ADVFEATURE_NO_INIT;

    if (HAL_UART_Init(&huart2) != HAL_OK)
    {
        Error_Handler();
    }
}

void Error_Handler(void)
{
    __disable_irq();

    while (1)
    {
    }
}
