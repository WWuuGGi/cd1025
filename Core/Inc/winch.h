#ifndef __WINCH_H__
#define __WINCH_H__

#include "bsp_motor.h"
#include "pid.h"
#include "JY901S.h"

typedef enum Winch_mode_t
{
    Winch_mode_stop = 0,    //停止运动
    Winch_mode_CT,          //恒定力矩模式
    Winch_mode_S,           //单关节点动模式
    Winch_mode_T,           //轨迹规划模式
    Winch_mode_SC,          //自标定模式
    Winch_mode_Lift         //升降平台模式
} Winch_mode;

typedef enum Winch_trj_t
{
    Winch_Trj_stop = 0,     //停止运动
    Winch_Trj_rise,         //上升轨迹
    Winch_Trj_line,         //直线轨迹
    Winch_Trj_line1,
    Winch_Trj_line2,
    Winch_Trj_line3,
    Winch_Trj_poly,         //折线轨迹
    Winch_Trj_circle,       //圆形轨迹
    Winch_Trj_helix,        //螺旋线轨迹
    Winch_Trj_fall,         //下降轨迹
    Winch_Trj_SC,            //自标定轨迹
    Winch_Trj_up,           //平台上升轨迹
    Winch_Trj_down          //平台下降轨迹
} Winch_trj;

typedef struct winch_t
{
    CAN_Motor *CAN1_M2006[5];
    PID *M2006_speed_pid[5];
    PID *M2006_position_pid[5];

    JY901S *pose;           //机器人姿态
    uint8_t pose_ZeroFlag;  //用于姿态角度置零的标志

    Winch_mode mode;        //当前运动模式
    Winch_trj trj;          //当前运行轨迹
    Winch_trj last_trj;     //上次运行轨迹
    uint8_t trj_CpltFlag;   //轨迹完成标志
    uint8_t trj_index;      //当前轨迹标识

    float poseX_ref;        //当前X轴平衡倾角
    float poseY_ref;        //当前Y轴平衡倾角

    // 自标定模式相关变量
    uint8_t sc_current_point; //当前标定点索引
    uint8_t sc_running;
    int sc_timer;      //自标定模式计时器，用于控制电流变化
    uint8_t sc_current_stage; //电流控制阶段 (0:大电流启动, 1:小电流维持)
    int16_t sc_high_current; //高电流值，用于克服静摩擦力
    int16_t sc_low_current;  //低电流值，用于维持张紧状态
    
    uint8_t ct_flag;        //恒定力矩模式标志


} Winch;

void Winch_Drvier_Init();
Winch *Winch_Create();
void Winch_SetPID(Winch *winch);
void Winch_RxCallback(uint8_t *data);
void Winch_Calc_Output(Winch *winch);
void Winch_ConstantTorque(Winch *winch);
void Winch_SingleMove(Winch *winch);
void Winch_Trajectory(Winch *winch);
void Winch_Send_Current();
void Winch_Pose_SetZero();

// 自标定模式相关函数声明
void Winch_SelfCalibration(Winch *winch);

// 升降平台模式相关函数声明
void Winch_LiftControl(Winch *winch);

#endif