#ifndef __USART_H
#define __USART_H
#include "stdio.h"	
#include "sys.h" 
#include "math.h"
#include "Motor_Control.h"
#include <string.h>
#include "stmflash.h"



#define FLASH_SAVE_ADDR  0X08070000		//����FLASH �����ַ(����Ϊż��������ֵҪ���ڱ�������ռ��FLASH�Ĵ�С+0X08000000)

//����봮���жϽ��գ��벻Ҫע�����º궨��
void uart_init(u32 bound);

#endif


