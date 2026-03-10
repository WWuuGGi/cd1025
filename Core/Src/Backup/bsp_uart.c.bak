#include "bsp_uart.h"
#include "winch.h"
#include "JY901S.h"

BSP_UART_TypeDef uart_ports[DEVICE_UART_CNT];

void BSP_UART_Init()
{
    uart_ports[0].port = PC_UART;
    uart_ports[1].port = JY901S_UART;

    for(uint8_t i = 0; i < DEVICE_UART_CNT; i++)
    {
        // 使能串口空闲中断
        __HAL_UART_ENABLE_IT(uart_ports[i].port, UART_IT_IDLE);
        // 开启串口DMA接收
        HAL_UART_Receive_DMA(uart_ports[i].port, uart_ports[i].rx_buff, BSP_UART_BUFFER_SIZE);
    }
}

void BSP_UART_Send(UART_HandleTypeDef *huart, uint8_t *data, uint16_t len, uint32_t timeout)
{
    HAL_UART_Transmit(huart, data, len, timeout);
}

void BSP_UART_Send_DMA(UART_HandleTypeDef *huart, uint8_t *data, uint16_t len)
{
    HAL_UART_Transmit_DMA(huart, data, len);
}

void BSP_UART_IDLECallback(UART_HandleTypeDef *huart)
{
    // 判断是否进入空闲中断
    if(__HAL_UART_GET_FLAG(huart, UART_FLAG_IDLE))
    {
        __HAL_UART_CLEAR_IDLEFLAG(huart); //清除空闲中断标志
        HAL_UART_DMAStop(huart); //暂停本次DMA传输，进行数据处理
        
        // 数据处理回调函数
        if(huart == PC_UART)
        {
            Winch_RxCallback(uart_ports[0].rx_buff);
            HAL_UART_Receive_DMA(huart, uart_ports[0].rx_buff, BSP_UART_BUFFER_SIZE); //重启DMA接收
        }
            
        if(huart == JY901S_UART)
        {
            JY901S_RxCallback(uart_ports[1].rx_buff);
            HAL_UART_Receive_DMA(huart, uart_ports[1].rx_buff, BSP_UART_BUFFER_SIZE); //重启DMA接收
        }
            
    }
}

/**
  * @brief 串口空闲中断（中断回调）函数
  * @param 串口号
  * @retval None
  * @note  放在"stm32f4xx_it.c"里形如"void USART2_IRQHandler(void)"类的函数中，只要用了DMA接收的串口都放
  * 具体位置：系统调用的HAL_UART_IRQHandler函数下面，"USER CODE BEGIN USART1_IRQn 1"和"USER CODE END USART1_IRQn 1"两行注释之间
  */
void BSP_UART_IRQHandler(UART_HandleTypeDef *huart)
{
    if(huart == PC_UART)
        BSP_UART_IDLECallback(PC_UART);
    if(huart == JY901S_UART)
        BSP_UART_IDLECallback(JY901S_UART);
    
}