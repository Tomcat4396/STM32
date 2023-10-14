/*
原文地址：
https://blog.csdn.net/qq_28039135/article/details/116379392
作者：胡小安
！！！注意串口接收套用正点原子的串口，发送的速度值需要加一个回车，不然不会被视为结束。
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
     
    HAL_Init();                    	 			//初始化HAL库    
    Stm32_Clock_Init(336,8,2,7);   				//设置时钟,168Mhz

    __HAL_RCC_GPIOF_CLK_ENABLE();           	//开启GPIOF时钟
	
    GPIO_Initure.Pin=GPIO_PIN_9|GPIO_PIN_10; 	//PF9,10
    GPIO_Initure.Mode=GPIO_MODE_OUTPUT_PP;  	//推挽输出
    GPIO_Initure.Pull=GPIO_PULLUP;          	//上拉
    GPIO_Initure.Speed=GPIO_SPEED_HIGH;    	 	//高速
    HAL_GPIO_Init(GPIOF,&GPIO_Initure);
	uart_init(115200);
	CAN_Init();
	
	for(g=0;g<MOTOR_NUM;g++)
	{
	pid_init(moto_pid+g);
	}
	
	moto_pid->f_param_init(moto_pid,PID_Speed,16384,5000,5,0,8000,0,2.0,0.02,0);//
	set_moto_current(&hcan1,0,0,0,0 );
	printf("初始化结束。\r\n");

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
				printf("\r\n电机转速：%d\r\n",speed);
				printf("\r\n电机温度：%d\r\n",tem);
				printf("\r\n电机角度：%.1lf\r\n",(pos/8191.0)*360);
			}

		if(USART_RX_STA&0x8000)
		{//串口接收处理					   
				len=USART_RX_STA&0x3fff;//得到此次接收到的数据长度
			
				if(USART_RX_BUF[0]=='-')
					{//加入正反转支持
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
			
			printf("\r\n发送的消息为:\r\n");
			HAL_UART_Transmit(&UART1_Handler,(uint8_t*)USART_RX_BUF,len,1000);	//发送接收到的数据
			while(__HAL_UART_GET_FLAG(&UART1_Handler,UART_FLAG_TC)!=SET);		//等待发送结束
			printf("\r\n向电调写入值:%d\r\n",spd_set);//插入换行
			
			spd_target=spd_set;
			USART_RX_STA=0;
			spd_set=0;
		}

		
		
		for(g=0;g<MOTOR_NUM;g++)
		{//计算pid以及电机控速
			
			moto_pid[g].target=spd_target;
			
			moto_pid[g].f_cal_pid(moto_pid+g,moto_chassis[g].speed_rpm);

		}
		
		set_moto_current(&hcan1,moto_pid[0].output,0,0,0);//发送电流值

	}
}



void Error_Handler(void){
	while(1);
	
}
