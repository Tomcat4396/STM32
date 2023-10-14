#ifndef CAN_H
#define CAN_H


#include "stm32f4xx_hal.h"

extern CAN_HandleTypeDef hcan1;

extern void Error_Handler(void);

void MX_CAN1_Init(void);



























#endif
