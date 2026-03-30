#ifndef __APP_H__
#define __APP_H__

#include "winch.h"
#include "trajectory.h"

void APP_Layer_Init(void);
void APP_WinchTask(void);
void APP_StartTrj(void);
void APP_UploadData(void);
void APP_SendPose(void);
void APP_WheelTask(void);

#endif