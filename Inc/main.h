/**
  ******************************************************************************
  * @file           : main.h
  * @brief          : Header for main.c file.
  *                   This file contains the common defines of the application.
  ******************************************************************************
  ** This notice applies to any and all portions of this file
  * that are not between comment pairs USER CODE BEGIN and
  * USER CODE END. Other portions of this file, whether 
  * inserted by the user or by software development tools
  * are owned by their respective copyright owners.
  *
  * COPYRIGHT(c) 2021 STMicroelectronics
  *
  * Redistribution and use in source and binary forms, with or without modification,
  * are permitted provided that the following conditions are met:
  *   1. Redistributions of source code must retain the above copyright notice,
  *      this list of conditions and the following disclaimer.
  *   2. Redistributions in binary form must reproduce the above copyright notice,
  *      this list of conditions and the following disclaimer in the documentation
  *      and/or other materials provided with the distribution.
  *   3. Neither the name of STMicroelectronics nor the names of its contributors
  *      may be used to endorse or promote products derived from this software
  *      without specific prior written permission.
  *
  * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS"
  * AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
  * IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
  * DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE LIABLE
  * FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL
  * DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR
  * SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER
  * CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY,
  * OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE
  * OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
  *
  ******************************************************************************
  */

/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef __MAIN_H__
#define __MAIN_H__

/* Includes ------------------------------------------------------------------*/
#include "stm32f4xx_hal.h"

/* USER CODE BEGIN Includes */

/* USER CODE END Includes */

/* Private define ------------------------------------------------------------*/

#define SENSOR_1_Pin GPIO_PIN_13
#define SENSOR_1_GPIO_Port GPIOC
#define SENSOR_2_Pin GPIO_PIN_14
#define SENSOR_2_GPIO_Port GPIOC
#define DRV_ENABLE_Pin GPIO_PIN_0
#define DRV_ENABLE_GPIO_Port GPIOC
#define DRV_M0_Pin GPIO_PIN_1
#define DRV_M0_GPIO_Port GPIOC
#define DRV_M1_Pin GPIO_PIN_2
#define DRV_M1_GPIO_Port GPIOC
#define DRV_M2_Pin GPIO_PIN_3
#define DRV_M2_GPIO_Port GPIOC
#define WKUP_Pin GPIO_PIN_0
#define WKUP_GPIO_Port GPIOA
#define EXT_0_Pin GPIO_PIN_5
#define EXT_0_GPIO_Port GPIOA
#define INT_0_Pin GPIO_PIN_6
#define INT_0_GPIO_Port GPIOA
#define X8D_Pin GPIO_PIN_7
#define X8D_GPIO_Port GPIOA
#define DRV_RESET_Pin GPIO_PIN_4
#define DRV_RESET_GPIO_Port GPIOC
#define DRV_DIR_Pin GPIO_PIN_5
#define DRV_DIR_GPIO_Port GPIOC
#define BOOT1_Pin GPIO_PIN_2
#define BOOT1_GPIO_Port GPIOB
#define FLASH_CE_Pin GPIO_PIN_12
#define FLASH_CE_GPIO_Port GPIOB
#define FLASH_SCK_Pin GPIO_PIN_13
#define FLASH_SCK_GPIO_Port GPIOB
#define FLASH_SO_Pin GPIO_PIN_14
#define FLASH_SO_GPIO_Port GPIOB
#define FLASH_SI_Pin GPIO_PIN_15
#define FLASH_SI_GPIO_Port GPIOB
#define DRV_STEP_Pin GPIO_PIN_6
#define DRV_STEP_GPIO_Port GPIOC
#define DRV_FAULT_Pin GPIO_PIN_7
#define DRV_FAULT_GPIO_Port GPIOC
#define FLASH_WP_Pin GPIO_PIN_8
#define FLASH_WP_GPIO_Port GPIOC
#define FLASH_HOLD_Pin GPIO_PIN_9
#define FLASH_HOLD_GPIO_Port GPIOC
#define X8C_Pin GPIO_PIN_8
#define X8C_GPIO_Port GPIOA
#define X8B_Pin GPIO_PIN_9
#define X8B_GPIO_Port GPIOA
#define X8A_Pin GPIO_PIN_10
#define X8A_GPIO_Port GPIOA
#define X6C_Pin GPIO_PIN_10
#define X6C_GPIO_Port GPIOC
#define X6B_Pin GPIO_PIN_11
#define X6B_GPIO_Port GPIOC
#define X7C_Pin GPIO_PIN_12
#define X7C_GPIO_Port GPIOC
#define X7B_Pin GPIO_PIN_2
#define X7B_GPIO_Port GPIOD
#define USART2_DE_Pin GPIO_PIN_5
#define USART2_DE_GPIO_Port GPIOB

/* ########################## Assert Selection ############################## */
/**
  * @brief Uncomment the line below to expanse the "assert_param" macro in the 
  *        HAL drivers code
  */
/* #define USE_FULL_ASSERT    1U */

/* USER CODE BEGIN Private defines */

/* USER CODE END Private defines */

#ifdef __cplusplus
 extern "C" {
#endif
void _Error_Handler(char *, int);

#define Error_Handler() _Error_Handler(__FILE__, __LINE__)
#ifdef __cplusplus
}
#endif

#endif /* __MAIN_H__ */

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
