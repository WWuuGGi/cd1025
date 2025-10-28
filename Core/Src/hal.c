#include "hal.h"
#include "tim.h"
#include "JY901S.h"

void HAL_Layer_Init()
{
    JY901S_Init();
    HAL_TIM_PWM_Start(&htim4, TIM_CHANNEL_1);
}