#include "wheel.h"
#include "math.h"
#include "stdlib.h"
#include "string.h"
#include "FreeRTOS.h"
#include "task.h"
#include "winch.h"
#include "JY901S.h"

// 机器人参数
static const float H[3][3] = {
    {0.0209f, 0.0f, 0.0f},
    {0.0f, 0.0209f, 0.0f},
    {0.0f, 0.0f, 0.0120f}};
static const float p_skew[3][3] = {
    {0.0f, 0.1581f, 0.0f},
    {-0.1581f, 0.0f, 0.0f},
    {0.0f, 0.0f, 0.0f}};
static const float G[3][4] = {
    {1.0612e-04f, 0.0f, -1.0612e-04f, 0.0f},
    {0.0f, 1.0612e-04f, 0.0f, -1.0612e-04f},
    {7.1577e-05f, 7.1577e-05f, 7.1577e-05f, 7.1577e-05f}};
static const float G_inv[4][3] = {
    {4.7118e03f, 0.0f, 3.4928e03f},
    {0.0f, 4.7118e03f, 3.4928e03f},
    {-4.7118e03f, 0.0f, 3.4928e03f},
    {0.0f, -4.7118e03f, 3.4928e03f}};
static const float J[4][4] = {
    {1.28e-04f, 0.0f, 0.0f, 0.0f},
    {0.0f, 1.28e-04f, 0.0f, 0.0f},
    {0.0f, 0.0f, 1.28e-04f, 0.0f},
    {0.0f, 0.0f, 0.0f, 1.28e-04f}};
static const float g[3] = {0.0f, 0.0f, -9.81f};

// 控制增益
static const float Kp[3] = {800.0f, 800.0f, 800.0f};
static const float Ki[3] = {0.0f, 0.0f, 0.0f};
static const float Kd[3] = {240.0f, 240.0f, 240.0f};
static float error_sum[3] = {0.0f, 0.0f, 0.0f};

// 动量轮电机引脚
static GPIO_TypeDef *Motor_Dir_GPIO_Port[4] = {GPIOI, GPIOD, GPIOD, GPIOA};
static uint16_t Motor_Dir_Pin[4] = {GPIO_PIN_0, GPIO_PIN_14, GPIO_PIN_15, GPIO_PIN_2};
static uint32_t Motor_PWM_Channel[4] = {TIM_CHANNEL_1, TIM_CHANNEL_2, TIM_CHANNEL_3, TIM_CHANNEL_4};

static Wheel_Typedef *wheel_instance = NULL;

Wheel_Typedef *Wheel_Create(Winch *winch)
{
    Wheel_Typedef *wheel = (Wheel_Typedef *)malloc(sizeof(Wheel_Typedef));
    if (wheel == NULL)
        return NULL;
    memset(wheel, 0, sizeof(Wheel_Typedef));

    wheel->imu = winch->pose;
    for (uint8_t i = 0; i < 4; i++)
    {
        wheel->wheel_motor[i] = Nidec_Motor_Create(i + 1);
    }
    wheel->euler_d[0] = 0.0f;
    wheel->euler_d[1] = 0.0f;
    wheel->euler_d[2] = 41.0f;

    wheel_instance = wheel;
    return wheel;
}

// 设置机器人期望四元数
void Wheel_Set_Qd(void)
{
    if (wheel_instance->imu_ZeroFlag)
    {
        // XY轴角度置0
        JY901S_SetZero();
        HAL_Delay(2000);
        // 期望四元数设为当前四元数(Z轴角度无法置0)
        memcpy(wheel_instance->qd, wheel_instance->imu->q, sizeof(float) * 4);
        memcpy(wheel_instance->euler_d, wheel_instance->imu->euler, sizeof(float) * 3);

        wheel_instance->imu_ZeroFlag = 0;
    }
}

// 计算动力学模型中的非线性项
void compute_fnl(float fnl[3])
{
    // 运动增稳控制中的计算过程量
    static float dtheta[4];
    static float R[3][3], RT[3][3], w_skew[3][3];
    float temp11[3][3], temp21[3][3], temp31[3][4], temp41[3][3];
    float temp1[3], temp2[3], temp3[3], temp4[3];

    for (uint8_t i = 0; i < 4; i++)
    {
        dtheta[i] = wheel_instance->wheel_motor[i]->filtered_speed;
    }

    // 计算旋转矩阵及其转置
    quat2rot(wheel_instance->imu->q, R);
    mat_transpose(R, RT);

    // 计算p_skew*R^T*ddX
    matmul_33_33(p_skew, RT, temp11);
    matmul_33_31(temp11, wheel_instance->accd, temp1);

    // 计算w_skew*H*w
    skew(wheel_instance->imu->omega, w_skew);
    matmul_33_33(w_skew, H, temp21);
    matmul_33_31(temp21, wheel_instance->imu->omega, temp2);

    // 计算w_skew*G*dtheta
    matmul_33_34(w_skew, G, temp31);
    matmul_34_41(temp31, dtheta, temp3);

    // 计算p_skew*R^T*g
    matmul_33_33(p_skew, RT, temp41);
    matmul_33_31(temp41, g, temp4);

    // fnl = p_skew*R^T*ddX + w_skew*H*w + w_skew*G*dtheta - p_skew*R^T*g
    for (uint8_t i = 0; i < 3; i++)
    {
        fnl[i] = temp1[i] + temp2[i] + temp3[i] - temp4[i];
    }
}

// 运动增稳控制
void Stable_Control(void)
{
    // // 计算四元数误差的向量部分
    // float e[3];
    // quat_error(wheel_instance->qd, wheel_instance->imu->q, e);

    // // 计算动力学模型中的非线性项
    // float fnl[3];
    // compute_fnl(fnl);

    // 计算欧拉角误差
    float e[3];
    for (uint8_t i = 0; i < 3; i++)
    {
        // e[i] = wheel_instance->euler_d[i] - wheel_instance->imu->euler[i];
				e[i] = wheel_instance->imu->euler[i] - wheel_instance->euler_d[i];
    }
		e[2] = 0; 

    // 计算等效控制输入
    float u[3];
    for (uint8_t i = 0; i < 3; i++)
    {
        // u[i] = Kp[i] * e[i] + Kd[i] * wheel_instance->imu->omega[i] - fnl[i];

        error_sum[i] += e[i];
        // 积分项限幅
        if (error_sum[i] > 20.0f)
            error_sum[i] = 20.0f;
        else if (error_sum[i] < -20.0f)
            error_sum[i] = -20.0f;

        u[i] = Kp[i] * e[i] + Ki[i] * error_sum[i] + Kd[i] * wheel_instance->imu->omega[i];
    }
		u[2] = Kp[2] * e[2] + Ki[2] * error_sum[2] - Kd[2] * wheel_instance->imu->omega[2];

    // 计算动量轮输出转矩
    float temp[4][3];
    matmul_44_43(J, G_inv, temp);
    matmul_43_31(temp, u, wheel_instance->tau);

    // 转矩限幅
    for (uint8_t i = 0; i < 4; i++)
    {
        if (wheel_instance->tau[i] > 4000.0f)
            wheel_instance->tau[i] = 4000.0f;
        else if (wheel_instance->tau[i] < -4000.0f)
            wheel_instance->tau[i] = -4000.0f;
    }

    if (wheel_instance->wheel_mode == WHEEL_MODE_ENABLED)
    {
        // 使能动量轮电机
        HAL_GPIO_WritePin(Motor_Enable_GPIO_Port, Motor_Enable_Pin, GPIO_PIN_SET);
        Wheel_Set_PWM(wheel_instance->tau);
    }
    else
    {
        // 动量轮电机刹车
        HAL_GPIO_WritePin(Motor_Enable_GPIO_Port, Motor_Enable_Pin, GPIO_PIN_RESET);
        for (uint8_t i = 0; i < 4; i++)
        {
            wheel_instance->tau[i] = 0;
        }
        Wheel_Set_PWM(wheel_instance->tau);
    }
}

void Wheel_Set_PWM(float tau[4])
{
    for (uint8_t i = 0; i < 4; i++)
    {
        if (tau[i] < 0)
        {
            HAL_GPIO_WritePin(Motor_Dir_GPIO_Port[i], Motor_Dir_Pin[i], GPIO_PIN_RESET);
            wheel_instance->pwm[i] = (uint16_t)(PWM_MAX + tau[i]);
            __HAL_TIM_SET_COMPARE(&htim8, Motor_PWM_Channel[i], wheel_instance->pwm[i]);
        }
        else
        {
            HAL_GPIO_WritePin(Motor_Dir_GPIO_Port[i], Motor_Dir_Pin[i], GPIO_PIN_SET);
            wheel_instance->pwm[i] = (uint16_t)(PWM_MAX - tau[i]);
            __HAL_TIM_SET_COMPARE(&htim8, Motor_PWM_Channel[i], wheel_instance->pwm[i]);
        }
    }
}

void Wheel_RxCallback(uint8_t *data)
{
    if((data[0] == '$') && (data[1] == 'W'))
    {
        if ((data[2] == 'z') && (data[3] == 'e') && (data[4] == 'r') && (data[5] == 'o'))
            wheel_instance->imu_ZeroFlag = 1;
        if ((data[2] == 's') && (data[3] == 't') && (data[4] == 'a') && (data[5] == 'r') && (data[6] == 't'))
            wheel_instance->wheel_mode = WHEEL_MODE_ENABLED;
        if ((data[2] == 's') && (data[3] == 't') && (data[4] == 'o') && (data[5] == 'p'))
            wheel_instance->wheel_mode = WHEEL_MODE_STOP;
    }
}

void Wheel_Test(void)
{
    // HAL_GPIO_WritePin(Motor_Enable_GPIO_Port, Motor_Enable_Pin, GPIO_PIN_SET);
    // for (uint8_t i = 0; i < 4; i++)
    // {
	// 		wheel_instance->pwm[i] = 7800;
    // 	HAL_GPIO_WritePin(Motor_Dir_GPIO_Port[i], Motor_Dir_Pin[i], GPIO_PIN_SET);
	// 		__HAL_TIM_SET_COMPARE(&htim8, Motor_PWM_Channel[i], wheel_instance->pwm[i]);
    // }
    // vTaskDelay(4000);

    // for (uint8_t i = 0; i < 4; i++)
    // {
    //     __HAL_TIM_SET_COMPARE(&htim8, Motor_PWM_Channel[i], 7500);
    // }
    // vTaskDelay(4000);
    // for (uint8_t i = 0; i < 4; i++)
    // {
    //     __HAL_TIM_SET_COMPARE(&htim8, Motor_PWM_Channel[i], 7000);
    // }
    // vTaskDelay(2000);
    // HAL_GPIO_WritePin(Motor_Enable_GPIO_Port, Motor_Enable_Pin, GPIO_PIN_RESET);
    // vTaskDelay(2000);

    // HAL_GPIO_WritePin(Motor_Enable_GPIO_Port, Motor_Enable_Pin, GPIO_PIN_SET);
    // HAL_GPIO_WritePin(Motor_Dir_GPIO_Port[0], Motor_Dir_Pin[0], GPIO_PIN_RESET);
    // wheel_instance->pwm[3] = 7800;
    // __HAL_TIM_SET_COMPARE(&htim8, Motor_PWM_Channel[3], wheel_instance->pwm[3]);
    // vTaskDelay(2000);


//    __HAL_TIM_SET_COMPARE(&htim8, Motor_PWM_Channel[0], 8250);
//    vTaskDelay(2000);
//    __HAL_TIM_SET_COMPARE(&htim8, Motor_PWM_Channel[0], 8200);
//    vTaskDelay(2000);
//    __HAL_TIM_SET_COMPARE(&htim8, Motor_PWM_Channel[0], 8150);
//    vTaskDelay(2000);
    // __HAL_TIM_SET_COMPARE(&htim8, Motor_PWM_Channel[0], 8200);
    // vTaskDelay(4000);
    // __HAL_TIM_SET_COMPARE(&htim8, Motor_PWM_Channel[0], 7750);
    // vTaskDelay(4000);
    // __HAL_TIM_SET_COMPARE(&htim8, Motor_PWM_Channel[0], 7500);
    // vTaskDelay(4000);
    // __HAL_TIM_SET_COMPARE(&htim8, Motor_PWM_Channel[0], 7250);
    // vTaskDelay(4000);
    // __HAL_TIM_SET_COMPARE(&htim8, Motor_PWM_Channel[0], 7000);
    // vTaskDelay(4000);
    // __HAL_TIM_SET_COMPARE(&htim8, Motor_PWM_Channel[0], 6750);
    // vTaskDelay(4000);
    // __HAL_TIM_SET_COMPARE(&htim8, Motor_PWM_Channel[0], 6500);
    // vTaskDelay(4000);
    // __HAL_TIM_SET_COMPARE(&htim8, Motor_PWM_Channel[0], 6250);
    // vTaskDelay(4000);
    // __HAL_TIM_SET_COMPARE(&htim8, Motor_PWM_Channel[0], 6000);
    // vTaskDelay(4000);
    // HAL_GPIO_WritePin(Motor_Enable_GPIO_Port, Motor_Enable_Pin, GPIO_PIN_RESET);
    // vTaskDelay(2000);

//    wheel_instance->tau[0] = 0.0f;
//    vTaskDelay(4000);
//    wheel_instance->tau[0] = 0.003f;
//    vTaskDelay(2000);
   wheel_instance->tau[0] = 0.006f;
   vTaskDelay(2000);
//    wheel_instance->tau[0] = 0.00f;
//    vTaskDelay(6000);
//    wheel_instance->tau[0] = -0.006f;
//    vTaskDelay(2000);
//    wheel_instance->tau[0] = -0.003f;
//    vTaskDelay(2000);
}

void Wheel_Run_Tau(void)
{
    HAL_GPIO_WritePin(Motor_Enable_GPIO_Port, Motor_Enable_Pin, GPIO_PIN_SET);
    wheel_instance->tau[0] = 0.00f;
    // 计算期望电压 U = E + I*R = Ke*w + (tau/Kt)*Rc
    //float V_fric = 0.454f; // 摩擦电压补偿
    float V_fric = 0.44f; // 摩擦电压补偿
    float E_back = Ke * wheel_instance->wheel_motor[0]->filtered_speed; // 反电动势

    // 根据电机方向补偿摩擦
    if (wheel_instance->wheel_motor[0]->filtered_speed > 0.1f)
        E_back += V_fric;
    else if (wheel_instance->wheel_motor[0]->filtered_speed < -0.1f)
        E_back -= V_fric;
		else if (wheel_instance->tau[0] > 0.0f)
				E_back += V_fric;
		else if (wheel_instance->tau[0] < 0.0f)
				E_back -= V_fric;

    float IR = (wheel_instance->tau[0] / Kt) * Rc;                      // 电阻压降
    float U_need = E_back + IR;

    // 确定电机方向
    if (U_need > 0)
    {
        HAL_GPIO_WritePin(Motor_Dir_GPIO_Port[0], Motor_Dir_Pin[0], GPIO_PIN_SET);
    }
    else
    {
        HAL_GPIO_WritePin(Motor_Dir_GPIO_Port[0], Motor_Dir_Pin[0], GPIO_PIN_RESET);
        U_need = -U_need;
    }

    // 电压限幅
    if (U_need > Vbus)
        U_need = Vbus;

    // 将电压转换为PWM占空比
    float duty = U_need / Vbus;

    wheel_instance->E_back = E_back;
    wheel_instance->IR = IR;
    wheel_instance->U_need = U_need;

    // 转换为PWM，映射关系 duty 0 -> PWM 8200; duty 1 -> 0
    wheel_instance->pwm[0] = (uint16_t)(8399.0f - duty * 8399.0f);

    // PWM限幅
    if (wheel_instance->pwm[0] < 4500)
        wheel_instance->pwm[0] = 4500;

    if (wheel_instance->tau[0] == 0.0035f)
        wheel_instance->pwm[0] = 8399;

    __HAL_TIM_SET_COMPARE(&htim8, Motor_PWM_Channel[0], wheel_instance->pwm[0]);
    //__HAL_TIM_SET_COMPARE(&htim8, Motor_PWM_Channel[0], 7800);
}
