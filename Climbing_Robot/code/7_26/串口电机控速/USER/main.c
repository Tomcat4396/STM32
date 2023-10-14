/*
ԭ�ĵ�ַ��
https://blog.csdn.net/qq_28039135/article/details/116379392
���ߣ���С��
������ע�⴮�ڽ�����������ԭ�ӵĴ��ڣ����͵��ٶ�ֵ��Ҫ��һ���س�����Ȼ���ᱻ��Ϊ������
*/




#include "sys.h"
#include "delay.h"
#include "usart.h"
#include "bsp_can.h"
#include <math.h>
#include "pid.h"
#define MOTOR_NUM 1



PID_TypeDef moto_pid[MOTOR_NUM];

void Delay(__IO uint32_t nCount);

void Delay(__IO uint32_t nCount)
{
  while(nCount--){}
}

int main(void)
{
	int len=0;
	int spd_set=0;
	int spd_target=0;
	int16_t speed=0,pos=0;
	u8 tem=0;
	uint32_t last_time=0;
	int light=0;
	int g = 0;
	int i = 0;
//	float i=0.2;

	
	
	GPIO_InitTypeDef GPIO_Initure;
     
    HAL_Init();                    	 			//��ʼ��HAL��    
    Stm32_Clock_Init(336,8,2,7);   				//����ʱ��,168Mhz

    __HAL_RCC_GPIOF_CLK_ENABLE();           	//����GPIOFʱ��
	
    GPIO_Initure.Pin=GPIO_PIN_9|GPIO_PIN_10; 	//PF9,10
    GPIO_Initure.Mode=GPIO_MODE_OUTPUT_PP;  	//�������
    GPIO_Initure.Pull=GPIO_PULLUP;          	//����
    GPIO_Initure.Speed=GPIO_SPEED_HIGH;    	 	//����
    HAL_GPIO_Init(GPIOF,&GPIO_Initure);
	uart_init(115200);
	CAN_Init();
	
	for(g=0;g<MOTOR_NUM;g++)
	{
	pid_init(moto_pid+g);
	}
	
	moto_pid->f_param_init(moto_pid,PID_Speed,16384,5000,5,0,8000,0,2.0,0.02,0);//
	set_moto_current(&hcan1,0,0,0,0 );
	printf("��ʼ��������\r\n");

	while(1)
	{
		if(HAL_GetTick()-last_time>500)
			{
					last_time=HAL_GetTick();
					printf("RUNNING!\r\n");
					if(0==light)
						{
								HAL_GPIO_WritePin(GPIOF,GPIO_PIN_9,GPIO_PIN_SET);		//PF9??1 
								HAL_GPIO_WritePin(GPIOF,GPIO_PIN_10,GPIO_PIN_SET);		//PF10??1  			
								light=1;
						}
					else
						{
								HAL_GPIO_WritePin(GPIOF,GPIO_PIN_9,GPIO_PIN_RESET);		//PF9??0
								HAL_GPIO_WritePin(GPIOF,GPIO_PIN_10,GPIO_PIN_RESET);	//PF10??0  
								light=0;
						}
				pos=((&hcan1)->pRxMsg->Data[0]<<8)|((&hcan1)->pRxMsg->Data[1]);
				speed=((&hcan1)->pRxMsg->Data[2]<<8)|((&hcan1)->pRxMsg->Data[3]);
				tem=(&hcan1)->pRxMsg->Data[6];
				printf("\r\n���ת�٣�%d\r\n",speed);
				printf("\r\n����¶ȣ�%d\r\n",tem);
				printf("\r\n����Ƕȣ�%.1lf\r\n",(pos/8191.0)*360);
			}

		if(USART_RX_STA&0x8000)
		{//���ڽ��մ���					   
				len=USART_RX_STA&0x3fff;//�õ��˴ν��յ������ݳ���
			
				if(USART_RX_BUF[0]=='-')
					{//��������ת֧��
							for(i=1;i<len;i++)
								{
									spd_set-=(USART_RX_BUF[i]-'0')*pow(10,len-i-1);
								}
					}
				else
					{
							for(i=0;i<len;i++)
								{
									spd_set+=(USART_RX_BUF[i]-'0')*pow(10,len-i-1);
								}
					}
			
			printf("\r\n���͵���ϢΪ:\r\n");
			HAL_UART_Transmit(&UART1_Handler,(uint8_t*)USART_RX_BUF,len,1000);	//���ͽ��յ�������
			while(__HAL_UART_GET_FLAG(&UART1_Handler,UART_FLAG_TC)!=SET);		//�ȴ����ͽ���
			printf("\r\n����д��ֵ:%d\r\n",spd_set);//���뻻��
			
			spd_target=spd_set;
			USART_RX_STA=0;
			spd_set=0;
		}

		
		
		for(g=0;g<MOTOR_NUM;g++)
		{//����pid�Լ��������
			
			moto_pid[g].target=spd_target;
			
			moto_pid[g].f_cal_pid(moto_pid+g,moto_chassis[g].speed_rpm);

		}
		
		set_moto_current(&hcan1,moto_pid[0].output,0,0,0);//���͵���ֵ

	}
}



void Error_Handler(void){
	while(1);
	
}
