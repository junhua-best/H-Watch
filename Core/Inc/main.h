/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * @file           : main.h
  * @brief          : Header for main.c file.
  *                   This file contains the common defines of the application.
  ******************************************************************************
  * @attention
  *
  * <h2><center>&copy; Copyright (c) 2021 STMicroelectronics.
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
#define LCD0_Pin GPIO_PIN_13
#define LCD0_GPIO_Port GPIOC
#define LCD_CS_Pin GPIO_PIN_1
#define LCD_CS_GPIO_Port GPIOA
#define FLASH_CS_Pin GPIO_PIN_4
#define FLASH_CS_GPIO_Port GPIOA
#define BLE_ON_Pin GPIO_PIN_12
#define BLE_ON_GPIO_Port GPIOB
#define SYS_ON_Pin GPIO_PIN_13
#define SYS_ON_GPIO_Port GPIOB
#define V_EH_Pin GPIO_PIN_14
#define V_EH_GPIO_Port GPIOB
#define ST25DV_GIO_Pin GPIO_PIN_15
#define ST25DV_GIO_GPIO_Port GPIOB
#define GET_VBAT_Pin GPIO_PIN_12
#define GET_VBAT_GPIO_Port GPIOA
#define usb_in_sign_Pin GPIO_PIN_3
#define usb_in_sign_GPIO_Port GPIOB
#define MPU_INT_Pin GPIO_PIN_5
#define MPU_INT_GPIO_Port GPIOB
#define LCD_DC_Pin GPIO_PIN_8
#define LCD_DC_GPIO_Port GPIOB
#define LCD_RES_Pin GPIO_PIN_9
#define LCD_RES_GPIO_Port GPIOB
/* USER CODE BEGIN Private defines */

/* USER CODE END Private defines */

#ifdef __cplusplus
}
#endif

#endif /* __MAIN_H */

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
