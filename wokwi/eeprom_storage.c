#include "main.h"
#include "eeprom_storage.h"

#include <stdio.h>
#include <string.h>

UART_HandleTypeDef huart2;

void SystemClock_Config(void);
static void MX_GPIO_Init(void);
static void MX_USART2_UART_Init(void);

static void UART_Print(const char *message);

int main(void)
{
    uint8_t value = 0U;

    uint8_t write_block[] =
    {
        0x11,
        0x22,
        0x33,
        0x44,
        0x55
    };

    uint8_t read_block[5] = {0};

    HAL_Init();

    SystemClock_Config();
    MX_GPIO_Init();
    MX_USART2_UART_Init();

    UART_Print("\r\n");
    UART_Print("========================================\r\n");
    UART_Print(" Automotive Dashboard ECU\r\n");
    UART_Print(" EEPROM Storage Test\r\n");
    UART_Print(" STM32C031C6\r\n");
    UART_Print(" Simulated EEPROM\r\n");
    UART_Print("========================================\r\n");

    EEPROM_Storage_Init();

    UART_Print("EEPROM INIT DONE\r\n");

    if (EEPROM_WriteByte(0x10U, 0x5AU))
        UART_Print("EEPROM WRITE BYTE: SUCCESS\r\n");
    else
        UART_Print("EEPROM WRITE BYTE: FAILED\r\n");

    if (EEPROM_ReadByte(0x10U, &value))
    {
        char buffer[64];

        sprintf(
            buffer,
            "EEPROM READ BYTE: 0x%02X\r\n",
            value
        );

        UART_Print(buffer);
    }

    if (EEPROM_WriteBlock(
            0x20U,
            write_block,
            5U))
    {
        UART_Print("EEPROM WRITE BLOCK: SUCCESS\r\n");
    }
    else
    {
        UART_Print("EEPROM WRITE BLOCK: FAILED\r\n");
    }

    if (EEPROM_ReadBlock(
            0x20U,
            read_block,
            5U))
    {
        char buffer[64];

        sprintf(
            buffer,
            "EEPROM READ BLOCK: %02X %02X %02X %02X %02X\r\n",
            read_block[0],
            read_block[1],
            read_block[2],
            read_block[3],
            read_block[4]
        );

        UART_Print(buffer);
    }

    UART_Print("----------------------------------------\r\n");
    UART_Print("EEPROM test complete.\r\n");

    while (1)
    {
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
        Error_Handler();

    clk.ClockType =
        RCC_CLOCKTYPE_HCLK |
        RCC_CLOCKTYPE_SYSCLK |
        RCC_CLOCKTYPE_PCLK1;

    clk.SYSCLKSource =
        RCC_SYSCLKSOURCE_HSI;

    clk.SYSCLKDivider =
        RCC_SYSCLK_DIV1;

    clk.AHBCLKDivider =
        RCC_HCLK_DIV1;

    clk.APB1CLKDivider =
        RCC_APB1_DIV1;

    if (HAL_RCC_ClockConfig(
            &clk,
            FLASH_LATENCY_1) != HAL_OK)
        Error_Handler();
}

static void MX_GPIO_Init(void)
{
    __HAL_RCC_GPIOA_CLK_ENABLE();
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
        Error_Handler();
}

void Error_Handler(void)
{
    __disable_irq();

    while (1)
    {
    }
}
