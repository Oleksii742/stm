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

uint8_t addr;
uint8_t A;
uint8_t B;
uint8_t C;
uint8_t Error;
uint16_t Byte_Num;
#include "stm32f1xx_hal_flash.h"
#include "string.h"
#define DATA_H HAL_GPIO_WritePin(GPIOC,GPIO_PIN_13,GPIO_PIN_SET)
#define DATA_L HAL_GPIO_WritePin(GPIOC,GPIO_PIN_13,GPIO_PIN_RESET)

#define CS_H HAL_GPIO_WritePin(GPIOC,GPIO_PIN_14,GPIO_PIN_SET)
#define CS_L HAL_GPIO_WritePin(GPIOC,GPIO_PIN_14,GPIO_PIN_RESET)

#define CLK_H HAL_GPIO_WritePin(GPIOC,GPIO_PIN_15,GPIO_PIN_SET)
#define CLK_L HAL_GPIO_WritePin(GPIOC,GPIO_PIN_15,GPIO_PIN_RESET)
char Massiv [100];

char Massiv2[100];


char U [30];

char K [10];

char R [10];

char N [20];
char Q [20];
char W [20];
char E [20];
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
SPI_HandleTypeDef hspi1;

/* USER CODE BEGIN PV */

/* USER CODE END PV */

/* Private function prototypes -----------------------------------------------*/
void SystemClock_Config(void);
static void MX_GPIO_Init(void);
static void MX_SPI1_Init(void);
/* USER CODE BEGIN PFP */

/* USER CODE END PFP */

/* Private user code ---------------------------------------------------------*/
/* USER CODE BEGIN 0 */
void FLASH_WriteByte(uint8_t Error)
{
	/*FLASH_UNLOCK();
	FLASH_ErasePage(Error);
	addr = HAL_FLASH_Unlock() == 2 + 2;
			FLASH_ErasePage(0x7f);
			 HAL_FLASH_Lock();
}

void SEND_SPI_CMD(uint8_t data)
{



	  for(uint8_t i = 0; i < 8; i++)
	   {

          CLK_H;
          HAL_Delay(1);

		  if(data & 0x80)
		  {
           DATA_H;
		  }
		  else
		  {
			DATA_L;
		  }



		  data = data << 1;

		  CLK_L;
          HAL_Delay(1);
	   }


}

void SEND_DATA(uint8_t ADDR, uint8_t data)
{


	  CS_L;

	   SEND_SPI_CMD(ADDR);
	   HAL_Delay(1);
	   SEND_SPI_CMD(data);

	  CS_H;

}



void SPI_SEND(volatile uint8_t ADDR, volatile uint8_t DATA)
{
	HAL_SPI_Transmit(&hspi1, (uint8_t*)&ADDR, 1, 100);
	HAL_SPI_Transmit(&hspi1, (uint8_t*)&DATA, 1, 100);
	HAL_Delay(1);
}

void MAX7219_Send_CMD(volatile uint8_t ADDR, volatile uint8_t DATA)
  {
  	CS_L;
  	HAL_Delay(2);
  	SPI_SEND(ADDR,DATA);
  	CS_H;
  	HAL_Delay(2);*/
  }

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
  MX_SPI1_Init();
  /* USER CODE BEGIN 2 */
  /*CS_H;
 	  MAX7219_Send_CMD(0x0F,0x00);//test
 	  MAX7219_Send_CMD(0x0C,0x01);//off
 	  MAX7219_Send_CMD(0x0B,0x02);//all segment
 	  MAX7219_Send_CMD(0x09,0xFF);//decode
 	  MAX7219_Send_CMD(0x0A,0);//Brighr

 	 CS_H;
 		  MAX7219_Send_CMD(0x0F,0x00);//test
 		  MAX7219_Send_CMD(0x0C,0x01);//off
 		  MAX7219_Send_CMD(0x0B,0x02);//all segment
 		  MAX7219_Send_CMD(0x09,0xFF);//decode
 		  MAX7219_Send_CMD(0x0A,0);//Brighr

 		 CS_H;
 			  MAX7219_Send_CMD(0x0F,0x00);//test
 			  MAX7219_Send_CMD(0x0C,0x01);//off
 			  MAX7219_Send_CMD(0x0B,0x02);//all segment
 			  MAX7219_Send_CMD(0x09,0xFF);//decode
 			  MAX7219_Send_CMD(0x0A,0);//Brighr
*/
  /* USER CODE END 2 */

  /* Infinite loop */
  /* USER CODE BEGIN WHILE */
  while (1)
  {


	  /////////////////////////////////////////

	  //strcpy - (куда,откуда);
	  //memcpy - (куда,откуда,сколько);

	 // strcpy(Massiv, "wedrftgyhujikoldrftghyjukilop;edrftgh_good");
	 // strcpy(Massiv+strlen(Massiv)+1, "d");


	 // memcpy(Massiv,"_day",4);// в начало строки добавить
     // strcat(Massiv,"_day");// в конец строки добавить
	 /// A = strlen(Massiv);

	//A =   strcmp(Massiv,Massiv2);


	  //strcpy(Massiv2, Massiv+(strlen(Massiv)-5));

  strcpy(U, "AT+CGDCONT = 1,2,3\n");

  strcpy(K, U+(strlen(U)-6));

  memcpy(R, U, 3);

  memcpy(N, U+3,7);

  memcpy(Q, U+13,1);

  memcpy(W, U+15,1);

  memcpy(E, U+17,1);
	/*  SEND_DATA(0x0F,0x00);
	  SEND_DATA(0x0C,0x01);
	  SEND_DATA(0x0B,0x02);
	  SEND_DATA(0x09,0xFF);
	  SEND_DATA(0x0A,1);
*/
	 /* for(uint8_t i = 0; i < 8; i++)
	   {
		  MAX7219_Send_CMD(i,10);
	   }


    HAL_Delay(1000);*/





	/*  FLASH_EraseInitTypeDef Flash_erase;
	 // HAL_StatusTypeDef Return_erase = NULL;
	  HAL_StatusTypeDef flash_ok = HAL_ERROR;

	  uint32_t Error = 0;
	  uint16_t Write_flash_yst = 0, Write_flash_giz = 0, Write_flash_on = 0;

	  Flash_erase. TypeErase = FLASH_TYPEERASE_PAGES;
	  Flash_erase. Banks = FLASH_BANK_1;
	  Flash_erase. NbPages = 1;

	  flash_ok = HAL_ERROR;
	  while(flash_ok != HAL_OK){
		  flash_ok = HAL_FLASH_Unlock();
	  }

	  if(HAL_FLASHEx_Erase(&Flash_erase, &Error)!=HAL_OK)
	  {
		 // Return_erase=HAL_FLASH_Lock();
	  }

	  flash_ok = HAL_ERROR;
	  while(flash_ok !=HAL_OK){
		  flash_ok = HAL_FLASH_Program(FLASH_TYPEPROGRAM_HALFWORD, 0x0801FC00, Write_flash_yst);
	  }
	  flash_ok = HAL_ERROR;
	  	  while(flash_ok !=HAL_OK){
	  		  flash_ok = HAL_FLASH_Program(FLASH_TYPEPROGRAM_HALFWORD, 0x0801FC02, Write_flash_giz);
	  }
	  flash_ok = HAL_ERROR;
		  while(flash_ok !=HAL_OK){
			  flash_ok = HAL_FLASH_Program(FLASH_TYPEPROGRAM_HALFWORD, 0x0801FC04, Write_flash_on);
	  }
	  flash_ok = HAL_ERROR;
	  while(flash_ok !=HAL_OK){
		  flash_ok = HAL_FLASH_Lock();
	  }


	  ystav_m = (*(__IO uint32_t*)0x0801FC00);
	  giz_m = (*(__IO uint32_t*)0x0801FC02);
	  on_m = (*(__IO uint32_t*)0x0801FC04);
	  if((ystav_m == 0xffff)&&(giz_m == 0xffff)&&(on_m == 0xffff))
	  {
	//	  error_read_flash = 1;
	  }
	  else
	  {
		 // error_read_flash = 0;
	  }*/
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
  RCC_OscInitStruct.PLL.PLLState = RCC_PLL_NONE;
  if (HAL_RCC_OscConfig(&RCC_OscInitStruct) != HAL_OK)
  {
    Error_Handler();
  }

  /** Initializes the CPU, AHB and APB buses clocks
  */
  RCC_ClkInitStruct.ClockType = RCC_CLOCKTYPE_HCLK|RCC_CLOCKTYPE_SYSCLK
                              |RCC_CLOCKTYPE_PCLK1|RCC_CLOCKTYPE_PCLK2;
  RCC_ClkInitStruct.SYSCLKSource = RCC_SYSCLKSOURCE_HSI;
  RCC_ClkInitStruct.AHBCLKDivider = RCC_SYSCLK_DIV1;
  RCC_ClkInitStruct.APB1CLKDivider = RCC_HCLK_DIV1;
  RCC_ClkInitStruct.APB2CLKDivider = RCC_HCLK_DIV1;

  if (HAL_RCC_ClockConfig(&RCC_ClkInitStruct, FLASH_LATENCY_0) != HAL_OK)
  {
    Error_Handler();
  }
}

/**
  * @brief SPI1 Initialization Function
  * @param None
  * @retval None
  */
static void MX_SPI1_Init(void)
{

  /* USER CODE BEGIN SPI1_Init 0 */

  /* USER CODE END SPI1_Init 0 */

  /* USER CODE BEGIN SPI1_Init 1 */

  /* USER CODE END SPI1_Init 1 */
  /* SPI1 parameter configuration*/
  hspi1.Instance = SPI1;
  hspi1.Init.Mode = SPI_MODE_MASTER;
  hspi1.Init.Direction = SPI_DIRECTION_2LINES;
  hspi1.Init.DataSize = SPI_DATASIZE_8BIT;
  hspi1.Init.CLKPolarity = SPI_POLARITY_LOW;
  hspi1.Init.CLKPhase = SPI_PHASE_1EDGE;
  hspi1.Init.NSS = SPI_NSS_SOFT;
  hspi1.Init.BaudRatePrescaler = SPI_BAUDRATEPRESCALER_16;
  hspi1.Init.FirstBit = SPI_FIRSTBIT_MSB;
  hspi1.Init.TIMode = SPI_TIMODE_DISABLE;
  hspi1.Init.CRCCalculation = SPI_CRCCALCULATION_DISABLE;
  hspi1.Init.CRCPolynomial = 10;
  if (HAL_SPI_Init(&hspi1) != HAL_OK)
  {
    Error_Handler();
  }
  /* USER CODE BEGIN SPI1_Init 2 */

  /* USER CODE END SPI1_Init 2 */

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
  HAL_GPIO_WritePin(GPIOC, DIN_Pin|CS_Pin|CLK_Pin, GPIO_PIN_SET);

  /*Configure GPIO pins : DIN_Pin CS_Pin CLK_Pin */
  GPIO_InitStruct.Pin = DIN_Pin|CS_Pin|CLK_Pin;
  GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
  GPIO_InitStruct.Pull = GPIO_PULLUP;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_HIGH;
  HAL_GPIO_Init(GPIOC, &GPIO_InitStruct);

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
