#include "sys.h"
#include "delay.h"
#include "usart.h"
#include "led.h"
#include "key.h"
#include "ble_app.h"

/************************************************
 ALIENTEK探索者STM32F407开发板 扩展实验23
 ATK-BLE02蓝牙串口模块透传实验
 技术支持：www.openedv.com
 淘宝店铺：http://eboard.taobao.com
 关注微信公众平台微信号："正点原子"，免费获取STM32资料。
 广州市星翼电子科技有限公司
 作者：正点原子 @ALIENTEK
************************************************/

int main(void)
{

    NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2);//设置系统中断优先级分组2
    delay_init(168);                               //初始化延时函数
    uart_init(115200);		                       //初始化串口波特率为115200
    LED_Init();					                   //初始化LED
    KEY_Init();                                    //KEY初始化
    Ble_Test();                                    //ble测试
}
