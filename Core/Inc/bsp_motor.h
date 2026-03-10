#ifndef __BSP_MOTOR_H__
#define __BSP_MOTOR_H__

#include "stdint.h"
#include "string.h"
#include "stdlib.h"

// 4个M2006电机的ID
#define CAN1_2006Motor1_ID 0x201
#define CAN1_2006Motor2_ID 0x202
#define CAN1_2006Motor3_ID 0x203
#define CAN1_2006Motor4_ID 0x204
#define CAN1_2006Motor5_ID 0x205

typedef struct CAN_Motor_t
{
    int realPos;        //电机实际位置(编码器值)
    short fdbPos;       //电机反馈转子机械角度(0~8191 —— 0~360°)
    short last_fdbPos;  //上次电机反馈转子机械角度
    short fdbSpeed;     //电机反馈转速(rpm)
    short round;        //电机转动圈数，用来计算电机实际位置
    short fdbCurrent;   //电机反馈转矩电流(-10000~0~10000 —— -10~10A)

    float init_pos;     //初始位置
    float ref_pos;      //期望位置
    float ref_speed;    //期望速度
    float now_pos;      //当前位置(绞盘角度/°)
} CAN_Motor;

void CAN1_Motor_Driver_Init();
CAN_Motor *CAN1_Motor_Create(uint8_t motorID);

#endif