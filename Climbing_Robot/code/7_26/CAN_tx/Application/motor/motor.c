#include "motor.h"
#include "can.h"
#include "usart.h"
#include "stm32f4xx_hal.h"


void Send_Motor(CAN_HandleTypeDef *hcan, uint16_t motor1, uint16_t motor2, uint16_t motor3, uint16_t motor4, 
								uint32_t stdId, uint32_t extId, uint32_t iDE, uint32_t rTR, uint32_t dLC)
{
		//����һ����������ַ
		uint32_t send_mail_box;
		uint8_t	motor_array[8] = {0};
//		motor_tx.StdId = 0x200;
//    motor_tx.IDE = CAN_ID_STD;
//		motor_tx.ExtId=0x12;
//    motor_tx.RTR = CAN_RTR_DATA;
//    motor_tx.DLC = 0x08;
	
		CAN_TxHeaderTypeDef motor_tx;
	  //���õ�һ������ID��Ϣ����ǰ�ĸ������
		motor_tx.StdId = stdId;
    motor_tx.IDE = iDE;
		motor_tx.ExtId = extId;
    motor_tx.RTR = rTR;
    motor_tx.DLC = dLC;
	
	
		//�����ĸ��������ֵ
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
				printf("�������ͳɹ�");
		}
}