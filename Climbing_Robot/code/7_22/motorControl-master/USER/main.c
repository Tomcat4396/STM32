/**************************************************************************
���ߣ����δ��
û���ر����ˣ������ر�Ŭ��
**************************************************************************/


#include "tim.h"
#include "Motor_Control.h"
#include "usart.h"


void PID_Write(void)
{
	u16 PID_data[3];
	STMFLASH_Read(FLASH_SAVE_ADDR,(u16*)PID_data,sizeof(PID_data));
	M3508[0].PID.Kp=PID_data[0];
	M3508[0].PID.Ki=PID_data[1];
	M3508[0].PID.Kd=PID_data[2];
	M3508[1].PID.Kp=PID_data[0];
	M3508[1].PID.Ki=PID_data[1];
	M3508[1].PID.Kd=PID_data[2];
}

 int main(void)
 {	
//	u8 i;
  NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2);	 //����NVIC�жϷ���2:2λ��ռ���ȼ���2λ��Ӧ���ȼ�
	delay_init();
	uart_init(115200);
	//Adc_Init();
	LED_Init();
	
	//PID_Write();
//	
//	for(i = 0; i < 2; ++i){
//		delay_ms(1000);
//	}
	// 15MS
	// ������Ҫ�Ż�
	CAN1_Init(CAN_SJW_1tq, CAN_BS2_8tq, CAN_BS1_9tq, 2);
	Control_Initialize();//����ṹ���ʼ��
	TIM3_Int_Init(15, 36000-1); // 8ms�ж�һ��
	//printf("start\r\n");
	while(1){
		//printf("ok\r\n");
		delay_ms(500);
		//LED1=!LED1;
		LED2=!LED2;
	}
	
 }



