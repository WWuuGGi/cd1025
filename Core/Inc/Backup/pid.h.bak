#ifndef __PID_H__
#define __PID_H__

typedef enum PID_Mode_e
{
    PID_POSITION = 0,
    PID_DELTA
} PID_Mode;

typedef struct PID_t
{
    float KP;
    float KD;
    float KI;
    float error[3];
    float error_sum;
    float error_max;
    float fdb;
    float ref;
    float output;
    float output_max;
    PID_Mode pid_mode;
} PID;

PID *PID_Create();
void PID_Calc(PID *pid);
void PID_Set(PID *pid, float KP, float KI, float KD, float error_max, float output_max, PID_Mode pid_mode);

#endif