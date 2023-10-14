/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * @file           : main.c
  * @brief          : Main program body
  ******************************************************************************
  * @attention
  *
  * Copyright (c) 2022 STMicroelectronics.
  * All rights reserved.
  *
  * This software is licensed under terms that can be found in the LICENSE file
  * in the root directory of this software component.
  * If no LICENSE file comes with this software, it is provided AS-IS.
  *
  ******************************************************************************
  */
/* USER CODE END Header */
/* Includes ------------------------------------------------------------------*/
#include "main.h"
#include "usart.h"
#include "gpio.h"

/* Private includes ----------------------------------------------------------*/
/* USER CODE BEGIN Includes */
#include "string.h"

/* USER CODE END Includes */

/* Private typedef -----------------------------------------------------------*/
/* USER CODE BEGIN PTD */

/* USER CODE END PTD */

/* Private define ------------------------------------------------------------*/
/* USER CODE BEGIN PD */
/* USER CODE END PD */

/* Private macro -------------------------------------------------------------*/
/* USER CODE BEGIN PM */

/* USER CODE END PM */

/* Private variables ---------------------------------------------------------*/

/* USER CODE BEGIN PV */

uint8_t Uart1_RxBuff[1];      //进入中断接收数据的数组
uint8_t Uart3_RxBuff[1];      //进入中断接收数据的数组
uint8_t Uart1_DataBuff[5000]; //保存接收到的数据的数组
uint8_t Uart3_DataBuff[5000]; //保存接收到的数据的数组
int Uart1_RxLine=0;           //接收到的数据长度
int Uart3_RxLine=0;           //接收到的数据长度


uint8_t Tx_str1[] = "hello";
uint8_t Tx_str2[] = "Open\r\n";
uint8_t Tx_str3[] = "Close";
uint8_t Tx_str4[] = "join";
uint8_t Tx_str5[] = "Uart1 Data Finish";
uint8_t Tx_str6[] = "Uart3 Data Finish";

uint8_t Rx_dat = 0;




//void HAL_UART_RxCpltCallback(UART_HandleTypeDef *huart)
//{
//	HAL_UART_Transmit(&huart3, Tx_str4, strlen(Tx_str4), 10000);
//	HAL_GPIO_TogglePin(GPIOF, GPIO_PIN_9);
//	if(huart->Instance == USART3)
//	{
//		if(Rx_dat == 0x01)
//		{
//			HAL_UART_Transmit(&huart3, Tx_str2, strlen(Tx_str2), 10000);
//				printf("%d\r\n",Rx_dat );
//		}
//		
//		if(Rx_dat == 0x02)
//		{
//			HAL_UART_Transmit(&huart3, Tx_str3, strlen(Tx_str3), 10000);
//			
//		}
//		HAL_UART_Receive_IT(&huart3, &Rx_dat, 1);
//	}
//}




void HAL_UART_RxCpltCallback(UART_HandleTypeDef *huart)
{
    if(huart->Instance == USART1)
		{
			Uart1_RxLine++;                      //每接收到一个数据，进入回调数据长度加1
			Uart1_DataBuff[Uart1_RxLine-1]=Uart1_RxBuff[0];  //把每次接收到的数据保存到缓存数组
    
			if(Uart1_RxBuff[0]==0xff)            //接收结束标志位，这个数据可以自定义，根据实际需求，这里只做示例使用，不一定是0xff
			{
					printf("Uart1 RXLen=%d\r\n",Uart1_RxLine); 
					for(int i=0;i<Uart1_RxLine;i++)
					{
							printf("UART1 DataBuff[%d] = ",i);
							printf("0x%x\r\n",Uart1_DataBuff[i]);
					}
								//        　　printf("UART DataBuff[%d] = 0x%x\r\n",i,DataBuff[i]);
					memset(Uart1_DataBuff,0,sizeof(Uart1_DataBuff));  //清空缓存数组
					Uart1_RxLine=0;  //清空接收长度
//					HAL_UART_Transmit(&huart1, (uint8_t *)Tx_str5, strlen(Tx_str5), 10000);
					printf(Tx_str5);
			}
    
			Uart1_RxBuff[0]=0;
			HAL_UART_Receive_IT(&huart1, (uint8_t *)Uart1_RxBuff, 1); //每接收一个数据，就打开一次串口中断接收，否则只会接收一个数据就停止接收
		}
		
		else if(huart->Instance == USART3)
		{
			Uart3_RxLine++;                      //每接收到一个数据，进入回调数据长度加1
			Uart3_DataBuff[Uart3_RxLine-1]=Uart3_RxBuff[0];  //把每次接收到的数据保存到缓存数组
    
			if(Uart3_RxBuff[0]==0xff)            //接收结束标志位，这个数据可以自定义，根据实际需求，这里只做示例使用，不一定是0xff
			{
					printf("Uart3 RXLen=%d\r\n",Uart3_RxLine); 
					for(int i=0;i<Uart3_RxLine;i++)
					{
							printf("UART3 DataBuff[%d] = ",i);
							printf("0x%x\r\n",Uart3_DataBuff[i]);
					}
								//        　　printf("UART DataBuff[%d] = 0x%x\r\n",i,DataBuff[i]);
					memset(Uart3_DataBuff,0,sizeof(Uart3_DataBuff));  //清空缓存数组
					Uart3_RxLine=0;  //清空接收长度
					HAL_UART_Transmit(&huart3, (uint8_t *)Tx_str6, strlen(Tx_str6), 10000);
			}
    
			Uart3_RxBuff[0]=0;
			HAL_UART_Receive_IT(&huart3, (uint8_t *)Uart3_RxBuff, 1); //每接收一个数据，就打开一次串口中断接收，否则只会接收一个数据就停止接收
		}
}


/* USER CODE END PV */

/* Private function prototypes -----------------------------------------------*/
void SystemClock_Config(void);
/* USER CODE BEGIN PFP */

/* USER CODE END PFP */

/* Private user code ---------------------------------------------------------*/
/* USER CODE BEGIN 0 */

/* USER CODE END 0 */

/**
  * @brief  The application entry point.
  * @retval int
  */
int main(void)
{
  /* USER CODE BEGIN 1 */
	SystemClock_Config();
  /* USER CODE END 1 */

  /* MCU Configuration--------------------------------------------------------*/

  /* Reset of all peripherals, Initializes the Flash interface and the Systick. */
  HAL_Init();

  /* USER CODE BEGIN Init */

  /* USER CODE END Init */

  /* Configure the system clock */
  SystemClock_Config();

  /* USER CODE BEGIN SysInit */

  /* USER CODE END SysInit */

  /* Initialize all configured peripherals */
  MX_GPIO_Init();
  MX_USART3_UART_Init();
  MX_USART1_UART_Init();
  /* USER CODE BEGIN 2 */
	HAL_UART_Transmit(&huart3, (uint8_t *)Tx_str1, strlen(Tx_str1), 10000);
	HAL_UART_Receive_IT(&huart1, (uint8_t *)Uart1_RxBuff, 1);	
	HAL_UART_Receive_IT(&huart3, (uint8_t *)Uart3_RxBuff, 1);		
//	printf("begin");
  /* USER CODE END 2 */

  /* Infinite loop */
  /* USER CODE BEGIN WHILE */
  while (1)
  {
    /* USER CODE END WHILE */

    /* USER CODE BEGIN 3 */
		
  }
  /* USER CODE END 3 */
}

/**
  * @brief System Clock Configuration
  * @retval None
  */
void SystemClock_Config(void)
{
  RCC_OscInitTypeDef RCC_OscInitStruct = {0};
  RCC_ClkInitTypeDef RCC_ClkInitStruct = {0};

  /** Configure the main internal regulator output voltage
  */
  __HAL_RCC_PWR_CLK_ENABLE();
  __HAL_PWR_VOLTAGESCALING_CONFIG(PWR_REGULATOR_VOLTAGE_SCALE1);

  /** Initializes the RCC Oscillators according to the specified parameters
  * in the RCC_OscInitTypeDef structure.
  */
  RCC_OscInitStruct.OscillatorType = RCC_OSCILLATORTYPE_HSE;
  RCC_OscInitStruct.HSEState = RCC_HSE_ON;
  RCC_OscInitStruct.PLL.PLLState = RCC_PLL_ON;
  RCC_OscInitStruct.PLL.PLLSource = RCC_PLLSOURCE_HSE;
  RCC_OscInitStruct.PLL.PLLM = 4;
  RCC_OscInitStruct.PLL.PLLN = 168;
  RCC_OscInitStruct.PLL.PLLP = RCC_PLLP_DIV2;
  RCC_OscInitStruct.PLL.PLLQ = 4;
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
  RCC_ClkInitStruct.APB1CLKDivider = RCC_HCLK_DIV4;
  RCC_ClkInitStruct.APB2CLKDivider = RCC_HCLK_DIV2;

  if (HAL_RCC_ClockConfig(&RCC_ClkInitStruct, FLASH_LATENCY_5) != HAL_OK)
  {
    Error_Handler();
  }
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
