/**************************************************************************
���ߣ����δ��
û���ر����ˣ������ر�Ŭ��
**************************************************************************/
#ifndef _BSP_CAN_H
#define _BSP_CAN_H	 

#include "sys.h"
#include "delay.h"
#include "usart.h"
#include "Motor_Control.h"

void CAN1_Init(u8 tsjw, u8 tbs2, u8 tbs1, u16 brp);//CAN��ʼ��
u8 CAN1_SendMsg(u8* msg, u8 len, u32 ID);//��Ϣ����

#endif

