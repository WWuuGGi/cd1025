#include "app.h"


Winch *winch;
uint8_t upload_data[43];
uint8_t pose_data[8];


void APP_Layer_Init()
{
    Winch_Drvier_Init();
    winch = Winch_Create();
    Winch_SetPID(winch);
}

void APP_WinchTask()
{
    Winch_Calc_Output(winch);
    Winch_Send_Current();
    Winch_Pose_SetZero();
}

void APP_StartTrj()
{
    if(winch->mode == Winch_mode_T || winch->mode == Winch_mode_SC)
        StartTrj(winch);
    else
        return;
}

void APP_UploadData()
{
    upload_data[0] = 0xAA;
    upload_data[1] = 0x55;

    //姿态
    upload_data[2] = ((int32_t)(winch->pose->angleX * 1000.0f)) & 0x000000FF;
    upload_data[3] = (((int32_t)(winch->pose->angleX * 1000.0f)) & 0x0000FF00) >> 8;
    upload_data[4] = (((int32_t)(winch->pose->angleX * 1000.0f)) & 0x00FF0000) >> 16;
    upload_data[5] = ((int32_t)(winch->pose->angleX * 1000.0f)) >> 24;

    upload_data[6] = ((int32_t)(winch->pose->angleY * 1000.0f)) & 0x000000FF;
    upload_data[7] = (((int32_t)(winch->pose->angleY * 1000.0f)) & 0x0000FF00) >> 8;
    upload_data[8] = (((int32_t)(winch->pose->angleY * 1000.0f)) & 0x00FF0000) >> 16;
    upload_data[9] = ((int32_t)(winch->pose->angleY * 1000.0f)) >> 24;

    upload_data[10] = ((int32_t)(winch->pose->angleZ * 1000.0f)) & 0x000000FF;
    upload_data[11] = (((int32_t)(winch->pose->angleZ * 1000.0f)) & 0x0000FF00) >> 8;
    upload_data[12] = (((int32_t)(winch->pose->angleZ * 1000.0f)) & 0x00FF0000) >> 16;
    upload_data[13] = ((int32_t)(winch->pose->angleZ * 1000.0f)) >> 24;

    //电机转角
    upload_data[14] = ((int32_t)(winch->CAN1_M2006[0]->now_pos * 1000.0f)) & 0x000000FF;
    upload_data[15] = (((int32_t)(winch->CAN1_M2006[0]->now_pos * 1000.0f)) & 0x0000FF00) >> 8;
    upload_data[16] = (((int32_t)(winch->CAN1_M2006[0]->now_pos * 1000.0f)) & 0x00FF0000) >> 16;
    upload_data[17] = ((int32_t)(winch->CAN1_M2006[0]->now_pos * 1000.0f)) >> 24;

    upload_data[18] = ((int32_t)(winch->CAN1_M2006[1]->now_pos * 1000.0f)) & 0x000000FF;
    upload_data[19] = (((int32_t)(winch->CAN1_M2006[1]->now_pos * 1000.0f)) & 0x0000FF00) >> 8;
    upload_data[20] = (((int32_t)(winch->CAN1_M2006[1]->now_pos * 1000.0f)) & 0x00FF0000) >> 16;
    upload_data[21] = ((int32_t)(winch->CAN1_M2006[1]->now_pos * 1000.0f)) >> 24;

    upload_data[22] = ((int32_t)(winch->CAN1_M2006[2]->now_pos * 1000.0f)) & 0x000000FF;
    upload_data[23] = (((int32_t)(winch->CAN1_M2006[2]->now_pos * 1000.0f)) & 0x0000FF00) >> 8;
    upload_data[24] = (((int32_t)(winch->CAN1_M2006[2]->now_pos * 1000.0f)) & 0x00FF0000) >> 16;
    upload_data[25] = ((int32_t)(winch->CAN1_M2006[2]->now_pos * 1000.0f)) >> 24;

    upload_data[26] = ((int32_t)(winch->CAN1_M2006[3]->now_pos * 1000.0f)) & 0x000000FF;
    upload_data[27] = (((int32_t)(winch->CAN1_M2006[3]->now_pos * 1000.0f)) & 0x0000FF00) >> 8;
    upload_data[28] = (((int32_t)(winch->CAN1_M2006[3]->now_pos * 1000.0f)) & 0x00FF0000) >> 16;
    upload_data[29] = ((int32_t)(winch->CAN1_M2006[3]->now_pos * 1000.0f)) >> 24;

    //姿态角速度
    upload_data[30] = ((int32_t)(winch->pose->velocityX * 1000.0f)) & 0x000000FF;
    upload_data[31] = (((int32_t)(winch->pose->velocityX * 1000.0f)) & 0x0000FF00) >> 8;
    upload_data[32] = (((int32_t)(winch->pose->velocityX * 1000.0f)) & 0x00FF0000) >> 16;
    upload_data[33] = ((int32_t)(winch->pose->velocityX * 1000.0f)) >> 24;

    upload_data[34] = ((int32_t)(winch->pose->velocityY * 1000.0f)) & 0x000000FF;
    upload_data[35] = (((int32_t)(winch->pose->velocityY * 1000.0f)) & 0x0000FF00) >> 8;
    upload_data[36] = (((int32_t)(winch->pose->velocityY * 1000.0f)) & 0x00FF0000) >> 16;
    upload_data[37] = ((int32_t)(winch->pose->velocityY * 1000.0f)) >> 24;

    upload_data[38] = ((int32_t)(winch->pose->velocityZ * 1000.0f)) & 0x000000FF;
    upload_data[39] = (((int32_t)(winch->pose->velocityZ * 1000.0f)) & 0x0000FF00) >> 8;
    upload_data[40] = (((int32_t)(winch->pose->velocityZ * 1000.0f)) & 0x00FF0000) >> 16;
    upload_data[41] = ((int32_t)(winch->pose->velocityZ * 1000.0f)) >> 24;

    upload_data[42] = winch->trj_index;

    BSP_UART_Send_DMA(PC_UART, upload_data, sizeof(upload_data));
}

void APP_SendPose()
{
    pose_data[0] = ((int32_t)(winch->poseX_ref * 1000.0f)) & 0x000000FF;
    pose_data[1] = (((int32_t)(winch->poseX_ref * 1000.0f)) & 0x0000FF00) >> 8;
    pose_data[2] = (((int32_t)(winch->poseX_ref * 1000.0f)) & 0x00FF0000) >> 16;
    pose_data[3] = ((int32_t)(winch->poseX_ref * 1000.0f)) >> 24;

    pose_data[4] = ((int32_t)(winch->poseY_ref * 1000.0f)) & 0x000000FF;
    pose_data[5] = (((int32_t)(winch->poseY_ref * 1000.0f)) & 0x0000FF00) >> 8;
    pose_data[6] = (((int32_t)(winch->poseY_ref * 1000.0f)) & 0x00FF0000) >> 16;
    pose_data[7] = ((int32_t)(winch->poseY_ref * 1000.0f)) >> 24;

    BSP_UART_Send_DMA(WHEEL_UART, pose_data, sizeof(pose_data));
}