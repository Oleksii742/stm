/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * @file           : main.h
  * @brief          : Header for main.c file.
  *                   This file contains the common defines of the application.
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

/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef __MAIN_H
#define __MAIN_H

#ifdef __cplusplus
extern "C" {
#endif

/* Includes ------------------------------------------------------------------*/
#include "stm32f1xx_hal.h"

/* Private includes ----------------------------------------------------------*/
/* USER CODE BEGIN Includes */

/* USER CODE END Includes */

/* Exported types ------------------------------------------------------------*/
/* USER CODE BEGIN ET */

/* USER CODE END ET */

/* Exported constants --------------------------------------------------------*/
/* USER CODE BEGIN EC */

/* USER CODE END EC */

/* Exported macro ------------------------------------------------------------*/
/* USER CODE BEGIN EM */

/* USER CODE END EM */

/* Exported functions prototypes ---------------------------------------------*/
void Error_Handler(void);

/* USER CODE BEGIN EFP */

/* USER CODE END EFP */

/* Private defines -----------------------------------------------------------*/
#define INPUT_Pin GPIO_PIN_0
#define INPUT_GPIO_Port GPIOA
#define PC_8_Pin GPIO_PIN_8
#define PC_8_GPIO_Port GPIOC
#define PC_9_Pin GPIO_PIN_9
#define PC_9_GPIO_Port GPIOC
#define PA_8_Pin GPIO_PIN_8
#define PA_8_GPIO_Port GPIOA
#define PA_9_Pin GPIO_PIN_9
#define PA_9_GPIO_Port GPIOA
#define PA_10_Pin GPIO_PIN_10
#define PA_10_GPIO_Port GPIOA
#define PA_11_Pin GPIO_PIN_11
#define PA_11_GPIO_Port GPIOA
#define PA_12_Pin GPIO_PIN_12
#define PA_12_GPIO_Port GPIOA
#define PA_15_Pin GPIO_PIN_15
#define PA_15_GPIO_Port GPIOA
/* USER CODE BEGIN Private defines */

/* USER CODE END Private defines */

#ifdef __cplusplus
}
#endif

#endif /* __MAIN_H */
