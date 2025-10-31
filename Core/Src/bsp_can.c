#include "bsp_can.h"

// CAN1发送帧头
CAN_TxHeaderTypeDef TxHeader = {
    .StdId = 0x200,
    .DLC = 0x08,
    .IDE = CAN_ID_STD,
    .RTR = CAN_RTR_DATA
};

// CAN1接收帧头
CAN_RxHeaderTypeDef RxHeader;


//uint8_t CanRxData_FIFO0[8] = {0}; //CAN1接收数据缓冲区
uint8_t TxData[8] = {0}; //CAN发送数据
 
/**
  * @brief CAN外设过滤器初始化
  * @retval None
  */
void CanFilterInit()
{
    CAN_FilterTypeDef sFilterConfig;
	
    sFilterConfig.FilterActivation = CAN_FILTER_ENABLE;
    sFilterConfig.FilterBank = 0;
    sFilterConfig.FilterFIFOAssignment = CAN_FILTER_FIFO0;
    sFilterConfig.FilterMode = CAN_FILTERMODE_IDLIST;
    sFilterConfig.FilterScale = CAN_FILTERSCALE_16BIT;
    // CAN1FIFO0过滤电机ID为1,2,3,4
    sFilterConfig.FilterIdHigh = 0x201 << 5;
    sFilterConfig.FilterIdLow = 0x202 << 5;
    sFilterConfig.FilterMaskIdHigh = 0x203 << 5;
    sFilterConfig.FilterMaskIdLow = 0x204 << 5;
    sFilterConfig.SlaveStartFilterBank = 14;
	
    // sFilterConfig.FilterMode = CAN_FILTERMODE_IDMASK;
    // sFilterConfig.FilterScale = CAN_FILTERSCALE_32BIT;
    // sFilterConfig.FilterIdHigh = 0x00;
    // sFilterConfig.FilterIdLow = 0x00;
    // sFilterConfig.FilterMaskIdHigh = 0x00;
    // sFilterConfig.FilterMaskIdLow =0x00;
    // sFilterConfig.SlaveStartFilterBank = 14;
	
//sFilterConfig.FilterBank = 0;
//sFilterConfig.FilterMode = CAN_FILTERMODE_IDMASK;
//sFilterConfig.FilterScale = CAN_FILTERSCALE_32BIT;
//sFilterConfig.FilterIdHigh = 0x0000;
//sFilterConfig.FilterIdLow = 0x0000;
//sFilterConfig.FilterMaskIdHigh = 0x0000;
//sFilterConfig.FilterMaskIdLow = 0x0000;
//sFilterConfig.FilterFIFOAssignment = CAN_FilterFIFO0;
//sFilterConfig.SlaveStartFilterBank = 14;//can1(0-13)和can2(14-27)分别得到一半的filter
//sFilterConfig.FilterActivation = ENABLE;


    // 配置CAN1FIFO0过滤器
    if (HAL_CAN_ConfigFilter(&hcan1, &sFilterConfig) != HAL_OK)
    {
        Error_Handler();
    }

    // 虽然未使用CAN2，但还是完成CAN2配置
    sFilterConfig.FilterBank = 14;
    if (HAL_CAN_ConfigFilter(&hcan2, &sFilterConfig) != HAL_OK)
    {
        Error_Handler();
    }

    // 启用CAN1和CAN2
    if (HAL_CAN_Start(&hcan1) != HAL_OK)
    {
        Error_Handler();
    }

    if (HAL_CAN_Start(&hcan2) != HAL_OK)
    {
        Error_Handler();
    }

    // 启用FIFO0中断
    if (HAL_CAN_ActivateNotification(&hcan1, CAN_IT_RX_FIFO0_MSG_PENDING) != HAL_OK)
    {
        Error_Handler();
    }

    if (HAL_CAN_ActivateNotification(&hcan2, CAN_IT_RX_FIFO0_MSG_PENDING) != HAL_OK)
    {
        Error_Handler();
    }
}

__weak void CAN1_Receive(uint32_t motorID, uint8_t *RxData)
{
    
}

/**
  * @brief CAN通信接收中断回调函数-FIFO0
  * @param CAN序号
  * @retval None
  */
void HAL_CAN_RxFifo0MsgPendingCallback(CAN_HandleTypeDef *hcan)
{
    uint8_t CanRxData_FIFO0[8] = {0};
	if(HAL_CAN_GetRxMessage(hcan, CAN_RX_FIFO0, &RxHeader, CanRxData_FIFO0) != HAL_OK)
	{
		Error_Handler();            
	}
	if(hcan == &hcan1)
		CAN1_Receive(RxHeader.StdId, CanRxData_FIFO0);   //进行CAN数据解析
}

/**
  * @brief ID1~4的电机电流发送函数
  * @param ID1~4的各个电机电流值
  * @retval None
  */
 void BSP_CAN_Transmit(CAN_HandleTypeDef *hcan, int16_t cm1_iq, int16_t cm2_iq, int16_t cm3_iq, int16_t cm4_iq)
 {
	TxData[0] = cm1_iq >> 8;
	TxData[1] = cm1_iq;
	TxData[2] = (uint8_t)(cm2_iq >> 8);
	TxData[3] = (uint8_t)(cm2_iq & 0xFF);
	TxData[4] = (uint8_t)(cm3_iq >> 8);
	TxData[5] = (uint8_t)(cm3_iq & 0xFF);
	TxData[6] = (uint8_t)(cm4_iq >> 8);
	TxData[7] = (uint8_t)(cm4_iq & 0xFF);
    uint32_t Can_TxMailbox;
	HAL_CAN_AddTxMessage(hcan, &TxHeader, TxData, &Can_TxMailbox);
 }