/**************************************************************************
���ߣ����δ��
û���ر����ˣ������ر�Ŭ��
**************************************************************************/
#ifndef _POT_H
#define _POT_H

#include "sys.h"
#include "delay.h"
#include "usart.h"
#include "adc.h"
#include "math.h"
#include "Motor_Control.h"

extern float angle[4];

void get_pot_adc(void);//��λ����ѹ�ɼ�
void adc_angle_transform(void);//��λ���Ƕ�ת��
void angle_speed_transform(void);//���Ĵ���λ

#endif
