/*
作者：Huxiaoan
日期：2021年9月15日

F1版本电机控速。串口接收速度值，速度值后需要有一个回车才能被识别到

F1版本与F4版本在CAN通信上有较大区别，请注意。


*/


#include "main.h"


#include "usart.h"
#include "bsp_can.h"
#include <math.h>
#include "pid.h"
#define MOTOR_NUM 1

CAN_HandleTypeDef hcan;
UART_HandleTypeDef huart1;
CAN_TxHeaderTypeDef   TxHeader;
CAN_RxHeaderTypeDef   RxHeader;
uint8_t               TxData[8];
uint8_t               RxData[8];
uint32_t							TxMailBox;



PID_TypeDef moto_pid[MOTOR_NUM];



void SystemClock_Config(void);
static void MX_GPIO_Init(void);
static void MX_CAN_Init(void);
static void MX_USART1_UART_Init(void);

int main(void)
{
	
	int len=0;
	int spd_set=0;
	int spd_target=0;
	int16_t speed=0,pos=0;
	u8 tem=0;
	
//////////////////////////////////////
  HAL_Init();
  SystemClock_Config();
  MX_GPIO_Init();
  MX_CAN_Init();
  MX_USART1_UART_Init();
	uart_init(&huart1);
/////////////////////////////////////

	for(int g=0;g<MOTOR_NUM;g++){
	pid_init(moto_pid+g);
	}
	
	moto_pid->f_param_init(moto_pid,PID_Speed,16384,5000,5,0,8000,0,10.0,0.15,0);
	
	set_moto_current(&hcan,&TxHeader,TxData,&TxMailBox,10000,0,0,0 );
	
//	printf("[*]初始化完毕。\r\n");
	printf("+++++++++++++++++++++++++++++++++++++++++++++++++++++");
	
	uint32_t last_time=0;
	
	
  while (1)
  {
		
	
		if(HAL_GetTick()-last_time>500){
			last_time=HAL_GetTick();
		printf("RUNNING!\r\n");
		pos=(RxData[0]<<8)|(RxData[1]);
				speed=(RxData[2]<<8)|(RxData[3]);
				tem=RxData[6];
				printf("\r\n电机转速：%d\r\n",speed);
				printf("\r\n电机温度：%d\r\n",tem);
				printf("\r\n电机角度：%.1lf\r\n",(pos/8191.0)*360);
		}
		
		
				if(USART_RX_STA&0x8000)
		{//串口接收处理					   
			len=USART_RX_STA&0x3fff;//得到此次接收到的数据长度
			
			if(USART_RX_BUF[0]=='-'){//加入正反转支持
				for(int i=1;i<len;i++){
				spd_set-=(USART_RX_BUF[i]-'0')*pow(10,len-i-1);
			}
		}
		else{
			for(int i=0;i<len;i++){
				spd_set+=(USART_RX_BUF[i]-'0')*pow(10,len-i-1);
				
			}
		}
			
			printf("\r\n发送的消息为:\r\n");
			HAL_UART_Transmit(&huart1,(uint8_t*)USART_RX_BUF,len,1000);	//发送接收到的数据
			while(__HAL_UART_GET_FLAG(&huart1,UART_FLAG_TC)!=SET);		//等待发送结束
			printf("\r\n向电调写入值:%d\r\n",spd_set);//插入换行
			
			spd_target=spd_set;
			USART_RX_STA=0;
			spd_set=0;
		}
		
		
		
		for(int g=0;g<MOTOR_NUM;g++){//
			
			moto_pid[g].target=spd_target;
			
			moto_pid[g].f_cal_pid(moto_pid+g,moto_chassis[g].speed_rpm);

		}

		set_moto_current(&hcan,&TxHeader,TxData,&TxMailBox,moto_pid[0].output,0,0,0);//发送电流数据
//    set_moto_current(&hcan,&TxHeader,TxData,&TxMailBox,5000,0,0,0);//发送电流数据
		
		
		HAL_Delay(1);//过快引起can发送失败
		
		
		
		
		
		
		
		
		
		
  }

}

/**
  * @brief System Clock Configuration
  * @retval None
  */
void SystemClock_Config(void)
{
  RCC_OscInitTypeDef RCC_OscInitStruct = {0};
  RCC_ClkInitTypeDef RCC_ClkInitStruct = {0};

  /** Initializes the RCC Oscillators according to the specified parameters
  * in the RCC_OscInitTypeDef structure.
  */
  RCC_OscInitStruct.OscillatorType = RCC_OSCILLATORTYPE_HSE;
  RCC_OscInitStruct.HSEState = RCC_HSE_ON;
  RCC_OscInitStruct.HSEPredivValue = RCC_HSE_PREDIV_DIV1;
  RCC_OscInitStruct.HSIState = RCC_HSI_ON;
  RCC_OscInitStruct.PLL.PLLState = RCC_PLL_ON;
  RCC_OscInitStruct.PLL.PLLSource = RCC_PLLSOURCE_HSE;
  RCC_OscInitStruct.PLL.PLLMUL = RCC_PLL_MUL9;
  if (HAL_RCC_OscConfig(&RCC_OscInitStruct) != HAL_OK)
  {
    Error_Handler();
  }
  /** Initializes the CPU, AHB and APB buses clocks
  */
  RCC_ClkInitStruct.ClockType = RCC_CLOCKTYPE_HCLK|RCC_CLOCKTYPE_SYSCLK
                              |RCC_CLOCKTYPE_PCLK1|RCC_CLOCKTYPE_PCLK2;
  RCC_ClkInitStruct.SYSCLKSource = RCC_SYSCLKSOURCE_PLLCLK;
  RCC_ClkInitStruct.AHBCLKDivider = RCC_SYSCLK_DIV1;
  RCC_ClkInitStruct.APB1CLKDivider = RCC_HCLK_DIV2;
  RCC_ClkInitStruct.APB2CLKDivider = RCC_HCLK_DIV1;

  if (HAL_RCC_ClockConfig(&RCC_ClkInitStruct, FLASH_LATENCY_2) != HAL_OK)
  {
    Error_Handler();
  }
}

/**
  * @brief CAN Initialization Function
  * @param None
  * @retval None
  */
static void MX_CAN_Init(void)
{

	CAN_FilterTypeDef filterInitType;
	
	
  hcan.Instance = CAN1;
  hcan.Init.Prescaler = 2;
  hcan.Init.Mode = CAN_MODE_NORMAL;
  hcan.Init.SyncJumpWidth = CAN_SJW_1TQ;
  hcan.Init.TimeSeg1 = CAN_BS1_9TQ;
  hcan.Init.TimeSeg2 = CAN_BS2_8TQ;//不能太小！！！
  hcan.Init.TimeTriggeredMode = DISABLE;
  hcan.Init.AutoBusOff = ENABLE;
  hcan.Init.AutoWakeUp = ENABLE;
  hcan.Init.AutoRetransmission = DISABLE;
  hcan.Init.ReceiveFifoLocked = DISABLE;
  hcan.Init.TransmitFifoPriority = DISABLE;
  if (HAL_CAN_Init(&hcan) != HAL_OK)
  {
    Error_Handler();
  }
//filter
	filterInitType.FilterBank=0;
	filterInitType.FilterMode=CAN_FILTERMODE_IDMASK;
	filterInitType.FilterScale = CAN_FILTERSCALE_32BIT;
  filterInitType.FilterIdHigh = 0x0000;
  filterInitType.FilterIdLow = 0x0000;
  filterInitType.FilterMaskIdHigh = 0x0000;
  filterInitType.FilterMaskIdLow = 0x0000;
  filterInitType.FilterFIFOAssignment = CAN_RX_FIFO0;
  filterInitType.FilterActivation = ENABLE;
  filterInitType.SlaveStartFilterBank = 14;
  if (HAL_CAN_ConfigFilter(&hcan, &filterInitType) != HAL_OK)
  {
    /* Filter configuration Error */
    Error_Handler();
  }
	
	  if (HAL_CAN_Start(&hcan) != HAL_OK)
  {
    /* Start Error */
    Error_Handler();
  }
	
	  if (HAL_CAN_ActivateNotification(&hcan, CAN_IT_RX_FIFO0_MSG_PENDING) != HAL_OK)
  {
    /* Notification Error */
    Error_Handler();
  }//接收中断回调
	
	
	
}

/**
  * @brief USART1 Initialization Function
  * @param None
  * @retval None
  */
static void MX_USART1_UART_Init(void)
{

  /* USER CODE BEGIN USART1_Init 0 */

  /* USER CODE END USART1_Init 0 */

  /* USER CODE BEGIN USART1_Init 1 */

  /* USER CODE END USART1_Init 1 */
  huart1.Instance = USART1;
  huart1.Init.BaudRate = 115200;
  huart1.Init.WordLength = UART_WORDLENGTH_8B;
  huart1.Init.StopBits = UART_STOPBITS_1;
  huart1.Init.Parity = UART_PARITY_NONE;
  huart1.Init.Mode = UART_MODE_TX_RX;
  huart1.Init.HwFlowCtl = UART_HWCONTROL_NONE;
  huart1.Init.OverSampling = UART_OVERSAMPLING_16;
  if (HAL_UART_Init(&huart1) != HAL_OK)
  {
    Error_Handler();
  }
  /* USER CODE BEGIN USART1_Init 2 */

  /* USER CODE END USART1_Init 2 */

}

/**
  * @brief GPIO Initialization Function
  * @param None
  * @retval None
  */
static void MX_GPIO_Init(void)
{

  /* GPIO Ports Clock Enable */
  __HAL_RCC_GPIOA_CLK_ENABLE();

}

/* USER CODE BEGIN 4 */

/* USER CODE END 4 */

/**
  * @brief  This function is executed in case of error occurrence.
  * @retval None
  */
void Error_Handler(void)
{
  /* USER CODE BEGIN Error_Handler_Debug */
  /* User can add his own implementation to report the HAL error return state */
  __disable_irq();
	printf("ERROR!!\r\n");
  while (1)
  {
		
  }
  /* USER CODE END Error_Handler_Debug */
}

#ifdef  USE_FULL_ASSERT
/**
  * @brief  Reports the name of the source file and the source line number
  *         where the assert_param error has occurred.
  * @param  file: pointer to the source file name
  * @param  line: assert_param error line source number
  * @retval None
  */
void assert_failed(uint8_t *file, uint32_t line)
{
  /* USER CODE BEGIN 6 */
  /* User can add his own implementation to report the file name and line number,
     ex: printf("Wrong parameters value: file %s on line %d\r\n", file, line) */
  /* USER CODE END 6 */
}
#endif /* USE_FULL_ASSERT */

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
