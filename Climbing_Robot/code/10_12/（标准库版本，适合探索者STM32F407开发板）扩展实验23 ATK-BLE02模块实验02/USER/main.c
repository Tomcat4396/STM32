#include "sys.h"
#include "delay.h"
#include "usart.h"
#include "led.h"
#include "key.h"
#include "ble_app.h"

/************************************************
 ALIENTEK̽����STM32F407������ ��չʵ��23
 ATK-BLE02��������ģ��͸��ʵ��
 ����֧�֣�www.openedv.com
 �Ա����̣�http://eboard.taobao.com
 ��ע΢�Ź���ƽ̨΢�źţ�"����ԭ��"����ѻ�ȡSTM32���ϡ�
 ������������ӿƼ����޹�˾
 ���ߣ�����ԭ�� @ALIENTEK
************************************************/

int main(void)
{

    NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2);//����ϵͳ�ж����ȼ�����2
    delay_init(168);                               //��ʼ����ʱ����
    uart_init(115200);		                       //��ʼ�����ڲ�����Ϊ115200
    LED_Init();					                   //��ʼ��LED
    KEY_Init();                                    //KEY��ʼ��
    Ble_Test();                                    //ble����
}
