#include "winch.h"
#include "gripper.h"
#include "bsp_can.h"

Winch *winch_instance;
int winch_motor_output[5] = {0};

void Winch_Drvier_Init()
{
    memset(&winch_instance, 0, sizeof(winch_instance));
}

Winch *Winch_Create()
{
    Winch *obj = (Winch *)malloc(sizeof(Winch));
    memset(obj, 0, sizeof(Winch));

    for(uint8_t i =0; i <5; i++)
    {
        obj->CAN1_M2006[i] = CAN1_Motor_Create(i);
        obj->M2006_speed_pid[i] = PID_Create();
        obj->M2006_position_pid[i] = PID_Create();
        // obj->pose = JY901S_Create();
    }
    obj->pose = JY901S_Create();
    winch_instance = obj;
    return obj;
}

void Winch_SetPID(Winch *winch)
{
    PID_Set(winch->M2006_speed_pid[0], 1, 0, 5, 0, 16384, PID_POSITION);
    PID_Set(winch->M2006_position_pid[0], 120, 2, 20, 2000, 16384, PID_POSITION);
	// 120 2 15 2000 16384

    PID_Set(winch->M2006_speed_pid[1], 1, 0, 5, 0, 16384, PID_POSITION);
    PID_Set(winch->M2006_position_pid[1], 120, 2, 20, 2000, 16384, PID_POSITION);

    PID_Set(winch->M2006_speed_pid[2], 1, 0, 5, 0, 16384, PID_POSITION);
    PID_Set(winch->M2006_position_pid[2], 120, 2, 20, 2000, 16384, PID_POSITION);

    PID_Set(winch->M2006_speed_pid[3], 1, 0, 5, 0, 16384, PID_POSITION);
    PID_Set(winch->M2006_position_pid[3], 120, 2, 20, 2000, 16384, PID_POSITION);

    PID_Set(winch->M2006_speed_pid[4], 1, 0, 5, 0, 16384, PID_POSITION);
    PID_Set(winch->M2006_position_pid[4], 120, 2, 20, 2000, 16384, PID_POSITION);
}

void Winch_RxCallback(uint8_t *data)
{
    if((data[0] == 'P') && (data[1] == 'o') && (data[2] == 's') && (data[3] == 'e'))
        winch_instance->pose_ZeroFlag = 1;

    if((data[0] == 'C') &&(data[1] == 'T') && (data[2] == 'M') && (data[3] == 'o') && (data[4] == 'd') && (data[5] == 'e'))
    {
        winch_instance->mode = Winch_mode_CT;
        winch_instance->ct_flag = 0;
    }

    if((data[0] == 'S') && (data[1] == 'M') && (data[2] == 'o') && (data[3] == 'd') && (data[4] == 'e'))
    {
        winch_instance->mode = Winch_mode_S;
    }

    if((data[0] == 'T') && (data[1] == 'M') && (data[2] == 'o') && (data[3] == 'd') && (data[4] == 'e'))
    {
        winch_instance->mode = Winch_mode_T;
        //获取初始位置
        for (uint8_t i = 0; i < 4; i++)
        {
            winch_instance->CAN1_M2006[i]->init_pos = winch_instance->CAN1_M2006[i]->realPos * 360.0f / 8192.0f / 36.0f;
        }
    }

    if((data[0] == 'S') && (data[1] == 'C') && (data[2] == 'M') && (data[3] == 'o') && (data[4] == 'd') && (data[5] == 'e'))
    {
        // 切换到自标定模式
        winch_instance->mode = Winch_mode_SC;
        winch_instance->sc_current_point = 0;  // 重置标定点索引
        winch_instance->trj_CpltFlag = 0;  // 重置完成标志
        winch_instance->sc_running = 0;
        winch_instance->sc_timer = 0;
        for (uint8_t i = 0; i < 5; i++)
        {
            winch_instance->CAN1_M2006[i]->init_pos = winch_instance->CAN1_M2006[i]->realPos * 360.0f / 8192.0f / 36.0f;
        }
    }

    if((data[0] == 'L') && (data[1] == 'I') && (data[2] == 'F') && (data[3] == 'T'))
    {
        // 切换到平台升降模式
        winch_instance->mode = Winch_mode_Lift;
        winch_instance->trj_CpltFlag = 0;  // 重置完成标志
        for (uint8_t i = 0; i < 5; i++)
        {
            winch_instance->CAN1_M2006[i]->init_pos = winch_instance->CAN1_M2006[i]->realPos * 360.0f / 8192.0f / 36.0f;
        }
    }


    if((data[0] == '#') && (data[1] == 'S') && (data[2] == 'M'))
    {
        uint8_t M2006_index = data[3];
        if(data[4] == 0x00)
            winch_instance->CAN1_M2006[M2006_index-1]->ref_speed = (float)(((uint16_t)data[5] << 8) | data[6]);
        else if(data[4] == 0x01)
            winch_instance->CAN1_M2006[M2006_index-1]->ref_speed = -(float)(((uint16_t)data[5] << 8) | data[6]);
    }

    if((data[0] == '#') && (data[1] == 'T'))
    {
        if((data[2] == 's') && (data[3] == 't') && (data[4] == 'o') && (data[5] == 'p'))
        {
            winch_instance->trj = Winch_Trj_stop;
            winch_instance->trj_index = 0;
        }
        else if((data[2] == 'r') && (data[3] == 'i') && (data[4] == 's') && (data[5] == 'e'))
        {
            winch_instance->trj = Winch_Trj_rise;
            winch_instance->trj_index = 1;
        }
        else if((data[2] == 'l') && (data[3] == 'i') && (data[4] == 'n') && (data[5] == 'e'))
        {
            winch_instance->trj = Winch_Trj_line1;
            winch_instance->trj_index = 2;
        }
        else if((data[2] == 'p') && (data[3] == 'o') && (data[4] == 'l') && (data[5] == 'y'))
        {
            winch_instance->trj = Winch_Trj_line2;
            winch_instance->trj_index = 3;
        }
        else if((data[2] == 'c') && (data[3] == 'i') && (data[4] == 'r') && (data[5] == 'c') && (data[6] == 'l') && (data[7] == 'e'))
        {
            winch_instance->trj = Winch_Trj_line3;
            winch_instance->trj_index = 4;
        }
        else if((data[2] == 'f') && (data[3] == 'a') && (data[4] == 'l') && (data[5] == 'l'))
        {
            winch_instance->trj = Winch_Trj_fall;
            winch_instance->trj_index = 5;
        }
        // 添加自标定模式命令处理
        else if((data[2] == 's') && (data[3] == 'c') && (data[4] == 'm') && (data[5] == 'o') && (data[6] == 'd') && (data[7] == 'e'))
        {
            // 只有在自标定模式下才处理此命令
            winch_instance->trj = Winch_Trj_SC;
            winch_instance->sc_running = 1;
            winch_instance->sc_timer = 0;
            winch_instance->trj_index = 6;
        }
        else if((data[2] == 'u') && (data[3] == 'p'))
        {
            winch_instance->trj = Winch_Trj_up;
            winch_instance->trj_index = 7;
        }
        else if((data[2] == 'd') && (data[3] == 'o') && (data[4] == 'w') && (data[5] == 'n'))
        {
            winch_instance->trj = Winch_Trj_down;
            winch_instance->trj_index = 8;
        }
    }
    //测量实际绳长的相关控制分类
    if((data[0] == '#') && (data[1] == 'C') && (data[2] == 'T'))
    {
        if(data[3] == '1')
        {
            winch_instance->ct_flag = 1;
        }
        else if(data[3] == '2')
        {
            winch_instance->ct_flag = 2;
        }
        else if(data[3] == '3')
        {
            winch_instance->ct_flag = 3;
        }
        else if(data[3] == '4')
        {
            winch_instance->ct_flag = 4;
        }
        else if(data[3] == '5')
        {
            winch_instance->ct_flag = 5;
        }
        else if(data[3] == '0')
        {
            winch_instance->ct_flag = 0;
        }
        
    }

    if((data[0] == 'G') && (data[1] == 'r') && (data[2] == 'a') && (data[3] == 'b'))
    {
        Gripper_grab();
    }

    if((data[0] == 'L') && (data[1] == 'o') && (data[2] == 'o') && (data[3] == 's') && (data[4] == 'e'))
    {
        Gripper_loose();
    }

    // // 升降平台模式控制命令
    // if((data[0] == 'L') && (data[1] == 'M') && (data[2] == 'o') && (data[3] == 'd') && (data[4] == 'e'))
    // {
    //     winch_instance->mode = Winch_mode_Lift;
    //     winch_instance->lift_running = 0;
    // }

    // // 升降平台目标位置设置
    // if((data[0] == '#') && (data[1] == 'L') && (data[2] == 'i') && (data[3] == 'f') && (data[4] == 't'))
    // {
    //     // 解析目标位置（圈数）和方向
    //     float target_pos = (float)((uint16_t)data[5] << 8 | data[6]) / 100.0f; // 范围：0-100.0圈
    //     uint8_t direction = data[7]; // 0:上升, 1:下降
        
    //     winch_instance->lift_ref_pos = target_pos;
    //     winch_instance->lift_direction = direction;
    //     winch_instance->lift_running = 1;
    // }

    // // 升降平台停止命令
    // if((data[0] == '#') && (data[1] == 'L') && (data[2] == 's') && (data[3] == 't') && (data[4] == 'o') && (data[5] == 'p'))
    // {
    //     winch_instance->lift_running = 0;
    // }
}

// 绞盘运动模式判断，计算电机电流
void Winch_Calc_Output(Winch *winch)
{
    switch (winch->mode)
    {
    case Winch_mode_stop:
        memset(winch_motor_output, 0, sizeof(winch_motor_output));
        break;
    case Winch_mode_CT:
        Winch_ConstantTorque(winch);
        break;
    case Winch_mode_S:
        Winch_SingleMove(winch);
        break;
    case Winch_mode_T:
        Winch_Trajectory(winch);
        break;
    case Winch_mode_SC:
        Winch_SelfCalibration(winch);
        break;
    case Winch_mode_Lift:
        Winch_LiftControl(winch);
        break;
    default:
        break;
    }
}

//恒定力矩模式，用于初始抽绳
void Winch_ConstantTorque(Winch *winch)
{
    switch(winch->ct_flag)
    {
        //正负有待确认调整
        case 0:
            winch_motor_output[0] = 0;
            winch_motor_output[1] = 0;
            winch_motor_output[2] = 0;
            winch_motor_output[3] = 0;
            winch_motor_output[4] = 0;
            break;
        case 1:
            winch_motor_output[0] = 500;
            winch_motor_output[1] = 0;
            winch_motor_output[2] = 0;
            winch_motor_output[3] = 0;
            winch_motor_output[4] = 0;
            break;
        case 2:
            winch_motor_output[0] = 0;
            winch_motor_output[1] = 500;
            winch_motor_output[2] = 0;
            winch_motor_output[3] = 0;
            winch_motor_output[4] = 0;
            break;
        case 3:
            winch_motor_output[0] = 0;
            winch_motor_output[1] = 0;
            winch_motor_output[2] = 500;
            winch_motor_output[3] = 0;
            winch_motor_output[4] = 0;
            break;
        case 4:
            winch_motor_output[0] = 0;
            winch_motor_output[1] = 0;
            winch_motor_output[2] = 0;
            winch_motor_output[3] = 500;
            winch_motor_output[4] = 0;
            break;
        case 5:
            winch_motor_output[0] = 500;
            winch_motor_output[1] = 500;
            winch_motor_output[2] = 500;
            winch_motor_output[3] = 500;    
            winch_motor_output[4] = 0;
            break;
        default:
            winch_motor_output[0] = 0;
            winch_motor_output[1] = 0;
            winch_motor_output[2] = 0;
            winch_motor_output[3] = 0;
            winch_motor_output[4] = 0;
            break;

    }
}

// 单关节点动模式，速度环
void Winch_SingleMove(Winch *winch)
{
    for(uint8_t i = 0; i < 5; i++)
    {
        winch->M2006_speed_pid[i]->ref = winch->CAN1_M2006[i]->ref_speed;
        winch->M2006_speed_pid[i]->fdb = winch->CAN1_M2006[i]->fdbSpeed;
        PID_Calc(winch->M2006_speed_pid[i]);
        winch_motor_output[i] = winch->M2006_speed_pid[i]->output;
    }
}

// 轨迹规划模式，位置环+速度环
void Winch_Trajectory(Winch *winch)
{
    for(uint8_t i = 0; i < 4; i++)
    {
        winch->M2006_position_pid[i]->ref = winch->CAN1_M2006[i]->ref_pos;
        winch->M2006_position_pid[i]->fdb = winch->CAN1_M2006[i]->realPos * 360.0f / 8192.0f / 36.0f;
        PID_Calc(winch->M2006_position_pid[i]);
        winch->M2006_speed_pid[i]->ref = winch->M2006_position_pid[i]->output;
        winch->M2006_speed_pid[i]->fdb = winch->CAN1_M2006[i]->fdbSpeed;
        PID_Calc(winch->M2006_speed_pid[i]);
        winch_motor_output[i] = winch->M2006_speed_pid[i]->output;
    }
    winch_motor_output[4] = 0;
}

void Winch_Send_Current()
{
    BSP_CAN_Transmit(&hcan1, winch_motor_output[0], winch_motor_output[1], winch_motor_output[2], winch_motor_output[3]);
    BSP_CAN_Transmit_58(&hcan1, winch_motor_output[4], 0, 0, 0);
}

void Winch_Pose_SetZero()
{
    if(winch_instance->pose_ZeroFlag)
    {
        JY901S_SetZero();
        winch_instance->pose_ZeroFlag = 0;
    }
}

// 自标定模式处理函数 - 修复逻辑
void Winch_SelfCalibration(Winch *winch)
{
    uint8_t fc_rope = 3;
    // 位置环+速度环控制
    for(uint8_t i = 0; i < 4; i++)
    {
        winch->M2006_position_pid[i]->ref = winch->CAN1_M2006[i]->ref_pos;
        winch->M2006_position_pid[i]->fdb = winch->CAN1_M2006[i]->realPos * 360.0f / 8192.0f / 36.0f;
        PID_Calc(winch->M2006_position_pid[i]);
        winch->M2006_speed_pid[i]->ref = winch->M2006_position_pid[i]->output;
        winch->M2006_speed_pid[i]->fdb = winch->CAN1_M2006[i]->fdbSpeed;
        PID_Calc(winch->M2006_speed_pid[i]);
        winch_motor_output[i] = winch->M2006_speed_pid[i]->output;
        //先全部置零，实验先大电流后小电流的执行效果

    }
    winch_motor_output[fc_rope] = 500;
    winch_motor_output[4] = 0;

    
    // // 初始化自标定相关参数（仅在首次进入模式时）
    // if(winch->sc_running && winch->sc_timer == 0)
    // {
    //     winch->sc_timer = HAL_GetTick(); // 记录开始时间
    //     winch->sc_current_stage = 0; // 初始为大电流阶段
    //     winch->sc_high_current = 500; // 设置大电流值（可根据实际需求调整）
    //     winch->sc_low_current = 180;   // 设置小电流值（可根据实际需求调整）
    // }
    
    // // 检查是否需要切换电流阶段（假设大电流阶段持续1000ms）
    // if(winch->sc_running && winch->sc_current_stage == 0 && (HAL_GetTick() - winch->sc_timer) > 1000)
    // {
    //     winch->sc_current_stage = 1; // 切换到小电流阶段
    // }
    
    // // 力控逻辑：根据当前阶段设置电流值
    // if(winch->sc_running)
    // {
    //     int16_t current_value;
        
    //     // 根据当前阶段选择电流值
    //     if(winch->sc_current_stage == 0)
    //     {
    //         current_value = winch->sc_high_current; // 大电流克服静摩擦力
    //     }
    //     else
    //     {
    //         current_value = winch->sc_low_current;  // 小电流维持张紧状态
    //     }
        
    //         winch_motor_output[fc_rope] = current_value;
    // }
    // else
    // {
    //     // 如果自标定模式未运行，重置计时器
    //     winch->sc_timer = 0;
    //     winch->sc_current_stage = 0;
    // }
  
}

// 升降平台模式控制函数
void Winch_LiftControl(Winch *winch)
{
        // 计算当前位置（圈数）
        // winch->lift_current_pos = winch->CAN1_M2006[4]->realPos * 360.0f / 8192.0f / 36.0f;
        
        // 位置环控制
        winch->M2006_position_pid[4]->ref = winch->lift_ref_pos;
        winch->M2006_position_pid[4]->fdb = winch->CAN1_M2006[4]->realPos * 360.0f / 8192.0f / 36.0f;
        //winch->M2006_position_pid[4]->fdb = winch->lift_current_pos;
        PID_Calc(winch->M2006_position_pid[4]);  
        // 速度环控制
        winch->M2006_speed_pid[4]->ref = winch->M2006_position_pid[4]->output;
        winch->M2006_speed_pid[4]->fdb = winch->CAN1_M2006[4]->fdbSpeed;
        PID_Calc(winch->M2006_speed_pid[4]);
        // 设置电机输出
        winch_motor_output[4] = winch->M2006_speed_pid[4]->output;
        
        // 其他电机保持静止
        winch_motor_output[0] = 0;
        winch_motor_output[1] = 0;
        winch_motor_output[2] = 0;
        winch_motor_output[3] = 0;


}