#include "timer.h"
#include "led.h"

TIM_HandleTypeDef TIM3_Handler;     //定时器句柄
TIM_OC_InitTypeDef TIM3_CH1Handler; //定时器3通道1句柄
TIM_OC_InitTypeDef TIM3_CH2Handler; //定时器3通道1句柄
TIM_OC_InitTypeDef TIM3_CH3Handler; //定时器3通道1句柄
TIM_OC_InitTypeDef TIM3_CH4Handler; //定时器3通道1句柄

//TIM4 PWM部分初始化
//PWM输出初始化
//arr：自动重装值
//psc：时钟预分频数
void TIM3_PWM_Init(u16 arr, u16 psc)
{
    TIM3_Handler.Instance = TIM3;                       //定时器3
    TIM3_Handler.Init.Prescaler = psc;                  //定时器分频
    TIM3_Handler.Init.CounterMode = TIM_COUNTERMODE_UP; //向上计数模式
    TIM3_Handler.Init.Period = arr;                     //自动重装载值
    TIM3_Handler.Init.ClockDivision = TIM_CLOCKDIVISION_DIV1;
    HAL_TIM_PWM_Init(&TIM3_Handler); //初始化PWM

    TIM3_CH1Handler.OCMode = TIM_OCMODE_PWM2;                                  //模式选择PWM1
    TIM3_CH1Handler.Pulse = arr / 2;                                           //设置比较值,此值用来确定占空比，
    TIM3_CH1Handler.OCPolarity = TIM_OCPOLARITY_LOW;                           //输出比较极性为低
    HAL_TIM_PWM_ConfigChannel(&TIM3_Handler, &TIM3_CH1Handler, TIM_CHANNEL_1); //配置TIM3通道2
    HAL_TIM_PWM_Start(&TIM3_Handler, TIM_CHANNEL_1);                           //开启PWM通道2

    TIM3_CH2Handler.OCMode = TIM_OCMODE_PWM2;                                  //模式选择PWM1
    TIM3_CH2Handler.Pulse = arr / 2;                                           //设置比较值,此值用来确定占空比，
    TIM3_CH2Handler.OCPolarity = TIM_OCPOLARITY_LOW;                           //输出比较极性为低
    HAL_TIM_PWM_ConfigChannel(&TIM3_Handler, &TIM3_CH2Handler, TIM_CHANNEL_2); //配置TIM3通道2
    HAL_TIM_PWM_Start(&TIM3_Handler, TIM_CHANNEL_2);                           //开启PWM通道2

    TIM3_CH3Handler.OCMode = TIM_OCMODE_PWM2;                                  //模式选择PWM1
    TIM3_CH3Handler.Pulse = arr / 2;                                           //设置比较值,此值用来确定占空比，
    TIM3_CH3Handler.OCPolarity = TIM_OCPOLARITY_LOW;                           //输出比较极性为低
    HAL_TIM_PWM_ConfigChannel(&TIM3_Handler, &TIM3_CH3Handler, TIM_CHANNEL_3); //配置TIM3通道2
    HAL_TIM_PWM_Start(&TIM3_Handler, TIM_CHANNEL_3);                           //开启PWM通道2

    TIM3_CH4Handler.OCMode = TIM_OCMODE_PWM2;                                  //模式选择PWM1
    TIM3_CH4Handler.Pulse = arr / 2;                                           //设置比较值,此值用来确定占空比，
    TIM3_CH4Handler.OCPolarity = TIM_OCPOLARITY_LOW;                           //输出比较极性为低
    HAL_TIM_PWM_ConfigChannel(&TIM3_Handler, &TIM3_CH4Handler, TIM_CHANNEL_4); //配置TIM3通道2
    HAL_TIM_PWM_Start(&TIM3_Handler, TIM_CHANNEL_4);                           //开启PWM通道2
}


//定时器底层驱动，时钟使能，引脚配置
//此函数会被HAL_TIM_PWM_Init()调用
//htim:定时器句柄
void HAL_TIM_PWM_MspInit(TIM_HandleTypeDef *htim)
{
    GPIO_InitTypeDef GPIO_Initure;

    if (htim->Instance == TIM3)
    {
        __HAL_RCC_TIM3_CLK_ENABLE();                                          //使能定时器3
        __HAL_RCC_GPIOC_CLK_ENABLE(); 
        __HAL_RCC_GPIOA_CLK_ENABLE();                                        //开启GPIOB时钟
        GPIO_Initure.Pin = GPIO_PIN_7 | GPIO_PIN_8 | GPIO_PIN_9; //PA6 PA7
        GPIO_Initure.Mode = GPIO_MODE_AF_PP;                                  //复用推完输出
        GPIO_Initure.Pull = GPIO_PULLUP;                                      //上拉
        GPIO_Initure.Speed = GPIO_SPEED_HIGH;                                 //高速
        GPIO_Initure.Alternate = GPIO_AF2_TIM3;                               //复用为TIM3
        HAL_GPIO_Init(GPIOC, &GPIO_Initure);
        GPIO_Initure.Pin = GPIO_PIN_6;
        HAL_GPIO_Init(GPIOA, &GPIO_Initure);
    }
    
}


void PWM_Init(void)
{
    TIM3_PWM_Init(20000 - 1, 108 - 1); //108M/108=1M的计数频率，自动重装载为20000，那么PWM频率为1M/2000=50HZ

    __HAL_TIM_SetCompare(&TIM3_Handler, TIM_CHANNEL_1, 1500);
		__HAL_TIM_SetCompare(&TIM3_Handler, TIM_CHANNEL_2, 1500);
		__HAL_TIM_SetCompare(&TIM3_Handler, TIM_CHANNEL_3, 1500);
		__HAL_TIM_SetCompare(&TIM3_Handler, TIM_CHANNEL_4, 1500);
}
