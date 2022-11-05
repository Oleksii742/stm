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

/* Private includes ----------------------------------------------------------*/
/* USER CODE BEGIN Includes */

#define  PC8_ON  HAL_GPIO_WritePin(GPIOC, GPIO_PIN_8,GPIO_PIN_SET)
#define  PC8_OFF HAL_GPIO_WritePin(GPIOC, GPIO_PIN_8,GPIO_PIN_RESET)	;



  uint8_t chas;
  uint8_t  minuta;
  uint8_t sekunda;

  uint32_t E;
  uint8_t x;
  char P;


  uint8_t TIMER_LED;
  uint8_t TIMER_SEKUND;
  uint8_t TIMER_MINUT;
  uint8_t TIMER_CHASOV;

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
TIM_HandleTypeDef htim1;
TIM_HandleTypeDef htim2;

/* USER CODE BEGIN PV */

/* USER CODE END PV */

/* Private function prototypes -----------------------------------------------*/
void SystemClock_Config(void);
static void MX_GPIO_Init(void);
static void MX_TIM1_Init(void);
static void MX_TIM2_Init(void);
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
  MX_TIM1_Init();
  MX_TIM2_Init();
  /* USER CODE BEGIN 2 */

//A = 128;
//B = 255;

//uint64_t B = (uint64_t)1 << 63;




/*C = 1 << 14;
C |= 1 << 2;
C |= C + 1;

C = A << 8|B;




E = C;
D = C >> 8;*/



/*C = 0xAB << 8|0xAC;

A = 0xCD << 8|0xCE;

B = 0xBF << 8|0xCA;

D = 0xEA << 8|0xEB*/

//A = 0xEB << 24|0xAC << 16|0xBC << 8|0xED;


 /* B = 0xAC << 56LL;
  B = 0xBD << 48;
  B = 0xEF << 40;
  B = 0xDA << 32;
  B = 0xFC << 24;
  B = 0xCE << 16;
  B = 0xAD << 8;*/


  /*C = 0xDA << 24;
  C = 0xFE << 16;
  C = 0xBC << 8;

  C = 0xDA << 24|0xFE << 16|0xBC << 8|0xEB;*/

//A = sqrt(1049600);



 /* uint16_t sot = 0;
  uint16_t des = 0;
  uint16_t Ed = 0;

  sot = 123/100;
  des = 123%10;
  Ed = 123%10;*/



/*uint32_t tsh = 0;
uint32_t sot = 0;
uint32_t des = 0;
uint32_t Ed = 0;

tsh = 1024/1000;

sot = 1024 - (tsh*1000);
sot = sot/100;

des = 1024 - ((tsh*1000) - sot*100);
des = des/10;

Ed = 1024 - (((tsh*1000) - sot*100) - des*10);
Ed = Ed/10;*/


  /*uint8_t B = 1;
  char*P = &B;*/




 /* for(uint8_t x = 0; x < 16; x ++)
  {
	  GPIOC->BSRR = (uint32_t)1 << x;
	  GPIOC->BSRR = (uint32_t)1 << (x+16);
  }*/







      //GPIOA->BSRR = (uint32_t)1 << x;
      //GPIOA->BSRR = (uint32_t)1 << (x+16);

  /*char*P = 0;
  uint8_t X = 1;
  P = &X;*/

HAL_TIM_Base_Start_IT(&htim1);
HAL_TIM_Base_Start(&htim1);



//HAL_TIM_Base_Start_IT(&htim2);
//HAL_TIM_Base_Start(&htim2);




  /* USER CODE END 2 */

  /* Infinite loop */
  /* USER CODE BEGIN WHILE */
  while (1)
  {

/*	  for(x = 9; x > 7; x--)
	  {
       GPIOC->BSRR = (uint32_t)1 << x;
       HAL_Delay(100);
       GPIOC->BSRR = (uint32_t)1 << (x+16);
       HAL_Delay(100);
	  }

	  for(x = 8; x < 15; x ++)
	   {
		  if(x <= 12)
		   {
	        GPIOA->BSRR = (uint32_t)1 << x;
	        HAL_Delay(100);
	        GPIOA->BSRR = (uint32_t)1 << (x+16);
	        HAL_Delay(100);
		   }

		      if(x == 12)
		 	   {
		 	    x = 15;
		 	     GPIOA->BSRR = (uint32_t)1 << x;
		 	     HAL_Delay(100);
				 GPIOA->BSRR = (uint32_t)1 << (x+16);
				 HAL_Delay(100);
		 	   }

	   }

*/





if(TIMER_LED == 1)
{


  PC8_ON;
  HAL_Delay(50);
  PC8_OFF;
  TIMER_LED = 0;


  x++;

  if(x == 5)
   {
	  HAL_GPIO_WritePin(GPIOC, GPIO_PIN_9,GPIO_PIN_SET)	;
	  HAL_Delay(50);
	  HAL_GPIO_WritePin(GPIOC, GPIO_PIN_9,GPIO_PIN_RESET);
	  x = 0;
   }


}





if(TIMER_SEKUND == 1 )
{
   TIMER_SEKUND = 0;

   if(TIMER_MINUT == 1, TIMER_SEKUND <= 59)

   {

	   TIMER_MINUT = 0;
	   x ++;

	  if(TIMER_CHASOV == 1, TIMER_MINUT <=59)

   {

		  TIMER_CHASOV = 0;
          x ++;
   }

	  	  //if(B &0x8000000000000000)
	  	  // {
	  	 	//  HAL_GPIO_WritePin(GPIOC,GPIO_PIN_8 , GPIO_PIN_SET);
	  	  // }
	  	   //else
	  	  // {
	  	 //	  HAL_GPIO_WritePin(GPIOC,GPIO_PIN_8 , GPIO_PIN_RESET);
	  	//   }
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

  /** Initializes the RCC Oscillators according to the specified parameters
  * in the RCC_OscInitTypeDef structure.
  */
  RCC_OscInitStruct.OscillatorType = RCC_OSCILLATORTYPE_HSI;
  RCC_OscInitStruct.HSIState = RCC_HSI_ON;
  RCC_OscInitStruct.HSICalibrationValue = RCC_HSICALIBRATION_DEFAULT;
  RCC_OscInitStruct.PLL.PLLState = RCC_PLL_ON;
  RCC_OscInitStruct.PLL.PLLSource = RCC_PLLSOURCE_HSI_DIV2;
  RCC_OscInitStruct.PLL.PLLMUL = RCC_PLL_MUL6;
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
  RCC_ClkInitStruct.APB1CLKDivider = RCC_HCLK_DIV1;
  RCC_ClkInitStruct.APB2CLKDivider = RCC_HCLK_DIV1;

  if (HAL_RCC_ClockConfig(&RCC_ClkInitStruct, FLASH_LATENCY_0) != HAL_OK)
  {
    Error_Handler();
  }
}

/**
  * @brief TIM1 Initialization Function
  * @param None
  * @retval None
  */
static void MX_TIM1_Init(void)
{

  /* USER CODE BEGIN TIM1_Init 0 */

  /* USER CODE END TIM1_Init 0 */

  TIM_ClockConfigTypeDef sClockSourceConfig = {0};
  TIM_MasterConfigTypeDef sMasterConfig = {0};

  /* USER CODE BEGIN TIM1_Init 1 */

  /* USER CODE END TIM1_Init 1 */
  htim1.Instance = TIM1;
  htim1.Init.Prescaler = 23999;
  htim1.Init.CounterMode = TIM_COUNTERMODE_UP;
  htim1.Init.Period = 999;
  htim1.Init.ClockDivision = TIM_CLOCKDIVISION_DIV1;
  htim1.Init.RepetitionCounter = 0;
  htim1.Init.AutoReloadPreload = TIM_AUTORELOAD_PRELOAD_DISABLE;
  if (HAL_TIM_Base_Init(&htim1) != HAL_OK)
  {
    Error_Handler();
  }
  sClockSourceConfig.ClockSource = TIM_CLOCKSOURCE_INTERNAL;
  if (HAL_TIM_ConfigClockSource(&htim1, &sClockSourceConfig) != HAL_OK)
  {
    Error_Handler();
  }
  sMasterConfig.MasterOutputTrigger = TIM_TRGO_RESET;
  sMasterConfig.MasterSlaveMode = TIM_MASTERSLAVEMODE_DISABLE;
  if (HAL_TIMEx_MasterConfigSynchronization(&htim1, &sMasterConfig) != HAL_OK)
  {
    Error_Handler();
  }
  /* USER CODE BEGIN TIM1_Init 2 */

  /* USER CODE END TIM1_Init 2 */

}

/**
  * @brief TIM2 Initialization Function
  * @param None
  * @retval None
  */
static void MX_TIM2_Init(void)
{

  /* USER CODE BEGIN TIM2_Init 0 */

  /* USER CODE END TIM2_Init 0 */

  TIM_ClockConfigTypeDef sClockSourceConfig = {0};
  TIM_MasterConfigTypeDef sMasterConfig = {0};

  /* USER CODE BEGIN TIM2_Init 1 */

  /* USER CODE END TIM2_Init 1 */
  htim2.Instance = TIM2;
  htim2.Init.Prescaler = 23999;
  htim2.Init.CounterMode = TIM_COUNTERMODE_UP;
  htim2.Init.Period = 4999;
  htim2.Init.ClockDivision = TIM_CLOCKDIVISION_DIV1;
  htim2.Init.AutoReloadPreload = TIM_AUTORELOAD_PRELOAD_DISABLE;
  if (HAL_TIM_Base_Init(&htim2) != HAL_OK)
  {
    Error_Handler();
  }
  sClockSourceConfig.ClockSource = TIM_CLOCKSOURCE_INTERNAL;
  if (HAL_TIM_ConfigClockSource(&htim2, &sClockSourceConfig) != HAL_OK)
  {
    Error_Handler();
  }
  sMasterConfig.MasterOutputTrigger = TIM_TRGO_RESET;
  sMasterConfig.MasterSlaveMode = TIM_MASTERSLAVEMODE_DISABLE;
  if (HAL_TIMEx_MasterConfigSynchronization(&htim2, &sMasterConfig) != HAL_OK)
  {
    Error_Handler();
  }
  /* USER CODE BEGIN TIM2_Init 2 */

  /* USER CODE END TIM2_Init 2 */

}

/**
  * @brief GPIO Initialization Function
  * @param None
  * @retval None
  */
static void MX_GPIO_Init(void)
{
  GPIO_InitTypeDef GPIO_InitStruct = {0};

  /* GPIO Ports Clock Enable */
  __HAL_RCC_GPIOC_CLK_ENABLE();
  __HAL_RCC_GPIOA_CLK_ENABLE();

  /*Configure GPIO pin Output Level */
  HAL_GPIO_WritePin(GPIOC, GPIO_PIN_8|GPIO_PIN_9, GPIO_PIN_RESET);

  /*Configure GPIO pin Output Level */
  HAL_GPIO_WritePin(GPIOA, GPIO_PIN_8|GPIO_PIN_9|GPIO_PIN_10|GPIO_PIN_11
                          |GPIO_PIN_12|GPIO_PIN_15, GPIO_PIN_RESET);

  /*Configure GPIO pins : PC8 PC9 */
  GPIO_InitStruct.Pin = GPIO_PIN_8|GPIO_PIN_9;
  GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
  HAL_GPIO_Init(GPIOC, &GPIO_InitStruct);

  /*Configure GPIO pins : PA8 PA9 PA10 PA11
                           PA12 PA15 */
  GPIO_InitStruct.Pin = GPIO_PIN_8|GPIO_PIN_9|GPIO_PIN_10|GPIO_PIN_11
                          |GPIO_PIN_12|GPIO_PIN_15;
  GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
  HAL_GPIO_Init(GPIOA, &GPIO_InitStruct);

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
