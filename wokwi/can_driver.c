#include "main.h"
#include "can_driver.h"

#include <stdio.h>
#include <string.h>

UART_HandleTypeDef huart2;

void SystemClock_Config(void);
static void MX_GPIO_Init(void);
static void MX_USART2_UART_Init(void);

static void UART_Print(const char *message);

int main(void)
{
    CAN_Message_t message;

    HAL_Init();

    SystemClock_Config();
    MX_GPIO_Init();
    MX_USART2_UART_Init();

    UART_Print("\r\n");
    UART_Print("========================================\r\n");
    UART_Print(" Automotive Dashboard ECU\r\n");
    UART_Print(" CAN Driver Test\r\n");
    UART_Print(" STM32C031C6\r\n");
    UART_Print(" Simulated CAN Transport\r\n");
    UART_Print("========================================\r\n");

    CAN_Driver_Init();

    UART_Print("CAN DRIVER INIT DONE\r\n");

    memset(
        &message,
        0,
        sizeof(message)
    );

    message.id = 0x123U;
    message.length = 8U;

    message.data[0] = 0x10U;
    message.data[1] = 0x20U;
    message.data[2] = 0x30U;
    message.data[3] = 0x40U;
    message.data[4] = 0x50U;
    message.data[5] = 0x60U;
    message.data[6] = 0x70U;
    message.data[7] = 0x80U;

    if (CAN_SendMessage(&message))
    {
        UART_Print("CAN TX: SUCCESS\r\n");
    }
    else
    {
        UART_Print("CAN TX: FAILED\r\n");
    }

    {
        char buffer[64];

        sprintf(
            buffer,
            "ID: 0x%03X DLC: %u\r\n",
            message.id,
            message.length
        );

        UART_Print(buffer);
    }

    UART_Print("DATA:");

    for (uint8_t i = 0U; i < message.length; i++)
    {
        char byte_buffer[8];

        sprintf(
            byte_buffer,
            " %02X",
            message.data[i]
        );

        UART_Print(byte_buffer);
    }

    UART_Print("\r\n");
    UART_Print("----------------------------------------\r\n");
    UART_Print("CAN driver test complete.\r\n");

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

    osc.OscillatorType =
        RCC_OSCILLATORTYPE_HSI;

    osc.HSIState =
        RCC_HSI_ON;

    osc.HSIDiv =
        RCC_HSI_DIV1;

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
    {
        Error_Handler();
    }
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

    gpio.Pin =
        GPIO_PIN_2 | GPIO_PIN_3;

    gpio.Mode =
        GPIO_MODE_AF_PP;

    gpio.Pull =
        GPIO_NOPULL;

    gpio.Speed =
        GPIO_SPEED_FREQ_VERY_HIGH;

    gpio.Alternate =
        GPIO_AF1_USART2;

    HAL_GPIO_Init(
        GPIOA,
        &gpio
    );

    huart2.Instance =
        USART2;

    huart2.Init.BaudRate =
        115200;

    huart2.Init.WordLength =
        UART_WORDLENGTH_8B;

    huart2.Init.StopBits =
        UART_STOPBITS_1;

    huart2.Init.Parity =
        UART_PARITY_NONE;

    huart2.Init.Mode =
        UART_MODE_TX_RX;

    huart2.Init.HwFlowCtl =
        UART_HWCONTROL_NONE;

    huart2.Init.OverSampling =
        UART_OVERSAMPLING_16;

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
