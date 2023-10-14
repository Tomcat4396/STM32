#ifndef __MOTOR_H__
#define __MOTOR_H__

//extern CAN_HandleTypeDef hcan1;
#include "sys.h"
#include "usart.h"
#include "can.h"
#include "stm32f4xx_hal.h"
#include "stm32f4xx_hal_can.h"

void Send_Motor(CAN_HandleTypeDef *hcan, int16_t motor1, int16_t motor2, int16_t motor3, int16_t motor4, uint32_t stdId, uint32_t extId, uint32_t iDE, uint32_t rTR, uint32_t dLC);
void Receive_Motor(CAN_HandleTypeDef *hcan);



typedef enum
{
    CAN_CHASSIS_ALL_ID = 0x200,
    CAN_3508_M1_ID = 0x201,
    CAN_3508_M2_ID = 0x202,
    CAN_3508_M3_ID = 0x203,
    CAN_3508_M4_ID = 0x204,

    CAN_YAW_MOTOR_ID = 0x205,
    CAN_PIT_MOTOR_ID = 0x206,
    CAN_TRIGGER_MOTOR_ID = 0x207,
    CAN_GIMBAL_ALL_ID = 0x1FF,

}can_msg_id_e;


typedef struct
{
    uint16_t ecd;
    int16_t speed_rpm;
    int16_t given_current;
    uint8_t temperate;
    int16_t last_ecd;
} motor_measure_t;



#endif

