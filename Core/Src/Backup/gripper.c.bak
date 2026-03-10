#include "gripper.h"
#include "tim.h"

void Gripper_grab()
{
    TIM4->CCR1 = 1500;
}

void Gripper_loose()
{
    TIM4->CCR1 = 2000;
}