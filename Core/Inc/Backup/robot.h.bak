#ifndef __ROBOT_H__
#define __ROBOT_H__

#include "bsp_imu.h"
#include "wheel.h"
#include "winch.h"

typedef struct Robot_TypeDef_t
{
    Wheel_Typedef *wheel;    // 动量轮模组
    Winch *winch;    // 绳驱单元模组
} Robot_TypeDef;

Robot_TypeDef *Robot_Create(void);
//void Robot_IMU_Update(IMU_Data_Typedef *imu_data);
void Robot_RxCallback(uint8_t *data);

#endif
