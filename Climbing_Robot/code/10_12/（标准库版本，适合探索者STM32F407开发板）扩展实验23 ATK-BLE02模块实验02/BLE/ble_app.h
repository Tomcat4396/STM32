#ifndef _BLE_APP_H_
#define _BLE_APP_H_

#include "sys.h"
#include "ble_cfg.h"

//////////////////////////////////////////////////////////////////////////////////	   
//������ֻ��ѧϰʹ�ã�δ��������ɣ��������������κ���;
//ALIENTEK STM32F407������
//ATK-BLE02ģ�鹦������
//����ԭ��@ALIENTEK
//������̳:www.openedv.com
//��������:2020/7/2
//�汾��V1.0
//��Ȩ���У�����ؾ���
//Copyright(C) ������������ӿƼ����޹�˾ 2020-2030
//All rights reserved
//******************************************************************************** 
//��

#define BLE_STA  PFin(6)  
#define BLE_WKUP  PCout(0) 


void Ble_Test(void);

#endif


