//本文件是UTF-8编码，main.c是ANSI，所以在本文件里写printf发送串口会乱码。


#include "bsp_can.h"
//#include "cmsis_os.h"


//moto_measure_t moto_pit;
//moto_measure_t moto_yaw;
//moto_measure_t moto_poke;	//拨单电机
moto_measure_t moto_chassis[4] = {0};//4 chassis moto
moto_measure_t moto_info;


void get_total_angle(moto_measure_t *p);
void get_moto_offset(moto_measure_t *ptr, uint8_t Data[8]);

extern CAN_HandleTypeDef hcan;



void CAN1_RX0_IRQHandler(void){
	
//	printf("IRQHandler\r\n");
	
	HAL_CAN_IRQHandler(&hcan);
	
	
}


void HAL_CAN_RxFifo0MsgPendingCallback(CAN_HandleTypeDef* _hcan)
{
	//	printf("Fifo0CallBack\r\n");
	
	  if (HAL_CAN_GetRxMessage(&hcan, CAN_RX_FIFO0, &RxHeader, RxData) != HAL_OK)
  {
    /* Reception Error */
    Error_Handler();
  }
	
	//printf("value:%d\r\n",(RxData[6]<<8)+RxData[7]);
	
	
	
	//ignore can1 or can2.
	switch(RxHeader.StdId){
		case 0x201:
		case 0x202:
		case 0x203:
		case 0x204:
			{
				static u8 i;
				
				i = RxHeader.StdId - 0x201;
				moto_chassis[i].msg_cnt++ <= 50	?	get_moto_offset(&moto_chassis[i], RxData) : get_moto_measure(&moto_chassis[i], RxData );
				//get_moto_measure(&moto_info, _hcan);
				get_moto_measure(&moto_chassis[i], RxData);
					
			}
			break;
		
		
	}
		
	__HAL_CAN_ENABLE_IT(&hcan, CAN_IT_RX_FIFO0_MSG_PENDING);

}




void get_moto_measure(moto_measure_t *ptr, uint8_t Data[8])
{

	ptr->last_angle = ptr->angle;
	ptr->angle = (uint16_t)(Data[0]<<8 | Data[1]) ;
	ptr->real_current  = (int16_t)(Data[2]<<8 | Data[3]);
	ptr->speed_rpm = ptr->real_current;	//这里是因为两种电调对应位不一样的信息
	ptr->given_current = (int16_t)(Data[4]<<8 | Data[5])/-5;
	ptr->hall = Data[6];
	if(ptr->angle - ptr->last_angle > 4096)
		ptr->round_cnt --;
	else if (ptr->angle - ptr->last_angle < -4096)
		ptr->round_cnt ++;
	ptr->total_angle = ptr->round_cnt * 8192 + ptr->angle - ptr->offset_angle;
}

/*this function should be called after system+can init */
void get_moto_offset(moto_measure_t *ptr, uint8_t Data[8])
{
	ptr->angle = (uint16_t)(Data[0]<<8 | Data[1]) ;
	ptr->offset_angle = ptr->angle;
}

#define ABS(x)	( (x>0) ? (x) : (-x) )
/**
*@bref 电机上电角度=0， 之后用这个函数更新3510电机的相对开机后（为0）的相对角度。
	*/
void get_total_angle(moto_measure_t *p){
	
	int res1, res2, delta;
	if(p->angle < p->last_angle){			//可能的情况
		res1 = p->angle + 8192 - p->last_angle;	//正转，delta=+
		res2 = p->angle - p->last_angle;				//反转	delta=-
	}else{	//angle > last
		res1 = p->angle - 8192 - p->last_angle ;//反转	delta -
		res2 = p->angle - p->last_angle;				//正转	delta +
	}
	//不管正反转，肯定是转的角度小的那个是真的
	if(ABS(res1)<ABS(res2))
		delta = res1;
	else
		delta = res2;

	p->total_angle += delta;
	p->last_angle = p->angle;
}





void set_moto_current(CAN_HandleTypeDef* _hcan,CAN_TxHeaderTypeDef* TxHeader,uint8_t TxData[8] ,uint32_t*	mailbox,s16 iq1, s16 iq2, s16 iq3, s16 iq4){

	TxHeader->StdId = 0x200;
	TxHeader->IDE = CAN_ID_STD;
	TxHeader->RTR = CAN_RTR_DATA;
	TxHeader->DLC = 0x08;
	TxData[0] = iq1 >> 8;
	TxData[1] = iq1;
	TxData[2] = iq2 >> 8;
	TxData[3] = iq2;
	TxData[4] = iq3 >> 8;
	TxData[5] = iq3;
	TxData[6] = iq4 >> 8;
	TxData[7] = iq4;
	//printf("TX\r\n");
		if (HAL_CAN_AddTxMessage(&hcan, TxHeader, TxData,mailbox) != HAL_OK)
	{
		printf("ERROR\r\n");
		/* Transmission request Error */
		//Error_Handler();//出问题可能要注释掉这行
	}
else{
//printf("oksending\r\n");
}	
}	




