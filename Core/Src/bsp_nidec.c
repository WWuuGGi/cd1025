#include "bsp_nidec.h"
#include "string.h"
#include "stdlib.h"

static Nidec_Typedef *Wheel_Motor[4] = {NULL, NULL, NULL, NULL}; // 用于存放动量轮电机的指针

// 初始化Nidec电机
void BSP_Nidec_Init(void)
{
    // 初始化Nidec电机编码器
    HAL_TIM_Encoder_Start(&htim2, TIM_CHANNEL_ALL);
    HAL_TIM_Encoder_Start(&htim3, TIM_CHANNEL_ALL);
    HAL_TIM_Encoder_Start(&htim4, TIM_CHANNEL_ALL);
    HAL_TIM_Encoder_Start(&htim5, TIM_CHANNEL_ALL);

    // 初始化Nidec电机PWM
    HAL_TIM_PWM_Start(&htim8, TIM_CHANNEL_1);
    HAL_TIM_PWM_Start(&htim8, TIM_CHANNEL_2);
    HAL_TIM_PWM_Start(&htim8, TIM_CHANNEL_3);
    HAL_TIM_PWM_Start(&htim8, TIM_CHANNEL_4);
}

// 创建Nidec电机实例
Nidec_Typedef *Nidec_Motor_Create(uint8_t id)
{
    Nidec_Typedef *motor = (Nidec_Typedef *)malloc(sizeof(Nidec_Typedef));
    if (motor == NULL)
        return NULL;
    memset(motor, 0, sizeof(Nidec_Typedef));
    motor->id = id;
    motor->Ts = 0.005f;  // 采样时间为5ms
    motor->CPR = 400;    // 旋转一圈脉冲数为100(PPR)，采用4倍频，因此CPR为400
    motor->alpha = 0.25f; // 速度滤波系数
    // 根据电机ID进行设置速度滤波系数
    // switch (id)
    // {
    // case 1:
    //     motor->alpha = 0.2f;
    //     break;
    // case 2:
    //     motor->alpha = 0.3f;
    //     break;
    // case 3:
    //     motor->alpha = 0.2f;
    //     break;
    // case 4:
    //     motor->alpha = 0.2f;
    //     break;
    // default:
    //     break;
    // }

    Wheel_Motor[id - 1] = motor;
    return motor;
}

// 获取Nidec电机编码器数据并更新
void Nidec_Data_Update(Nidec_Typedef *motor)
{
    // 更新电机位置计数器
    switch (motor->id)
    {
    case 1:
        motor->counter = __HAL_TIM_GET_COUNTER(&htim2);
        break;
    case 2:
        motor->counter = __HAL_TIM_GET_COUNTER(&htim3);
        break;
    case 3:
        motor->counter = __HAL_TIM_GET_COUNTER(&htim4);
        break;
    case 4:
        motor->counter = __HAL_TIM_GET_COUNTER(&htim5);
        break;

    default:
        break;
    }

    int32_t delta_counter = motor->counter - motor->last_counter;

    // TIM3和TIM4为16位编码器，需要进行溢出处理
    if (motor->id == 3 || motor->id == 4)
    {
        if (delta_counter > 32767)
            delta_counter -= 65536;
        else if (delta_counter < -32768)
            delta_counter += 65536;
    }

    // 计算电机速度
    //弧度制角速度 如果一秒一圈就是6.28 rad/s
    motor->raw_speed = delta_counter * 2 * 3.14f / motor->CPR / motor->Ts;
    motor->raw_speed = -motor->raw_speed; // 方向调整

    // 速度滤波
    motor->filtered_speed = motor->alpha * motor->raw_speed + (1 - motor->alpha) * motor->filtered_speed;
    

    // 更新上一个位置计数器
    motor->last_counter = motor->counter;
}

// 更新动量轮电机数据
void Wheel_Motor_Update(void)
{
    for (int i = 0; i < 4; i++)
    {
        Nidec_Data_Update(Wheel_Motor[i]);
    }
}

uint16_t Nidec_Torque2PWM(float tau, float speed)
{
    if (tau <= 0.0f)
        return 0;

    float I_req = tau / Kt;
    float V_req = I_req * Rc + speed * Ke;

    if (V_req < 0.0f)
        V_req = 0.0f;
    if (V_req > Vbus)
        V_req = Vbus;

    float duty = V_req / Vbus;
    uint16_t amp = (uint16_t)(duty * PWM_MAX);
    uint16_t min_amp = (uint16_t)((PWM_MAX - PWM_DEADZONE) + 50);
    if (amp > 0 && amp < min_amp)
        amp = min_amp;

    if (amp > PWM_MAX)
        amp = PWM_MAX;

    return amp;
}
