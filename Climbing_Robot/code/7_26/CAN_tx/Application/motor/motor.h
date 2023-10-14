#ifndef __MOTOR_H__
#define __MOTOR_H__

//extern CAN_HandleTypeDef hcan1;
#include "stm32f4xx_hal.h"



void Send_Motor(CAN_HandleTypeDef *hcan, uint16_t motor1, uint16_t motor2, uint16_t motor3, uint16_t motor4, uint32_t stdId, uint32_t extId, uint32_t iDE, uint32_t rTR, uint32_t dLC);

#endif

