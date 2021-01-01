/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * @file           : main.h
  * @brief          : Header for main.c file.
  *                   This file contains the common defines of the application.
  ******************************************************************************
  * @attention
  *
  * <h2><center>&copy; Copyright (c) 2020 STMicroelectronics.
  * All rights reserved.</center></h2>
  *
  * This software component is licensed by ST under BSD 3-Clause license,
  * the "License"; You may not use this file except in compliance with the
  * License. You may obtain a copy of the License at:
  *                        opensource.org/licenses/BSD-3-Clause
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
#include "stm32l4xx_hal.h"

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
#define SEG_4_Pin GPIO_PIN_2
#define SEG_4_GPIO_Port GPIOA
#define BTN_4_Pin GPIO_PIN_3
#define BTN_4_GPIO_Port GPIOA
#define BTN_4_EXTI_IRQn EXTI3_IRQn
#define SEG_2_Pin GPIO_PIN_4
#define SEG_2_GPIO_Port GPIOA
#define SEG_7_Pin GPIO_PIN_5
#define SEG_7_GPIO_Port GPIOA
#define SEG_6_Pin GPIO_PIN_6
#define SEG_6_GPIO_Port GPIOA
#define SEG_5_Pin GPIO_PIN_7
#define SEG_5_GPIO_Port GPIOA
#define SEG_0_Pin GPIO_PIN_0
#define SEG_0_GPIO_Port GPIOB
#define BTN_0_Pin GPIO_PIN_1
#define BTN_0_GPIO_Port GPIOB
#define BTN_0_EXTI_IRQn EXTI1_IRQn
#define BTN_3_Pin GPIO_PIN_2
#define BTN_3_GPIO_Port GPIOB
#define BTN_3_EXTI_IRQn EXTI2_IRQn
#define M_SPI_SCK_Pin GPIO_PIN_13
#define M_SPI_SCK_GPIO_Port GPIOE
#define M_SPI_MISO_Pin GPIO_PIN_14
#define M_SPI_MISO_GPIO_Port GPIOE
#define M_SPI_MOSI_Pin GPIO_PIN_15
#define M_SPI_MOSI_GPIO_Port GPIOE
#define LED2_Pin GPIO_PIN_14
#define LED2_GPIO_Port GPIOB
#define BTN_2_Pin GPIO_PIN_14
#define BTN_2_GPIO_Port GPIOD
#define BTN_2_EXTI_IRQn EXTI15_10_IRQn
#define SEG_3_Pin GPIO_PIN_15
#define SEG_3_GPIO_Port GPIOA
#define BTN_5_Pin GPIO_PIN_4
#define BTN_5_GPIO_Port GPIOB
#define BTN_5_EXTI_IRQn EXTI4_IRQn
#define SEG_9_Pin GPIO_PIN_8
#define SEG_9_GPIO_Port GPIOB
#define SEG_8_Pin GPIO_PIN_9
#define SEG_8_GPIO_Port GPIOB
/* USER CODE BEGIN Private defines */

/* USER CODE END Private defines */

#ifdef __cplusplus
}
#endif

#endif /* __MAIN_H */

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
