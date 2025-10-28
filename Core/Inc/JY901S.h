#ifndef __JY901S_H__
#define __JY901S_H__

#include "bsp_uart.h"

typedef struct JY901S_t
{
    float angleX;
    float angleY;
    float angleZ;
    float velocityX;
    float velocityY;
    float velocityZ;
} JY901S;

void JY901S_Init();
JY901S* JY901S_Create();
void JY901S_AccCali();
void JY901S_SetZero();
void JY901S_RxCallback(uint8_t *data);

#endif