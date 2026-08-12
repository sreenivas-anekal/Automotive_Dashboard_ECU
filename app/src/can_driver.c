#include "can_driver.h"
#include "stm32c0xx_hal.h"
#include <string.h>

#define CAN_SOF 0xAAU
#define CAN_FRAME_SIZE 13U

static UART_HandleTypeDef huart_can;

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

void CAN_Driver_Init(void)
{
    GPIO_InitTypeDef gpio = {0};

    __HAL_RCC_GPIOA_CLK_ENABLE();
    __HAL_RCC_USART1_CLK_ENABLE();

    /*
     * USART1:
     * PA9  = TX
     * PA10 = RX
     */

    gpio.Pin = GPIO_PIN_9 | GPIO_PIN_10;
    gpio.Mode = GPIO_MODE_AF_PP;
    gpio.Pull = GPIO_NOPULL;
    gpio.Speed = GPIO_SPEED_FREQ_VERY_HIGH;
    gpio.Alternate = GPIO_AF1_USART1;

    HAL_GPIO_Init(GPIOA, &gpio);

    huart_can.Instance = USART1;

    huart_can.Init.BaudRate = 500000;
    huart_can.Init.WordLength = UART_WORDLENGTH_8B;
    huart_can.Init.StopBits = UART_STOPBITS_1;
    huart_can.Init.Parity = UART_PARITY_NONE;
    huart_can.Init.Mode = UART_MODE_TX_RX;
    huart_can.Init.HwFlowCtl = UART_HWCONTROL_NONE;
    huart_can.Init.OverSampling = UART_OVERSAMPLING_16;
    huart_can.Init.OneBitSampling =
        UART_ONE_BIT_SAMPLE_DISABLE;
    huart_can.Init.ClockPrescaler =
        UART_PRESCALER_DIV1;

    huart_can.AdvancedInit.AdvFeatureInit =
        UART_ADVFEATURE_NO_INIT;

    HAL_UART_Init(&huart_can);
}

bool CAN_SendMessage(const CAN_Message_t *message)
{
    uint8_t frame[CAN_FRAME_SIZE];

    if (message == NULL)
    {
        return false;
    }

    if (message->length > CAN_MAX_DATA_LENGTH)
    {
        return false;
    }

    /*
     * Standard CAN identifier:
     * 11-bit range = 0x000 to 0x7FF
     */
    if (message->id > 0x7FFU)
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

    if (HAL_UART_Transmit(
            &huart_can,
            frame,
            CAN_FRAME_SIZE,
            100U) != HAL_OK)
    {
        return false;
    }

    return true;
}

bool CAN_ReceiveMessage(CAN_Message_t *message)
{
    uint8_t frame[CAN_FRAME_SIZE];

    if (message == NULL)
    {
        return false;
    }

    if (HAL_UART_Receive(
            &huart_can,
            frame,
            CAN_FRAME_SIZE,
            100U) != HAL_OK)
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

    message->id =
        ((uint32_t)frame[1] << 8) |
        (uint32_t)frame[2];

    message->length = frame[3];

    memcpy(
        message->data,
        &frame[4],
        CAN_MAX_DATA_LENGTH
    );

    return true;
}
