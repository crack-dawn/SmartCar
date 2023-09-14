/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * @file           : main.h
  * @brief          : Header for main.c file.
  *                   This file contains the common defines of the application.
  ******************************************************************************
  * @attention
  *
  * Copyright (c) 2023 STMicroelectronics.
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
#include "stm32f4xx_hal.h"

/* Private includes ----------------------------------------------------------*/
/* USER CODE BEGIN Includes */
//#include "control.h"

/* USER CODE END Includes */

/* Exported types ------------------------------------------------------------*/
/* USER CODE BEGIN ET */
typedef unsigned char  u8;
typedef unsigned short u16;
typedef unsigned int   u32;
typedef unsigned long  u64;

// _MAIN_H_
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
#define Key1_EXIT3_Pin GPIO_PIN_3
#define Key1_EXIT3_GPIO_Port GPIOE
#define Key1_EXIT3_EXTI_IRQn EXTI3_IRQn
#define Key0_EXIT4_Pin GPIO_PIN_4
#define Key0_EXIT4_GPIO_Port GPIOE
#define Key0_EXIT4_EXTI_IRQn EXTI4_IRQn
#define TIM9_CH1_pwm1_Pin GPIO_PIN_5
#define TIM9_CH1_pwm1_GPIO_Port GPIOE
#define TIM9_CH2_pwm2_Pin GPIO_PIN_6
#define TIM9_CH2_pwm2_GPIO_Port GPIOE
#define LED0_PF9_Pin GPIO_PIN_9
#define LED0_PF9_GPIO_Port GPIOF
#define LED1_PF10_Pin GPIO_PIN_10
#define LED1_PF10_GPIO_Port GPIOF
#define DIR1_Pin GPIO_PIN_0
#define DIR1_GPIO_Port GPIOC
#define DIR2_Pin GPIO_PIN_1
#define DIR2_GPIO_Port GPIOC
#define DIR3_Pin GPIO_PIN_2
#define DIR3_GPIO_Port GPIOC
#define DIR4_Pin GPIO_PIN_3
#define DIR4_GPIO_Port GPIOC
#define TIM5_CH1_Step1_Pin GPIO_PIN_0
#define TIM5_CH1_Step1_GPIO_Port GPIOA
#define TIM5_CH2_Step2_Pin GPIO_PIN_1
#define TIM5_CH2_Step2_GPIO_Port GPIOA
#define TIM5_CH3_Step3_Pin GPIO_PIN_2
#define TIM5_CH3_Step3_GPIO_Port GPIOA
#define TIM5_CH4_Step4_Pin GPIO_PIN_3
#define TIM5_CH4_Step4_GPIO_Port GPIOA
#define STEP_EN_Pin GPIO_PIN_4
#define STEP_EN_GPIO_Port GPIOC
#define Limit_1_Pin GPIO_PIN_12
#define Limit_1_GPIO_Port GPIOE
#define Limit_1_EXTI_IRQn EXTI15_10_IRQn
#define Limit_2_Pin GPIO_PIN_13
#define Limit_2_GPIO_Port GPIOE
#define Limit_2_EXTI_IRQn EXTI15_10_IRQn
#define Limit_3_Pin GPIO_PIN_14
#define Limit_3_GPIO_Port GPIOE
#define Limit_4_Pin GPIO_PIN_15
#define Limit_4_GPIO_Port GPIOE
#define Limit_4_EXTI_IRQn EXTI15_10_IRQn
#define Limit_light_1_Pin GPIO_PIN_11
#define Limit_light_1_GPIO_Port GPIOB
#define Limit_light_1_EXTI_IRQn EXTI15_10_IRQn
#define UART3_TX_SCAN_Pin GPIO_PIN_8
#define UART3_TX_SCAN_GPIO_Port GPIOD
#define UART3_RX_SCAN_Pin GPIO_PIN_9
#define UART3_RX_SCAN_GPIO_Port GPIOD
#define EB2_Pin GPIO_PIN_12
#define EB2_GPIO_Port GPIOD
#define EA2_Pin GPIO_PIN_13
#define EA2_GPIO_Port GPIOD
#define PWMA_pin_Pin GPIO_PIN_8
#define PWMA_pin_GPIO_Port GPIOA
#define USART1_TX_DEBUG_Pin GPIO_PIN_9
#define USART1_TX_DEBUG_GPIO_Port GPIOA
#define USART1_RX_DEBUG_Pin GPIO_PIN_10
#define USART1_RX_DEBUG_GPIO_Port GPIOA
#define PWMB_pin_Pin GPIO_PIN_11
#define PWMB_pin_GPIO_Port GPIOA
#define STBY_Pin GPIO_PIN_12
#define STBY_GPIO_Port GPIOA
#define UART4_TX_DISPLAY_Pin GPIO_PIN_10
#define UART4_TX_DISPLAY_GPIO_Port GPIOC
#define UART4_RX_DISPLAY_Pin GPIO_PIN_11
#define UART4_RX_DISPLAY_GPIO_Port GPIOC
#define AN2_Pin GPIO_PIN_0
#define AN2_GPIO_Port GPIOD
#define AN1_Pin GPIO_PIN_1
#define AN1_GPIO_Port GPIOD
#define BN2_Pin GPIO_PIN_3
#define BN2_GPIO_Port GPIOD
#define BN1_Pin GPIO_PIN_4
#define BN1_GPIO_Port GPIOD
#define UART2_TX_CARMERA_Pin GPIO_PIN_5
#define UART2_TX_CARMERA_GPIO_Port GPIOD
#define UART2_RX_CARMERA_Pin GPIO_PIN_6
#define UART2_RX_CARMERA_GPIO_Port GPIOD
#define EB1_Pin GPIO_PIN_4
#define EB1_GPIO_Port GPIOB
#define EA1_Pin GPIO_PIN_5
#define EA1_GPIO_Port GPIOB

/* USER CODE BEGIN Private defines */
extern float duty;
/* USER CODE END Private defines */

#ifdef __cplusplus
}
#endif

#endif /* __MAIN_H */
