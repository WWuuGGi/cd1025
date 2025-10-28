#ifndef __APP_H__
#define __APP_H__

#include "winch.h"
#include "trajectory.h"

void APP_Layer_Init();
void APP_WinchTask();
void APP_StartTrj();
void APP_UploadData();
void APP_SendPose();

#endif