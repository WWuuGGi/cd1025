#ifndef __BSP_CAN_H__
#define __BSP_CAN_H__

#include "can.h"

void CanFilterInit();
void BSP_CAN_Transmit(CAN_HandleTypeDef *hcan, int16_t cm1_iq, int16_t cm2_iq, int16_t cm3_iq, int16_t cm4_iq);
void BSP_CAN_Transmit_58(CAN_HandleTypeDef *hcan, int16_t cm1_iq, int16_t cm2_iq, int16_t cm3_iq, int16_t cm4_iq);

#endif