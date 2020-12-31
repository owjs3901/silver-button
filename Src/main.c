/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * @file           : main.c
  * @brief          : Main program body
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
/* Includes ------------------------------------------------------------------*/
#include "main.h"

/* Private includes ----------------------------------------------------------*/
/* USER CODE BEGIN Includes */

#include <string.h>
#include <stdio.h>

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
UART_HandleTypeDef huart1;

/* USER CODE BEGIN PV */

/* USER CODE END PV */

/* Private function prototypes -----------------------------------------------*/
void SystemClock_Config(void);

static void MX_GPIO_Init(void);

static void MX_USART1_UART_Init(void);
/* USER CODE BEGIN PFP */

/* USER CODE END PFP */

/* Private user code ---------------------------------------------------------*/
/* USER CODE BEGIN 0 */
/**
 * 1. 온도
 * 2. 습도
 * 3. 문
 * 4. 창문
 * 5. 방
 * 6. 거실
 */

GPIO_TypeDef *seg_type_def[10] = {
        SEG_0_GPIO_Port,
        SEG_1_GPIO_Port,
        SEG_2_GPIO_Port,
        SEG_3_GPIO_Port,
        SEG_4_GPIO_Port,
        SEG_5_GPIO_Port,
        SEG_6_GPIO_Port,
        SEG_7_GPIO_Port,
        SEG_8_GPIO_Port,
        SEG_9_GPIO_Port,
};
uint16_t seg_pin[10] = {
        SEG_0_Pin,
        SEG_1_Pin,
        SEG_2_Pin,
        SEG_3_Pin,
        SEG_4_Pin,
        SEG_5_Pin,
        SEG_6_Pin,
        SEG_7_Pin,
        SEG_8_Pin,
        SEG_9_Pin,
};

/**
 * seg 숫자
 */
int seg_array[10][10] = {
        {
                0, 0, 0, 0, 0, 0, 1, 0, 0, 0
        },
        {
                1, 1, 1, 0, 0, 0, 1, 1, 1, 0
        },
        {
                0, 0, 0, 1, 0, 0, 0, 1, 0, 0
        },
        {
                0, 0, 1, 0, 0, 0, 0, 1, 0, 0
        },
        {
                1, 0, 1, 0, 0, 0, 0, 0, 1, 0 // 4
        },
        {
                0, 0, 1, 0, 0, 0, 0, 0, 0, 1
        },
        {
                0, 0, 0, 0, 0, 0, 0, 0, 1, 1
        },
        {
                1, 1, 1, 0, 0, 1, 1, 0, 0, 0
        },
        {
                0, 0, 0, 0, 0, 0, 0, 0, 0, 0
        },
        {
                1, 1, 1, 0, 0, 1, 0, 0, 0, 0
        },
};

// 0~99 까지 입력 가능
void setSeg(uint8_t num) {

    // 10의 자리
    HAL_GPIO_WritePin(seg_type_def[5], seg_pin[5], GPIO_PIN_SET);
    HAL_GPIO_WritePin(seg_type_def[4], seg_pin[4], GPIO_PIN_RESET);

    for (int i = 0; i < 10; i++)
        if (i != 4 && i != 5)
            HAL_GPIO_WritePin(seg_type_def[i], seg_pin[i], seg_array[num / 10][i] ? GPIO_PIN_SET : GPIO_PIN_RESET);
    HAL_Delay(4);

    // 1의 자리
    HAL_GPIO_WritePin(seg_type_def[4], seg_pin[4], GPIO_PIN_SET);
    HAL_GPIO_WritePin(seg_type_def[5], seg_pin[5], GPIO_PIN_RESET);

    for (int i = 0; i < 10; i++)
        if (i != 4 && i != 5)
            HAL_GPIO_WritePin(seg_type_def[i], seg_pin[i], seg_array[num % 10][i] ? GPIO_PIN_SET : GPIO_PIN_RESET);
    HAL_Delay(4);
}

int g_temp; // 온도
int g_hum; // 습도

int g_seg_type = 0; // 온도를 보여줄건지 습도를 보여줄 건지
int g_mode = 0; // 글로벌 모드

int g_lastSec = 0;

/* USER CODE END 0 */

/**
  * @brief  The application entry point.
  * @retval int
  */
int main(void) {
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
    MX_USART1_UART_Init();
    /* USER CODE BEGIN 2 */

    /* USER CODE END 2 */

    /* Infinite loop */
    /* USER CODE BEGIN WHILE */
    while (1) {
        /* USER CODE END WHILE */

        /* USER CODE BEGIN 3 */

//        setSeg(g_seg_type == 0 ? g_temp : g_hum);
//        time_t se;

        int nowSec = HAL_GetTick();

        setSeg(nowSec - g_lastSec < 3000 ? g_mode : g_seg_type == 0 ? g_temp : g_hum);
    }
    /* USER CODE END 3 */
}

/**
  * @brief System Clock Configuration
  * @retval None
  */
void SystemClock_Config(void) {
    RCC_OscInitTypeDef RCC_OscInitStruct = {0};
    RCC_ClkInitTypeDef RCC_ClkInitStruct = {0};
    RCC_PeriphCLKInitTypeDef PeriphClkInit = {0};

    /** Configure the main internal regulator output voltage
    */
    if (HAL_PWREx_ControlVoltageScaling(PWR_REGULATOR_VOLTAGE_SCALE1) != HAL_OK) {
        Error_Handler();
    }
    /** Initializes the RCC Oscillators according to the specified parameters
    * in the RCC_OscInitTypeDef structure.
    */
    RCC_OscInitStruct.OscillatorType = RCC_OSCILLATORTYPE_MSI;
    RCC_OscInitStruct.MSIState = RCC_MSI_ON;
    RCC_OscInitStruct.MSICalibrationValue = 0;
    RCC_OscInitStruct.MSIClockRange = RCC_MSIRANGE_6;
    RCC_OscInitStruct.PLL.PLLState = RCC_PLL_NONE;
    if (HAL_RCC_OscConfig(&RCC_OscInitStruct) != HAL_OK) {
        Error_Handler();
    }
    /** Initializes the CPU, AHB and APB buses clocks
    */
    RCC_ClkInitStruct.ClockType = RCC_CLOCKTYPE_HCLK | RCC_CLOCKTYPE_SYSCLK
                                  | RCC_CLOCKTYPE_PCLK1 | RCC_CLOCKTYPE_PCLK2;
    RCC_ClkInitStruct.SYSCLKSource = RCC_SYSCLKSOURCE_MSI;
    RCC_ClkInitStruct.AHBCLKDivider = RCC_SYSCLK_DIV1;
    RCC_ClkInitStruct.APB1CLKDivider = RCC_HCLK_DIV1;
    RCC_ClkInitStruct.APB2CLKDivider = RCC_HCLK_DIV1;

    if (HAL_RCC_ClockConfig(&RCC_ClkInitStruct, FLASH_LATENCY_0) != HAL_OK) {
        Error_Handler();
    }
    PeriphClkInit.PeriphClockSelection = RCC_PERIPHCLK_USART1;
    PeriphClkInit.Usart1ClockSelection = RCC_USART1CLKSOURCE_PCLK2;
    if (HAL_RCCEx_PeriphCLKConfig(&PeriphClkInit) != HAL_OK) {
        Error_Handler();
    }
}

/**
  * @brief USART1 Initialization Function
  * @param None
  * @retval None
  */
static void MX_USART1_UART_Init(void) {

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
    huart1.Init.OneBitSampling = UART_ONE_BIT_SAMPLE_DISABLE;
    huart1.Init.ClockPrescaler = UART_PRESCALER_DIV1;
    huart1.AdvancedInit.AdvFeatureInit = UART_ADVFEATURE_NO_INIT;
    if (HAL_UART_Init(&huart1) != HAL_OK) {
        Error_Handler();
    }
    if (HAL_UARTEx_SetTxFifoThreshold(&huart1, UART_TXFIFO_THRESHOLD_1_8) != HAL_OK) {
        Error_Handler();
    }
    if (HAL_UARTEx_SetRxFifoThreshold(&huart1, UART_RXFIFO_THRESHOLD_1_8) != HAL_OK) {
        Error_Handler();
    }
    if (HAL_UARTEx_DisableFifoMode(&huart1) != HAL_OK) {
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
static void MX_GPIO_Init(void) {
    GPIO_InitTypeDef GPIO_InitStruct = {0};

    /* GPIO Ports Clock Enable */
    __HAL_RCC_GPIOA_CLK_ENABLE();
    __HAL_RCC_GPIOB_CLK_ENABLE();
    __HAL_RCC_GPIOD_CLK_ENABLE();

    /*Configure GPIO pin Output Level */
    HAL_GPIO_WritePin(GPIOA, SEG_4_Pin | SEG_2_Pin | SEG_7_Pin | SEG_6_Pin
                             | SEG_5_Pin | SEG_3_Pin, GPIO_PIN_RESET);

    /*Configure GPIO pin Output Level */
    HAL_GPIO_WritePin(GPIOB, SEG_0_Pin | SEG_1_Pin | LED2_Pin | SEG_9_Pin
                             | SEG_8_Pin, GPIO_PIN_RESET);

    /*Configure GPIO pins : BTN_1_Pin BTN_0_Pin BTN_4_Pin */
    GPIO_InitStruct.Pin = BTN_1_Pin | BTN_0_Pin | BTN_4_Pin;
    GPIO_InitStruct.Mode = GPIO_MODE_IT_RISING;
    GPIO_InitStruct.Pull = GPIO_PULLUP;
    HAL_GPIO_Init(GPIOA, &GPIO_InitStruct);

    /*Configure GPIO pins : SEG_4_Pin SEG_2_Pin SEG_7_Pin SEG_6_Pin
                             SEG_5_Pin SEG_3_Pin */
    GPIO_InitStruct.Pin = SEG_4_Pin | SEG_2_Pin | SEG_7_Pin | SEG_6_Pin
                          | SEG_5_Pin | SEG_3_Pin;
    GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
    GPIO_InitStruct.Pull = GPIO_NOPULL;
    GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
    HAL_GPIO_Init(GPIOA, &GPIO_InitStruct);

    /*Configure GPIO pins : SEG_0_Pin SEG_1_Pin LED2_Pin SEG_9_Pin
                             SEG_8_Pin */
    GPIO_InitStruct.Pin = SEG_0_Pin | SEG_1_Pin | LED2_Pin | SEG_9_Pin
                          | SEG_8_Pin;
    GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
    GPIO_InitStruct.Pull = GPIO_NOPULL;
    GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
    HAL_GPIO_Init(GPIOB, &GPIO_InitStruct);

    /*Configure GPIO pins : BTN_3_Pin BTN_5_Pin */
    GPIO_InitStruct.Pin = BTN_3_Pin | BTN_5_Pin;
    GPIO_InitStruct.Mode = GPIO_MODE_IT_RISING;
    GPIO_InitStruct.Pull = GPIO_PULLUP;
    HAL_GPIO_Init(GPIOB, &GPIO_InitStruct);

    /*Configure GPIO pin : BTN_2_Pin */
    GPIO_InitStruct.Pin = BTN_2_Pin;
    GPIO_InitStruct.Mode = GPIO_MODE_IT_RISING;
    GPIO_InitStruct.Pull = GPIO_PULLUP;
    HAL_GPIO_Init(BTN_2_GPIO_Port, &GPIO_InitStruct);

    /* EXTI interrupt init*/
    HAL_NVIC_SetPriority(EXTI0_IRQn, 0, 0);
    HAL_NVIC_EnableIRQ(EXTI0_IRQn);

    HAL_NVIC_SetPriority(EXTI1_IRQn, 0, 0);
    HAL_NVIC_EnableIRQ(EXTI1_IRQn);

    HAL_NVIC_SetPriority(EXTI2_IRQn, 0, 0);
    HAL_NVIC_EnableIRQ(EXTI2_IRQn);

    HAL_NVIC_SetPriority(EXTI3_IRQn, 0, 0);
    HAL_NVIC_EnableIRQ(EXTI3_IRQn);

    HAL_NVIC_SetPriority(EXTI4_IRQn, 0, 0);
    HAL_NVIC_EnableIRQ(EXTI4_IRQn);

    HAL_NVIC_SetPriority(EXTI15_10_IRQn, 0, 0);
    HAL_NVIC_EnableIRQ(EXTI15_10_IRQn);

}

/* USER CODE BEGIN 4 */

void HAL_GPIO_EXTI_Callback(uint16_t GPIO_Pin) {
    // 모드 변경 날짜 갱신
    g_lastSec = HAL_GetTick();

    switch (GPIO_Pin) {
        case BTN_0_Pin:
            HAL_UART_Transmit(&huart1, "BTN0", strlen("BTN0"), 10);
            g_mode = 0;
            break;
        case BTN_1_Pin:
            HAL_UART_Transmit(&huart1, "BTN1", strlen("BTN1"), 10);
            g_mode = 1;
            break;
        case BTN_2_Pin:
            HAL_UART_Transmit(&huart1, "BTN2", strlen("BTN2"), 10);
            g_mode = 2;
            break;
        case BTN_3_Pin:
            HAL_UART_Transmit(&huart1, "BTN3", strlen("BTN3"), 10);
            g_mode = 3;
            break;
        case BTN_4_Pin:
            HAL_UART_Transmit(&huart1, "BTN4", strlen("BTN4"), 10);
            g_mode = 4;
            break;
        case BTN_5_Pin:
            HAL_UART_Transmit(&huart1, "BTN5", strlen("BTN5"), 10);
            g_mode = 5;
            break;
    }
}

/* USER CODE END 4 */

/**
  * @brief  This function is executed in case of error occurrence.
  * @retval None
  */
void Error_Handler(void) {
    /* USER CODE BEGIN Error_Handler_Debug */
    /* User can add his own implementation to report the HAL error return state */
    __disable_irq();
    while (1) {
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
