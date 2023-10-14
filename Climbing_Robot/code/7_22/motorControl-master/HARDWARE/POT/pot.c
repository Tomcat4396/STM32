/**************************************************************************
���ߣ����δ��
û���ر����ˣ������ر�Ŭ��
**************************************************************************/
#include "pot.h"

float v[4]={0};
float angle[4]={0};


/**************************************************************************
�������ܣ���λ����ѹ�ɼ�
��ڲ�������
����  ֵ����
**************************************************************************/
void get_pot_adc()
{
	  u16 adc[4]={0,0,0,0};
		u8 i;
		adc[0]=Get_Adc_Average(ADC_Channel_0,3);
		adc[1]=Get_Adc_Average(ADC_Channel_1,3);
		adc[2]=Get_Adc_Average(ADC_Channel_2,3);
		adc[3]=Get_Adc_Average(ADC_Channel_3,3);
			for(i=0;i<4;i++)
		{
			v[i]=(float)(adc[i])*(3.3/4096);
			adc[i]=v[i];
		}
}


/**************************************************************************
�������ܣ���λ���Ƕ�ת��
��ڲ�������
����  ֵ����
**************************************************************************/

void adc_angle_transform()
{
	u8 i;
	for(i=0;i<4;i++)
	{
		angle[i]=v[i]*300/3.3;
		//printf("angle = %f\t", angle[i]);
	}
	//printf("\r\n");
}

/**************************************************************************
�������ܣ����Ĵ���λ
��ڲ�������
����  ֵ����
**************************************************************************/
double target_angle[4];
int f_speed=5;
void angle_speed_transform()
{
	int i,error;
	for(i = 0; i < 4; i++)
	{
		error = angle[i] - target_angle[i];
		if(fabs(error) > 1.0)
		{
			switch(i)
			{
				case 0:
					if(error > 0) M3508[i+2].PID.Goal_Speed = f_speed;
					else M3508[i+2].PID.Goal_Speed =  -f_speed;
					break;
				case 1:
					if(error > 0) M3508[i+2].PID.Goal_Speed = f_speed;
					else M3508[i+2].PID.Goal_Speed =  -f_speed;
					break;
				case 2:
					if(error > 0) M3508[i+2].PID.Goal_Speed = f_speed;
					else M3508[i+2].PID.Goal_Speed =  -f_speed;
					break;
				case 3:
					if(error > 0) M3508[i+2].PID.Goal_Speed = f_speed;
					else M3508[i+2].PID.Goal_Speed =  -f_speed;
					break;
				default: break;
			}
		}
		else
		{
				M3508[i+2].PID.Goal_Speed = 0;
		}
	}		
}

