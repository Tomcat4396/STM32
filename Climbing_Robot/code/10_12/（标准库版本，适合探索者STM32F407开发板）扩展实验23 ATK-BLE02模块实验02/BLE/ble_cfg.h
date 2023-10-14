#ifndef _BLE_CFG_H
#define _BLE_CFG_H

//////////////////////////////////////////////////////////////////////////////////	   
//������ֻ��ѧϰʹ�ã�δ��������ɣ��������������κ���;
//ALIENTEK STM32F407������
//ATK-BLE02ģ��������� 
//����ԭ��@ALIENTEK
//������̳:www.openedv.com
//��������:2020/7/2
//�汾��V1.0
//��Ȩ���У�����ؾ���
//Copyright(C) ������������ӿƼ����޹�˾ 2020-2030
//All rights reserved
//******************************************************************************** 
//��

/*******************��������***********************/

/*��������*/
#define BLE_NAME  "ATK-BLE02"

/*������ӭ��*/
#define BLE_HELLO "ATK-BLE02"

/*����ģʽ*/
#define BLE_MODE_M    'M'   /*��ģʽ(������)*/
#define BLE_MODE_S    'S'   /*��ģʽ(������)*/
#define BLE_MODE_O    'O'   /*�۲�ģʽ(��������,ɨ��㲥����)*/
#define BLE_MODE_B    'B'   /*�㲥ģʽ(��������,�㲥�û��Զ�������)*/
#define BLE_MODE_I    'I'   /*iBeacon(��������,�㲥�ض�����,���ڶ�λ)*/

/*���书��*/
#define BLE_TP_N20dBm  1   /*-20dBm*/
#define BLE_TP_N14dBm  2   /*-14dBm*/
#define BLE_TP_N8dBm   3   /*-8dBm*/
#define BLE_TP_N3dBm   4   /*-3dBm*/
#define BLE_TP_P0dBm   5   /*+0dBm*/
#define BLE_TP_P1dBm   6   /*+1dBm*/
#define BLE_TP_P2dBm   7   /*+2dBm*/
#define BLE_TP_P3dBm   8   /*+3dBm*/


/*��������*/
#define BLE_UARTBPS   115200   /*������ ��Χ:1200-230400
                                (1200,2400,4800,9600,14400,19200��38400��43000
                                57600��76800��115200��128000��230400)
                                */
#define BLE_UARTDATE  8        /*����λ*/
#define BLE_UARTSTOP  1        /*ֹͣλ*/
#define BLE_UARTPR    0        /*��У��*/

/*�㲥�ٶ�*/
#define BLE_ADPTIM  5  /*��λ:10ms */

/*��·ƥ������ʹ��*/
#define BLE_LINKPASSEN 0

/*����LEDʹ��*/
#define BLE_LEDEN  1

/*���豸����˯��ʹ��*/
#define BLE_SLAVESLEEPEN 0

/*����������ʹ��*/
#define BLE_MAXPUT  0

/************************************************/

/*LED��������*/

#define LED0_ON  "+LED0:ON"
#define LED0_OFF "+LED0:OFF"


#endif
