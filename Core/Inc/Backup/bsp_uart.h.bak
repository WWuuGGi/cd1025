#ifndef __BSP_UART_H__
#define __BSP_UART_H__

#include "stdint.h"
#include "usart.h"

#define BSP_UART_BUFFER_SIZE 255
#define DEVICE_UART_CNT 2

#define PC_UART &huart6
#define WHEEL_UART &huart7
#define JY901S_UART &huart8

typedef struct BSP_UART_TypeDef_t
{
    UART_HandleTypeDef *port;
    uint8_t rx_buff[BSP_UART_BUFFER_SIZE];
} BSP_UART_TypeDef;

void BSP_UART_Init();
void BSP_UART_Send(UART_HandleTypeDef *huart, uint8_t *data, uint16_t len, uint32_t timeout);
void BSP_UART_Send_DMA(UART_HandleTypeDef *huart, uint8_t *data, uint16_t len);
void BSP_UART_IDLECallback(UART_HandleTypeDef *huart);
void BSP_UART_IRQHandler(UART_HandleTypeDef *huart);
#endif