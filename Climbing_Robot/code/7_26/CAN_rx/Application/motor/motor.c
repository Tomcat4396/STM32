#include "motor.h"
#include "can.h"
#include "usart.h"
#include "stm32f4xx_hal.h"
#include "stm32f4xx_hal_can.h"

motor_measure_t motor_chassis[7];

//motor data read
#define get_motor_measure(ptr, data)                                    \
    {                                                                   \
        (ptr)->last_ecd = (ptr)->ecd;                                   \
        (ptr)->ecd = (uint16_t)((data)[0] << 8 | (data)[1]);            \
        (ptr)->speed_rpm = (uint16_t)((data)[2] << 8 | (data)[3]);      \
        (ptr)->given_current = (uint16_t)((data)[4] << 8 | (data)[5]);  \
        (ptr)->temperate = (data)[6];                                   \
    }



void Send_Motor(CAN_HandleTypeDef *hcan, uint16_t motor1, uint16_t motor2, uint16_t motor3, uint16_t motor4, 
								uint32_t stdId, uint32_t extId, uint32_t iDE, uint32_t rTR, uint32_t dLC)
{
		//设置一个随机邮箱地址
		uint32_t send_mail_box;
		uint8_t	motor_array[8] = {0};
//		motor_tx.StdId = 0x200;
//    motor_tx.IDE = CAN_ID_STD;
//		motor_tx.ExtId=0x12;
//    motor_tx.RTR = CAN_RTR_DATA;
//    motor_tx.DLC = 0x08;
	
		CAN_TxHeaderTypeDef motor_tx;
	  //设置第一组电机的ID信息。（前四个电机）
		motor_tx.StdId = stdId;
    motor_tx.IDE = iDE;
		motor_tx.ExtId = extId;
    motor_tx.RTR = rTR;
    motor_tx.DLC = dLC;
	
	
		//赋予四个电机电流值
    motor_array[0] = motor1 >> 8;
    motor_array[1] = motor1;
    motor_array[2] = motor2 >> 8;
    motor_array[3] = motor2;
    motor_array[4] = motor3 >> 8;
    motor_array[5] = motor3;
    motor_array[6] = motor4 >> 8;
    motor_array[7] = motor4;
		
		if(HAL_CAN_AddTxMessage(hcan, &motor_tx, motor_array, &send_mail_box) != HAL_OK)
		{
//				printf("电流发送成功");
		}
}


//接收数据
void Receive_Motor(CAN_HandleTypeDef *hcan)
{
		CAN_RxHeaderTypeDef rx_header;
    uint8_t rx_data[8];

    HAL_CAN_GetRxMessage(hcan, CAN_RX_FIFO0, &rx_header, rx_data);
		printf("Receive is ok \r\n");
	
		
		static uint8_t i = 0;
    //get motor id
    i = rx_header.StdId - CAN_3508_M1_ID;
    get_motor_measure(&motor_chassis[i], rx_data);
		printf("speed is %d \r\n", motor_chassis[i].speed_rpm);
	
    switch (rx_header.StdId)
    {
        case CAN_3508_M1_ID:
        {
            static uint8_t i = 0;
            //get motor id
            i = rx_header.StdId - CAN_3508_M1_ID;
            get_motor_measure(&motor_chassis[i], rx_data);
						printf("speed is %d \r\n", motor_chassis[i].speed_rpm);
            break;
        }

        default:
        {
            break;
        }
    }
	
}


//CAN中断回调函数
void HAL_CAN_RxFifo0MsgPendingCallback(CAN_HandleTypeDef *hcan)
{
    CAN_RxHeaderTypeDef rx_header;
    uint8_t rx_data[8];

    HAL_CAN_GetRxMessage(hcan, CAN_RX_FIFO0, &rx_header, rx_data);
		printf("callback is ok \r\n");
		
    switch (rx_header.StdId)
    {
        case CAN_3508_M1_ID:
        {
            static uint8_t i = 0;
            //get motor id
            i = rx_header.StdId - CAN_3508_M1_ID;
            get_motor_measure(&motor_chassis[i], rx_data);
						printf("speed is %d \r\n", motor_chassis[i].speed_rpm);
            break;
        }

        default:
        {
            break;
        }
    }
		
		__HAL_CAN_ENABLE_IT(hcan, CAN_IT_RX_FIFO0_MSG_PENDING);
}







