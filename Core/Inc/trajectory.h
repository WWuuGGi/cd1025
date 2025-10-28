#ifndef __TRAJECTORY_H__
#define __TRAJECTORY_H__

#include "winch.h"

void StartTrj(Winch *winch);
void Winch_Stop(Winch *winch);
void Winch_Start_Rise(Winch *winch);
void Winch_Start_Line(Winch *winch);
void Winch_Start_Line1(Winch *winch);
void Winch_Start_Line2(Winch *winch);
void Winch_Start_Line3(Winch *winch);
void Winch_Start_Circle1(Winch *winch);
void Winch_Start_Circle2(Winch *winch);
void Winch_Start_Circle3(Winch *winch);
void Winch_Start_Swim1(Winch *winch);
void Winch_Start_Swim2(Winch *winch);
void Winch_Start_Swim3(Winch *winch);
void Winch_Start_Poly(Winch *winch);
void Winch_Start_Fall(Winch *winch);
void Winch_Run_Step(Winch *winch, uint16_t t_num, float time_step, float (*motor_angle)[t_num], float (*pose_ref)[t_num]);

// 自标定模式相关函数声明
void Winch_Start_SelfCalibration(Winch *winch);
void Generate_SelfCalibration_Trajectory(float current_pos[4], float target_pos[4], float trajectory[][61]);
#endif