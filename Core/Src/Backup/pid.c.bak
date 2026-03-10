#include "pid.h"
#include "string.h"
#include "stdlib.h"

PID *PID_Create()
{
    PID *pid = (PID *)malloc(sizeof(PID));
    memset(pid, 0, sizeof(PID));
    return pid;
}

void PID_Calc(PID *pid)
{
    pid->error[2] = pid->error[1];
    pid->error[1] = pid->error[0];
    pid->error[0] = pid->ref - pid->fdb;

    if(pid->pid_mode == PID_POSITION)   //位置式PID
    {
        pid->error_sum += pid->error[0];
        // 积分上限判断
        if(pid->error_sum > pid->error_max) pid->error_sum = pid->error_max;
        if(pid->error_sum < -pid->error_max) pid->error_sum = -pid->error_max;

        pid->output = pid->KP * pid->error[0] + pid->KI * pid->error_sum + pid->KD * (pid->error[0] - pid->error[1]);
    }
    else if(pid->pid_mode == PID_DELTA)
    {
        pid->output += pid->KP * (pid->error[0] - pid->error[1]) + pid->KI * (pid->error[0] - 2.0f * pid->error[1] + pid->error[2]) + pid->KD * pid->error[0];
    }

    // 输出上限
    if(pid->output > pid->output_max) pid->output = pid->output_max;
    if(pid->output < -pid->output_max) pid->output = -pid->output_max;
}

void PID_Set(PID *pid, float KP, float KI, float KD, float error_max, float output_max, PID_Mode pid_mode)
{
    pid->KP = KP;
    pid->KI = KI;
    pid->KD = KD;
    pid->error_max = error_max;
    pid->output_max = output_max;
    pid->pid_mode = pid_mode;
}