/**************************************************************************
���ߣ����δ��
û���ر����ˣ������ر�Ŭ��
**************************************************************************/
#include "can.h"

/**************************************************************************
�������ܣ�CAN��ʼ������
��ڲ�����tsjw:����ͬ����Ծʱ�䵥Ԫ.��Χ:CAN_SJW_1tq~ CAN_SJW_4tq
					tbs2:ʱ���2��ʱ�䵥Ԫ.   ��Χ:CAN_BS2_1tq~CAN_BS2_8tq;
					tbs1:ʱ���1��ʱ�䵥Ԫ.   ��Χ:CAN_BS1_1tq ~CAN_BS1_16tq
					brp :�����ʷ�Ƶ��.��Χ:1~1024;  tq=(brp)*tpclk1
					������=Fpclk1/((tbs1+1+tbs2+1+1)*brp)
					FilterID:��ʶ��ID
����  ֵ����
**************************************************************************/	
void CAN1_Init(u8 tsjw, u8 tbs2, u8 tbs1, u16 brp)
{
	GPIO_InitTypeDef GPIO_InitStructure; 
	CAN_InitTypeDef CAN_InitStructure;
	CAN_FilterInitTypeDef CAN_FilterInitStructure;
	NVIC_InitTypeDef NVIC_InitStructure;

	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA | RCC_APB2Periph_AFIO, ENABLE);//ʹ��PORTBʱ��	                   											 
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_CAN1, ENABLE);//ʹ��CAN1ʱ��	

	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_12;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;	//��������
	GPIO_Init(GPIOA, &GPIO_InitStructure);			//��ʼ��IO

	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_11;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPU;	//��������
	GPIO_Init(GPIOA, &GPIO_InitStructure);			//��ʼ��IO
	
	GPIO_PinRemapConfig(GPIO_Remap1_CAN1,ENABLE);//������ӳ��
	
	//CAN��Ԫ����
	CAN_InitStructure.CAN_TTCM = DISABLE;			//��ʱ�䴥��ͨ��ģʽ  
	CAN_InitStructure.CAN_ABOM = DISABLE;			//����Զ����߹���	 
	CAN_InitStructure.CAN_AWUM = DISABLE;			//˯��ģʽͨ���������(���CAN->MCR��SLEEPλ)
	CAN_InitStructure.CAN_NART = ENABLE;			//��ֹ�����Զ����� 
	CAN_InitStructure.CAN_RFLM = DISABLE;		 	//���Ĳ�����,�µĸ��Ǿɵ�  
	CAN_InitStructure.CAN_TXFP = DISABLE;			//���ȼ��ɱ��ı�ʶ������ 
	CAN_InitStructure.CAN_Mode = CAN_Mode_Normal;	//��ͨģʽ
	
	//���ò�����
	CAN_InitStructure.CAN_SJW = tsjw;				//����ͬ����Ծ���(Tsjw)Ϊtsjw+1��ʱ�䵥λ  CAN_SJW_1tq	 CAN_SJW_2tq CAN_SJW_3tq CAN_SJW_4tq
	CAN_InitStructure.CAN_BS1 = tbs1; 			//Tbs1=tbs1+1��ʱ�䵥λCAN_BS1_1tq ~CAN_BS1_16tq
	CAN_InitStructure.CAN_BS2 = tbs2;				//Tbs2=tbs2+1��ʱ�䵥λCAN_BS2_1tq ~	CAN_BS2_8tq
	CAN_InitStructure.CAN_Prescaler = brp;        //��Ƶϵ��(Fdiv)Ϊbrp+1	
	CAN_Init(CAN1, &CAN_InitStructure);        	//��ʼ��CAN1 
  
	//���ù�����0
	CAN_FilterInitStructure.CAN_FilterNumber = 0;	//������0
	CAN_FilterInitStructure.CAN_FilterMode = CAN_FilterMode_IdList; 	//����λģʽ
	CAN_FilterInitStructure.CAN_FilterScale = CAN_FilterScale_32bit; 	//32λ�� 
	
	CAN_FilterInitStructure.CAN_FilterIdHigh = (0x01e0>>5)<<5;	//32λID
	CAN_FilterInitStructure.CAN_FilterIdLow =CAN_Id_Standard | CAN_RTR_Data;

	CAN_FilterInitStructure.CAN_FilterMaskIdHigh =(0x02e0>>5)<<5; //32λMASK
	CAN_FilterInitStructure.CAN_FilterMaskIdLow = CAN_Id_Standard | CAN_RTR_Data;
	
	CAN_FilterInitStructure.CAN_FilterFIFOAssignment = CAN_Filter_FIFO0;//������0������FIFO0
	CAN_FilterInitStructure.CAN_FilterActivation = ENABLE;//���������0
	CAN_FilterInit(&CAN_FilterInitStructure);			//�˲�����ʼ��
  
	//���ù�����1
	CAN_FilterInitStructure.CAN_FilterNumber = 1;	//������1
	CAN_FilterInitStructure.CAN_FilterMode = CAN_FilterMode_IdList; 	//����λģʽ
	CAN_FilterInitStructure.CAN_FilterScale = CAN_FilterScale_32bit; 	//32λ�� 
	
	CAN_FilterInitStructure.CAN_FilterIdHigh = (0x03e0 >> 5)<<5;	//32λID
	CAN_FilterInitStructure.CAN_FilterIdLow = CAN_Id_Standard | CAN_RTR_Data;

	CAN_FilterInitStructure.CAN_FilterMaskIdHigh = (0x04e0 >> 5)<<5; //32λMASK
	CAN_FilterInitStructure.CAN_FilterMaskIdLow = CAN_Id_Standard | CAN_RTR_Data;
	
	CAN_FilterInitStructure.CAN_FilterFIFOAssignment = CAN_Filter_FIFO0;//������0������FIFO0
	CAN_FilterInitStructure.CAN_FilterActivation = ENABLE;//���������1
	CAN_FilterInit(&CAN_FilterInitStructure);			//�˲�����ʼ��
	
	//���ù�����2
	CAN_FilterInitStructure.CAN_FilterNumber = 2;	//������2
	CAN_FilterInitStructure.CAN_FilterMode = CAN_FilterMode_IdList; 	//����λģʽ
	CAN_FilterInitStructure.CAN_FilterScale = CAN_FilterScale_32bit; 	//32λ�� 
	
	CAN_FilterInitStructure.CAN_FilterIdHigh = (0x0201)<<5;	//32λID
	CAN_FilterInitStructure.CAN_FilterIdLow = CAN_Id_Standard | CAN_RTR_Data;

	CAN_FilterInitStructure.CAN_FilterMaskIdHigh = (0x0202)<<5; //32λMASK
	CAN_FilterInitStructure.CAN_FilterMaskIdLow = CAN_Id_Standard | CAN_RTR_Data;
	
	CAN_FilterInitStructure.CAN_FilterFIFOAssignment = CAN_Filter_FIFO0;//������0������FIFO0
	CAN_FilterInitStructure.CAN_FilterActivation = ENABLE;//���������2
	CAN_FilterInit(&CAN_FilterInitStructure);			//�˲�����ʼ��
	
	//���ù�����3
	CAN_FilterInitStructure.CAN_FilterNumber = 3;	//������2
	CAN_FilterInitStructure.CAN_FilterMode = CAN_FilterMode_IdList; 	//����λģʽ
	CAN_FilterInitStructure.CAN_FilterScale = CAN_FilterScale_32bit; 	//32λ�� 
	
	CAN_FilterInitStructure.CAN_FilterIdHigh = (0x0203)<<5;	//32λID
	CAN_FilterInitStructure.CAN_FilterIdLow = CAN_Id_Standard | CAN_RTR_Data;

	CAN_FilterInitStructure.CAN_FilterMaskIdHigh = (0x0204)<<5; //32λMASK
	CAN_FilterInitStructure.CAN_FilterMaskIdLow = CAN_Id_Standard | CAN_RTR_Data;
	
	CAN_FilterInitStructure.CAN_FilterFIFOAssignment = CAN_Filter_FIFO0;//������0������FIFO0
	CAN_FilterInitStructure.CAN_FilterActivation = ENABLE;//���������2
	CAN_FilterInit(&CAN_FilterInitStructure);			//�˲�����ʼ��
	
	//���ù�����4
	CAN_FilterInitStructure.CAN_FilterNumber = 4;	//������2
	CAN_FilterInitStructure.CAN_FilterMode = CAN_FilterMode_IdList; 	//����λģʽ
	CAN_FilterInitStructure.CAN_FilterScale = CAN_FilterScale_32bit; 	//32λ�� 
	
	CAN_FilterInitStructure.CAN_FilterIdHigh = (0x0205)<<5;	//32λID
	CAN_FilterInitStructure.CAN_FilterIdLow = CAN_Id_Standard | CAN_RTR_Data;

	CAN_FilterInitStructure.CAN_FilterMaskIdHigh = (0x0206)<<5; //32λMASK
	CAN_FilterInitStructure.CAN_FilterMaskIdLow = CAN_Id_Standard | CAN_RTR_Data;
	
	CAN_FilterInitStructure.CAN_FilterFIFOAssignment = CAN_Filter_FIFO0;//������0������FIFO0
	CAN_FilterInitStructure.CAN_FilterActivation = ENABLE;//���������2
	CAN_FilterInit(&CAN_FilterInitStructure);			//�˲�����ʼ��
	
	
	CAN_ITConfig(CAN1,CAN_IT_FMP0,ENABLE);				//FIFO0��Ϣ�Һ��ж�����.		    

	NVIC_InitStructure.NVIC_IRQChannel = USB_LP_CAN1_RX0_IRQn;
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 1;     // �����ȼ�Ϊ1
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0;            // �����ȼ�Ϊ0
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
	NVIC_Init(&NVIC_InitStructure);
}

/**
  * @brief  CAN1���ݷ��ͺ�������
	* @param  msg:u8��������
	* @param  len:���ݳ���
	* @param  ID:���ջ�ID
	* @retval 0:�ɹ� 1:ʧ��
  */
u8 CAN1_SendMsg(u8* msg,u8 len,u32 ID)
{	
	u8 mbox;
	u16 i=0;
	CanTxMsg TxMessage;
	TxMessage.StdId=ID;			// ��׼��ʶ�� 
	TxMessage.ExtId=ID;			// ������չ��ʾ�� 
	TxMessage.IDE=CAN_Id_Standard; 	// ��׼֡
	TxMessage.RTR=CAN_RTR_Data;		// ����֡
	TxMessage.DLC=len;				// Ҫ���͵����ݳ���
	for(i=0;i<len;i++)
	TxMessage.Data[i]=msg[i];			          
	mbox= CAN_Transmit(CAN1, &TxMessage);   
	i=0; 
	while((CAN_TransmitStatus(CAN1, mbox)==CAN_TxStatus_Failed)&&(i<0XFFF))i++;	//�ȴ����ͽ���
	if(i>=0XFFF)return 1;
	return 0;	 
}

/**
  * @brief  CAN1�жϷ�����
  * @param  void
  * @retval void
  */


void USB_LP_CAN1_RX0_IRQHandler(void){
	CanRxMsg RxMessage;
	static float Goal_Speed = 0;
  u8 i;
	CAN_Receive(CAN1, CAN_FIFO0, &RxMessage);
	
	switch(RxMessage.StdId)
	{
		case 0x0201 : {
      for(i=0; i < 8; ++i){
        //����ԭʼ��Ϣ
        M3508[0].Original_Data[i] = RxMessage.Data[i];
      }
      //������Ϣ��ת��
      Data_Convert(0);
			//printf("0x201\r\n");
			//printf("Speed:%d\r\n",M3508[0].Speed);
      break;
    }
    case 0x0202 : {
      for(i=0; i < 8; ++i){
        //����ԭʼ��Ϣ
        M3508[1].Original_Data[i] = RxMessage.Data[i];
      }
      //������Ϣ��ת��
      Data_Convert(1);
			
			//printf("0x202");
			break;
    }
		case 0x0203 : {
      for(i=0; i < 8; ++i){
        //����ԭʼ��Ϣ
        M3508[2].Original_Data[i] = RxMessage.Data[i];
      }
      //������Ϣ��ת��
      Data_Convert(2);
			//printf("%d\r\n",M3508[2].Speed);
			//printf("0x202");
			break;
    }
		case 0x0204 : {
      for(i=0; i < 8; ++i){
        //����ԭʼ��Ϣ
        M3508[3].Original_Data[i] = RxMessage.Data[i];
      }
      //������Ϣ��ת��
      Data_Convert(3);
			
			//printf("0x202");
			break;
    }
		case 0x0205 : {
      for(i=0; i < 8; ++i){
        //����ԭʼ��Ϣ
        M3508[4].Original_Data[i] = RxMessage.Data[i];
      }
      //������Ϣ��ת��
      Data_Convert(4);
			
			//printf("0x202");
			break;
    }
		case 0x0206 : {
      for(i=0; i < 8; ++i){
        //����ԭʼ��Ϣ
        M3508[5].Original_Data[i] = RxMessage.Data[i];
      }
      //������Ϣ��ת��
      Data_Convert(5);
			
			//printf("0x202");
			break;
    }
    
    // ����������Ϣ
    case 0x01e0 >> 5 : {
      Goal_Speed = *((float*)(&RxMessage.Data[0])); //��1
      Goal_Speed *= 1000;
			//printf("ID1:%d\r\n",(int32_t)Goal_Speed);
		  M3508[0].PID.Goal_Speed =(int32_t) Goal_Speed;
			//printf("Goal_Speed: %d\r\n",M3508[0].PID.Goal_Speed);
			//printf("0x01e0\r\n");
			
			Goal_Speed=*((float*)(&RxMessage.Data[4]));//��2
			Goal_Speed*=1000;
			M3508[1].PID.Goal_Speed=(int32_t)Goal_Speed;
			break;
    }
    case 0x02e0 >> 5 : {
      Goal_Speed = *((float*)(&RxMessage.Data[0])); //
      Goal_Speed *= 1000;
			//printf("ID3:%d\r\n",(int32_t)Goal_Speed);
		  M3508[2].PID.Goal_Speed =(int32_t) Goal_Speed;

      Goal_Speed = *((float*)(&RxMessage.Data[4])); //
      Goal_Speed *= 1000;
			//printf("ID4:%d\r\n",(int32_t)Goal_Speed);
		  M3508[3].PID.Goal_Speed =(int32_t) Goal_Speed;
			break;
    }
    case 0x03e0 >> 5 : {
      Goal_Speed = *((float*)(&RxMessage.Data[0])); //
      Goal_Speed *= 1000;
			//printf("ID5:%d\r\n",(int32_t)Goal_Speed);
		  M3508[4].PID.Goal_Speed =(int32_t) Goal_Speed;

      Goal_Speed = *((float*)(&RxMessage.Data[4])); //
      Goal_Speed *= 1000;
			//printf("ID6:%d\r\n",(int32_t)Goal_Speed);
		  M3508[5].PID.Goal_Speed =(int32_t) Goal_Speed;
			break;
    }
    
  }
	return;
}

