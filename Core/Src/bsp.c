#include "bsp.h"

void BSP_Layer_Init()
{
    CanFilterInit();
    imu_init();
    CAN1_Motor_Driver_Init();
    BSP_UART_Init();
    BSP_Nidec_Init();
}