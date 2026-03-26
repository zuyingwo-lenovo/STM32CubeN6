/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * @file    I2C/I2C_TwoBoards_ComPolling/Inc/main.h
  * @author  MCD Application Team
  * @brief   Header for main.c module
  ******************************************************************************
  * @attention
  *
  * Copyright (c) 2024 STMicroelectronics.
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

#if defined ( __ICCARM__ )
#  define CMSE_NS_CALL  __cmse_nonsecure_call
#  define CMSE_NS_ENTRY __cmse_nonsecure_entry
#else
#  define CMSE_NS_CALL  __attribute((cmse_nonsecure_call))
#  define CMSE_NS_ENTRY __attribute((cmse_nonsecure_entry))
#endif

/* Includes ------------------------------------------------------------------*/
#include "stm32n6xx_hal.h"

/* Private includes ----------------------------------------------------------*/
/* USER CODE BEGIN Includes */
#include "stm32n6xx_nucleo.h"
/* USER CODE END Includes */

/* Exported types ------------------------------------------------------------*/
/* Function pointer declaration in non-secure*/
#if defined ( __ICCARM__ )
typedef void (CMSE_NS_CALL *funcptr)(void);
#else
typedef void CMSE_NS_CALL (*funcptr)(void);
#endif

/* typedef for non-secure callback functions */
typedef funcptr funcptr_NS;

/* USER CODE BEGIN ET */

/* USER CODE END ET */

/* Exported constants --------------------------------------------------------*/
/* USER CODE BEGIN EC */

/* USER CODE END EC */

/* Exported macro ------------------------------------------------------------*/
/* USER CODE BEGIN EM */
#define COUNTOF(__BUFFER__)   (sizeof(__BUFFER__) / sizeof(*(__BUFFER__)))
/* USER CODE END EM */

/* Exported functions prototypes ---------------------------------------------*/
void Error_Handler(void);

/* USER CODE BEGIN EFP */
HAL_StatusTypeDef PAF9615_CheckPartID(void);
/* USER CODE END EFP */

/* Private defines -----------------------------------------------------------*/
#define I2C_ADDRESS 0x30F
/* PAF9615C2 Sensor Address (7-bit 0x34 shifted for HAL) */
#define SENSOR_ADDRESS_1 (0x34 << 1)
#define SENSOR_ADDRESS_2 (0x57 << 1)

/* PAF9615C2 Bank0 Registers */
#define REG_PART_ID_L 0x00
#define REG_PART_ID_H 0x01
#define REG_STATUS 0x05
#define REG_CMD_BANK_SEL 0x7F
#define REG_SW_RESET 0x7D

/* Expected Values */
#define VAL_PART_ID 0x0271
#define VAL_BANK0 0x00
#define VAL_COLD_RESET 0x5A

/* Status Bits */
#define STATUS_OTP_LOAD_DONE (1 << 6)

/* USER CODE BEGIN Private defines */
#define THRMPL1_PD_Pin GPIO_PIN_9
#define THRMPL1_PD_GPIO_Port GPIOG

#define THRMPL1_ALERT_Pin GPIO_PIN_13
#define THRMPL1_ALERT_GPIO_Port GPIOG


#define THRMPL2_PD_Pin GPIO_PIN_12
#define THRMPL2_PD_GPIO_Port GPIOG

#define THRMPL2_ALERT_Pin GPIO_PIN_14
#define THRMPL2_ALERT_GPIO_Port GPIOG

/* Size of Transmission buffer */
#define TXBUFFERSIZE                      (COUNTOF(aTxBuffer) - 1)
/* Size of Reception buffer */
#define RXBUFFERSIZE                      TXBUFFERSIZE
/* USER CODE END Private defines */

#ifdef __cplusplus
}
#endif

#endif /* __MAIN_H */
