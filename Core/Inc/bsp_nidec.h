/**
 *******************************************
 * @file    : bsp_nidec.h
 * @author  : 胡咏杰
 * @brief   : Nidec电机（动量轮电机）相关函数
 * @version : v1.0
 * @date    : 2025-11-13
 *******************************************
 */

#ifndef __BSP_NIDEC_H__
#define __BSP_NIDEC_H__

#include "tim.h"

// 电机参数
// #define Kt 0.0375f        // 电机转矩常数(Nm/A)
// #define Ke 0.0375f        // 电机电动势常数(Vs/rad)
#define Kt 0.0366f        // 电机转矩常数(Nm/A)
#define Ke 0.0366f        // 电机电动势常数(Vs/rad)
#define Rc 3.0f           // 电机电阻(Ω)
#define Vbus 24.0f        // 电源电压(V)
#define PWM_DEADZONE 8200 // PWM死区值（大于该值，电机不转动）
#define PWM_MAX 8399      // 最大PWM值

typedef struct Nidec_Typedef_t
{
    uint8_t id;           // 电机ID
    float Ts;             // 采样时间
    int16_t CPR;          // 旋转一圈的脉冲数
    int32_t counter;      // 电机位置计数器
    int32_t last_counter; // 上一个位置计数器
    float raw_speed;      // 未滤波前的电机速度（rad/s）
    float alpha;          // 速度滤波系数
    float filtered_speed; // 滤波后的电机速度（rad/s）

} Nidec_Typedef;

void BSP_Nidec_Init(void);
Nidec_Typedef *Nidec_Motor_Create(uint8_t id);
void Nidec_Data_Update(Nidec_Typedef *motor);
void Wheel_Motor_Update(void);
uint16_t Nidec_Torque2PWM(float tau, float speed);

#endif
