#include "bsp_motor.h"
#include "bsp_can.h"



// CAN_Motor CAN1_2006Motor1;
// CAN_Motor CAN1_2006Motor2;
// CAN_Motor CAN1_2006Motor3;
// CAN_Motor CAN1_2006Motor4;
CAN_Motor *CAN1_Motor[5]; //用于存放CAN_Motor*类型的指向电机实体的指针，在每个电机初始化时被填充

// 动态学习：第4路电机反馈ID（默认0x204，若检测到其它ID将自动更新）
// static uint16_t can1_motor4_id = CAN1_2006Motor4_ID;

// 调试变量（已注释，需要时取消注释）
// volatile uint32_t g_m4_rx_cnt = 0;        // 调试：第4路电机接收计数
// volatile uint8_t g_m4_raw[8];              // 调试：第4路最近一次原始8字节反馈

void CAN1_Motor_Driver_Init()
{
    memset(&CAN1_Motor, 0, sizeof(CAN1_Motor));
    // can1_motor4_id = CAN1_2006Motor4_ID;
    // 调试代码（已注释，需要时取消注释）
    // g_m4_rx_cnt = 0;
    // memset((void*)g_m4_raw, 0, sizeof(g_m4_raw));
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
    // 若检测到并非 0x201~0x203 的标准反馈，但属于 0x200~0x2FF 范围，且第4路尚未更新过，则动态绑定为第4路
    // if (motorID >= 0x200 && motorID <= 0x2FF)
    // {
    //     if (motorID != CAN1_2006Motor1_ID && motorID != CAN1_2006Motor2_ID && motorID != CAN1_2006Motor3_ID)
    //     {
    //         // 如果第4路还没有实质更新（now_pos为0），则学习新的ID
    //         if (CAN1_Motor[3] != NULL && CAN1_Motor[3]->now_pos == 0)
    //         {
    //             can1_motor4_id = (uint16_t)motorID;
    //         }
    //     }
    // }

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
        case CAN1_2006Motor5_ID:
            data_process(CAN1_Motor[4], RxData);
            break;
                // 调试代码（已注释，需要时取消注释）
                // g_m4_rx_cnt++;
                // for (int i = 0; i < 8; ++i) g_m4_raw[i] = RxData[i];  // 记录原始8字节
            
    }
}