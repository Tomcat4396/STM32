#include "sys.h"
#include "usart.h"	  


//////////////////////////////////////////////////////////////////
//�������´���,֧��printf����,������Ҫѡ��use MicroLIB	  
#if 1
#pragma import(__use_no_semihosting)             
//��׼����Ҫ��֧�ֺ���                 
struct __FILE 
{ 
	int handle; 

}; 

FILE __stdout;       
//����_sys_exit()�Ա���ʹ�ð�����ģʽ    
_sys_exit(int x) 
{ 
	x = x; 
} 
//�ض���fputc���� 
int fputc(int ch, FILE *f)
{      
	while((USART1->SR&0X40)==0);//ѭ������,ֱ���������   
    USART1->DR = (u8) ch;      
	return ch;
}
#endif 

/**************************************************************************
�������ܣ�USART1��ʼ������
��ڲ�����bound: ������
����  ֵ����
**************************************************************************/	

void uart_init(u32 bound){
  //GPIO�˿�����
  GPIO_InitTypeDef GPIO_InitStructure;
	USART_InitTypeDef USART_InitStructure;
	NVIC_InitTypeDef NVIC_InitStructure;
	 
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_USART1|RCC_APB2Periph_GPIOA, ENABLE);	//ʹ��USART1��GPIOAʱ��
  
	//USART1_TX   GPIOA.9
  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_9; //PA.9
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;	//�����������
  GPIO_Init(GPIOA, &GPIO_InitStructure);//��ʼ��GPIOA.9
   
  //USART1_RX	  GPIOA.10��ʼ��
  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_10;//PA10
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING;//��������
  GPIO_Init(GPIOA, &GPIO_InitStructure);//��ʼ��GPIOA.10  

  //Usart1 NVIC ����
  NVIC_InitStructure.NVIC_IRQChannel = USART1_IRQn;
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority=3 ;//��ռ���ȼ�3
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 3;		//�����ȼ�3
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;			//IRQͨ��ʹ��
	NVIC_Init(&NVIC_InitStructure);	//����ָ���Ĳ�����ʼ��VIC�Ĵ���
  
   //USART ��ʼ������

	USART_InitStructure.USART_BaudRate = bound;//���ڲ�����
	USART_InitStructure.USART_WordLength = USART_WordLength_8b;//�ֳ�Ϊ8λ���ݸ�ʽ
	USART_InitStructure.USART_StopBits = USART_StopBits_1;//һ��ֹͣλ
	USART_InitStructure.USART_Parity = USART_Parity_No;//����żУ��λ
	USART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None;//��Ӳ������������
	USART_InitStructure.USART_Mode = USART_Mode_Rx | USART_Mode_Tx;	//�շ�ģʽ

  USART_Init(USART1, &USART_InitStructure); //��ʼ������1
  USART_ITConfig(USART1, USART_IT_RXNE, ENABLE);//�������ڽ����ж�
  USART_Cmd(USART1, ENABLE);                    //ʹ�ܴ���1 

}

/**************************************************************************
�������ܣ�USART1 �жϷ�����
��ڲ�������
����  ֵ����
**************************************************************************/	
u16 PID_Buffer[3];
void USART1_IRQHandler(void)                	//����1�жϷ������
{
	static u8 Flag_PID,Receive[50],i,j;
	static	u8 uart_receive=0;//
	static float Data;
	if(uart_receive=='a') Flag_PID=1;//PID����д����ʼλ
	if(uart_receive=='b') Flag_PID=2;// PID ����д��ֹͣλ
	
	if(Flag_PID==1)//�ɼ�����
	{
		Receive[i]=uart_receive;
		i++;
	}
	/*
	a w P/I/D ... b
	*/
	if(Flag_PID==2)//��������
	{
		if(Receive[1]=='w')
		{
			for(j = i;j >= 4; j--)
			{
				Data+=(Receive[j-1]-'0')*pow(10,i-j);
			}
			switch(Receive[2])
			{
				case 'P': 
					M3508[0].PID.Kp=Data/100;
					M3508[1].PID.Kp=Data/100;
				  PID_Buffer[0]=Data;
					break;
				case 'I':
					M3508[0].PID.Ki=Data/100;
				  M3508[1].PID.Ki=Data/100;
				  PID_Buffer[1]=Data;
					break;
				case 'D':
					M3508[0].PID.Kd=Data/100;
				  M3508[1].PID.Kd=Data/100;
				  PID_Buffer[2]=Data;
					break;
			}
			STMFLASH_Write(FLASH_SAVE_ADDR,(u16*)PID_Buffer,sizeof(PID_Buffer));
		}
		Flag_PID=0;
		i=0;
		j=0;
		Data=0;
		memset(Receive,0,sizeof(u8)*50);//��������
	}
} 

