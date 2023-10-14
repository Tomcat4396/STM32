#ifndef _TIMER_H
#define _TIMER_H
#include "sys.h"


extern TIM_HandleTypeDef TIM3_Handler;      //定时器3句柄 
extern TIM_OC_InitTypeDef TIM3_CH1Handler;  //定时器3通道1句柄
extern TIM_OC_InitTypeDef TIM3_CH2Handler;  //定时器3通道2句柄
extern TIM_OC_InitTypeDef TIM3_CH3Handler;  //定时器3通道3句柄
extern TIM_OC_InitTypeDef TIM3_CH4Handler;  //定时器3通道4句柄

void TIM3_PWM_Init(u16 arr, u16 psc);
void PWM_Init(void);

#endif

