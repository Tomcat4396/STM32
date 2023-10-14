/**************************************************************************
���ߣ����δ��
û���ر����ˣ������ر�Ŭ��
**************************************************************************/

#include "Communication.h"



//��������λ����������

void NiMing_Report(u8 fun, u8 *data, u8 len){
	u8 send_buf[32];
	u8 i;
	if(len > 28) return;	//���28�ֽ����� 
	send_buf[len + 3] = 0;	//У��������
	send_buf[0] = 0X88;	//֡ͷ
	send_buf[1] = fun;	//������
	send_buf[2] = len;	//���ݳ���
	for(i = 0; i < len; i++) send_buf[3 + i] = data[i];			//��������
	for(i = 0; i < len + 3; i++) send_buf[len + 3] += send_buf[i];	//����У���	
	for(i = 0; i < len + 4; i++){
		while(USART_GetFlagStatus(SEND_PORT, USART_FLAG_TC) == RESET);
		USART_SendData(SEND_PORT, send_buf[i]);	//�������ݵ�����1 
	}
}

void Motor_Info_Send(void){
	u8 tbuf[16];
	char *p =(char*)&M3508[0].Speed;
	tbuf[0]=*((char*)p+0);
	tbuf[1]=*((char*)p+1);
	tbuf[2]=*((char*)p+2);
	tbuf[3]=*((char*)p+3);
	
	p=(char*)&M3508[0].PID.Goal_Speed;
	tbuf[4]=*((char*)p+0);
	tbuf[5]=*((char*)p+1);
	tbuf[6]=*((char*)p+2);
	tbuf[7]=*((char*)p+3);
	
	p=(char*)&M3508[1].Speed;
	tbuf[8]=*((char*)p+0);
	tbuf[9]=*((char*)p+1);
	tbuf[10]=*((char*)p+2);
	tbuf[11]=*((char*)p+3);
	
	p=(char*)&M3508[1].PID.Goal_Speed;
	tbuf[12]=*((char*)p+0);
	tbuf[13]=*((char*)p+1);
	tbuf[14]=*((char*)p+2);
	tbuf[15]=*((char*)p+3);
	
	NiMing_Report(0xA1,tbuf,16);
	// printf("%d\r\n", (int32_t)(M3508.Speed));
}


	
