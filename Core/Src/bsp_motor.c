#include "bsp_motor.h"
#include "bsp_can.h"



// CAN_Motor CAN1_2006Motor1;
// CAN_Motor CAN1_2006Motor2;
// CAN_Motor CAN1_2006Motor3;
// CAN_Motor CAN1_2006Motor4;
CAN_Motor *CAN1_Motor[4]; //用于存放CAN_Motor*类型的指向电机实体的指针，在每个电机初始化时被填充

void CAN1_Motor_Driver_Init()
{
    memset(&CAN1_Motor, 0, sizeof(CAN1_Motor));
}

CAN_Motor *CAN1_Motor_Create(uint8_t motorID)
{
    CAN_Motor *obj = (CAN_Motor *)malloc(sizeof(CAN_Motor));
    memset(obj, 0, sizeof(CAN_Motor));
    CAN1_Motor[motorID] = obj;

    return obj;
}

void data_process(CAN_Motor *motor, uint8_t *RxData)
{
    motor->last_fdbPos = motor->fdbPos;
    motor->fdbPos = (((uint16_t)RxData[0]) << 8) | RxData[1];
    motor->fdbSpeed = (((uint16_t)RxData[2]) << 8) | RxData[3];
    motor->fdbCurrent = (((uint16_t)RxData[4]) << 8) | RxData[5];

    if (motor->fdbPos - motor->last_fdbPos > 4096)
    {
        motor->round --;
    }
    else if (motor->fdbPos - motor->last_fdbPos < -4096)
    {
        motor->round ++;
    }

    motor->realPos = (motor->fdbPos + motor->round * 8192);
    motor->now_pos = motor->realPos * 360.0f / 8192.0f / 36.0f;
}

void CAN1_Receive(uint32_t motorID, uint8_t *RxData)
{
    switch(motorID)
    {
        case CAN1_2006Motor1_ID:
            data_process(CAN1_Motor[0], RxData);
            break;
        case CAN1_2006Motor2_ID:
            data_process(CAN1_Motor[1], RxData);
            break;
        case CAN1_2006Motor3_ID:
            data_process(CAN1_Motor[2], RxData);
            break;
        case CAN1_2006Motor4_ID:
            data_process(CAN1_Motor[3], RxData);
            break;
        default:
            break;
    }
}