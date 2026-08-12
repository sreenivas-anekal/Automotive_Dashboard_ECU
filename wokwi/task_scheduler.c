#include "main.h"
#include "task_scheduler.h"

#include <stdio.h>
#include <string.h>

UART_HandleTypeDef huart2;

void SystemClock_Config(void);
static void MX_GPIO_Init(void);
static void MX_USART2_UART_Init(void);

static void UART_Print(const char *message);

static uint32_t task1_count = 0U;
static uint32_t task2_count = 0U;

static void Task1_Callback(void)
{
    task1_count++;
}

static void Task2_Callback(void)
{
    task2_count++;
}

int main(void)
{
    HAL_Init();

    SystemClock_Config();
    MX_GPIO_Init();
    MX_USART2_UART_Init();

    UART_Print("\r\n");
    UART_Print("========================================\r\n");
    UART_Print(" Automotive Dashboard ECU\r\n");
    UART_Print(" Task Scheduler Test\r\n");
    UART_Print(" STM32C031C6\r\n");
    UART_Print("========================================\r\n");

    Task_Scheduler_Init();

    UART_Print("TASK SCHEDULER INIT DONE\r\n");

    Task_Register(
        0U,
        100U,
        Task1_Callback
    );

    Task_Register(
        1U,
        250U,
        Task2_Callback
    );

    UART_Print("TASKS REGISTERED\r\n");

    /*
     * Simulated scheduler time.
     */
    for (uint32_t time_ms = 0U;
         time_ms <= 500U;
         time_ms += 50U)
    {
        Task_Scheduler_Run(time_ms);

        HAL_Delay(50U);
    }

    {
        char buffer[100];

        sprintf(
            buffer,
            "Task 1 executions: %lu\r\n",
            task1_count
        );

        UART_Print(buffer);

        sprintf(
            buffer,
            "Task 2 executions: %lu\r\n",
            task2_count
        );

        UART_Print(buffer);
    }

    UART_Print("----------------------------------------\r\n");
    UART_Print("Task scheduler test complete.\r\n");

    while (1)
    {
        HAL_Delay(1000U);
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
