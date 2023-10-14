#include "sys.h"
#include "delay.h"
#include "usart.h"
#include "led.h"
#include "key.h"
#include "lcd.h"
#include "can.h"
#include "motor.h"
#include "motor_pid.h"
#include "timer.h"

/***************************************************************************************************
 描述：M3508 PID控制实验
 功能：通过两个按键控制M3508电机的速度，采用CAN通信进行电机速度闭环控制，采用查询方式接受CAN报文
 测试：正点原子探索者F4板子/阿波罗F7板子（本例程是F4，F7除了库和头文件不一样其他都相同
 注意：使用F1，CAN驱动部分会不一样，对应的需要修改motor.c里面的
			 读取数据函数(get_moto_measure)和
			 写入数据的函数(set_moto_current)
 作者：何为其然 @CSDN (主页：https://blog.csdn.net/qq_30267617)
 时间：2021-08-12
***************************************************************************************************/

int main(void)
{
	u8 key;
	s8 key_cnt;
	u8 i;
	u16 retry;
	u8 flag_motor[motor_num];		   //电机信息接受成功标志
	pid_t pid_speed[motor_num];		   //电机速度PID环
	float set_speed_temp;			   //加减速时的临时设定速度
	int16_t delta;					   //设定速度与实际速度的差值
	int16_t max_speed_change = 1000;   //电机单次最大变化速度，加减速用
									   // 500经测试差不多是最大加速区间，即从零打到最大速度不异常的最大值
	static float set_speed[motor_num]; //电机速度全局变量

	HAL_Init();																	 //初始化HAL库
	Stm32_Clock_Init(360, 25, 2, 8);											 //设置时钟,180Mhz
	delay_init(180);															 //初始化延时函数
	uart_init(115200);															 //初始化USART
	LED_Init();																	 //初始化LED
	KEY_Init();																	 //初始化按键
	CAN1_Mode_Init(CAN_SJW_1TQ, CAN_BS2_6TQ, CAN_BS1_8TQ, 3, CAN_MODE_LOOPBACK); //CAN初始化,波特率1000Kbps
	PWM_Init();
	//PID初始化
	for (i = 0; i < 4; i++)
	{
		PID_struct_init(&pid_speed[i], POSITION_PID, 16384, 16384, 1.5f, 0.1f, 0.0f); //4 motos angular rate closeloop.
	}

	// 等待CAN通讯成功
	while (!get_moto_measure(&moto_info, &CAN1_Handler))
	{
		LED0 = ~LED0;
		delay_ms(200);
	}

	while (1)
	{

		// 按键控制速度加减
		key = KEY_Scan(0);
		if (key == KEY0_PRES) //KEY0按下,发送一次数据
		{
			key_cnt++;
		}
		else if (key == KEY1_PRES) //WK_UP按下，改变CAN的工作模式
		{
			key_cnt--;
		}

#if defined CAN_CONTROL
		if (key_cnt < -16)
			key_cnt = -16;
		if (key_cnt > 16)
			key_cnt = 16;
		set_speed[0] = set_speed[1] = set_speed[2] = set_speed[3] = key_cnt * 500; // -8000-8000，双向转

		//PID采样，获取电机数据
		retry = 0;
		flag_motor[0] = flag_motor[1] = flag_motor[2] = flag_motor[3] = 0;
		while (retry < 20)
		{
			get_moto_measure(&moto_info, &CAN1_Handler);
			if (CAN1_Handler.pRxMsg->StdId == 0x201)
			{
				moto_chassis[0] = moto_info;
				flag_motor[0] = 1;
			}
			if (CAN1_Handler.pRxMsg->StdId == 0x202)
			{
				moto_chassis[1] = moto_info;
				flag_motor[1] = 1;
			}
			if (CAN1_Handler.pRxMsg->StdId == 0x203)
			{
				moto_chassis[2] = moto_info;
				flag_motor[2] = 1;
			}
			if (CAN1_Handler.pRxMsg->StdId == 0x204)
			{
				moto_chassis[3] = moto_info;
				flag_motor[3] = 1;
			}

			if (flag_motor[0] && flag_motor[1] && flag_motor[2] && flag_motor[3])
				break;
			else
				retry++;
		}

		//更新状态，上传到上位机
		//				CAN_MotoInfo.motor1.speed_rpm = moto_chassis[0].speed_rpm;
		//				CAN_MotoInfo.motor2.speed_rpm = moto_chassis[1].speed_rpm;
		//				CAN_MotoInfo.motor3.speed_rpm = moto_chassis[2].speed_rpm;
		//				CAN_MotoInfo.motor1.given_current = moto_chassis[0].given_current;
		//				CAN_MotoInfo.motor2.given_current = moto_chassis[1].given_current;
		//				CAN_MotoInfo.motor3.given_current = moto_chassis[2].given_current;

		//PID计算输出，写5 多算了一个，避免最后一个电机不计算，这里是一个未知bug
		for (i = 0; i < 5; i++)
		{
			//PID计算

			// 无加减速
			//pid_calc(&pid_speed[i], (float)moto_chassis[i].speed_rpm, set_speed[i]);

			//加减速
			delta = (int16_t)set_speed[i] - moto_chassis[i].speed_rpm;
			if (delta > max_speed_change)
				set_speed_temp = (float)(moto_chassis[i].speed_rpm + max_speed_change);
			else if (delta < -max_speed_change)
				set_speed_temp = (float)(moto_chassis[i].speed_rpm - max_speed_change);
			else
				set_speed_temp = set_speed[i];
			pid_calc(&pid_speed[i], (float)moto_chassis[i].speed_rpm, set_speed_temp);
		}

		// 通过串口调试助手观察实际速度与设定速度
		printf("PID out: %f\r\n", pid_speed[0].pos_out);
		printf("real speed: %d \r\n", moto_chassis[0].speed_rpm);
		printf("set speed: %f \r\n", set_speed[0]);

		//PID 输出
		set_moto_current(&CAN1_Handler, 0x200, (s16)(pid_speed[0].pos_out),
						 (s16)(pid_speed[1].pos_out),
						 (s16)(pid_speed[2].pos_out),
						 (s16)(pid_speed[3].pos_out));

#elif defined PWM_CONTROL
		if (key_cnt < -10)
			key_cnt = -10;
		if (key_cnt > 10)
			key_cnt = 10;
		set_speed[0] = set_speed[1] = set_speed[2] = set_speed[3] = key_cnt * 50; // -500-500,双向转
		__HAL_TIM_SetCompare(&htim5, TIM_CHANNEL_1, 1500+set_speed[0]);				// PWM范围1000-2000,双向转
		__HAL_TIM_SetCompare(&htim5, TIM_CHANNEL_2, 1500+set_speed[1]);
		__HAL_TIM_SetCompare(&htim5, TIM_CHANNEL_3, 1500+set_speed[2]);
		__HAL_TIM_SetCompare(&htim5, TIM_CHANNEL_4, 1500+set_speed[3]);

#endif

		delay_ms(5); // 采样周期
	}
}
