/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * @file           : main.c
  * @brief          : Main program body
  ******************************************************************************
  * @attention
  *
  * Copyright (c) 2026 STMicroelectronics.
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
//#define I2C_ADDRESS_ACC1 (0x18 << 1)
#define I2C_ADDRESS_ACC1 (0x34 << 1)
#define I2C_ADDRESS_TOF  (0x52 << 1)
/* PAF9615C2 Sensor Address (7-bit 0x34 shifted for HAL) */
//#define SENSOR_ADDRESS_1 (0x34 << 1)
//#define SENSOR_ADDRESS_2 (0x57 << 1)
#define SENSOR_ADDRESS_1 (0x34 << 0)
#define SENSOR_ADDRESS_2 (0x57 << 0)
//#define SENSOR_ADDRESS_1 (0x2B << 0)

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


#define TOF_RESET_Pin GPIO_PIN_5
#define TOF_RESET_Port GPIOP
#define TOF_INIT_Pin GPIO_PIN_6
#define TOF_INIT_Port GPIOP
/* USER CODE END Includes */

/* Private typedef -----------------------------------------------------------*/
/* USER CODE BEGIN PTD */

/* USER CODE END PTD */

/* Private define ------------------------------------------------------------*/
/* USER CODE BEGIN PD */
#define I3C_IDX_FRAME_1         0U  /* Index of Frame 1 */
#define I3C_IDX_FRAME_2         1U  /* Index of Frame 2 */
/* USER CODE END PD */

/* Private macro -------------------------------------------------------------*/
/* USER CODE BEGIN PM */

/* USER CODE END PM */

/* Private variables ---------------------------------------------------------*/

I2C_HandleTypeDef hi2c1;

/* USER CODE BEGIN PV */
/* Buffer used for transmission */

uint8_t aTxBuffer[] = {0x7F};

#define COUNTOF(__BUFFER__)   (sizeof(__BUFFER__) / sizeof(*(__BUFFER__)))

#define TXBUFFERSIZE                      (COUNTOF(aTxBuffer) - 1)
#define RXBUFFERSIZE                      TXBUFFERSIZE
/* Buffer used for reception */
uint8_t aRxBuffer[RXBUFFERSIZE];

/* USER CODE END PV */

/* Private function prototypes -----------------------------------------------*/
void SystemClock_Config(void);
static void MX_GPIO_Init(void);
static void MX_I2C1_Init(void);
/* USER CODE BEGIN PFP */
static uint16_t Buffercmp(uint8_t *pBuffer1, uint8_t *pBuffer2, uint16_t BufferLength);
static void TestI2C1();
//static void TestI2C3();
//static void TestI3C1();

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

  /* Enable the CPU Cache */

  /* Enable I-Cache---------------------------------------------------------*/
  SCB_EnableICache();

  /* MCU Configuration--------------------------------------------------------*/
  HAL_Init();

  /* USER CODE BEGIN Init */

  /* USER CODE END Init */

  /* Configure the system clock */
  SystemClock_Config();

  /* USER CODE BEGIN SysInit */

  /* USER CODE END SysInit */

  /* Initialize all configured peripherals */
  MX_GPIO_Init();
  MX_I2C1_Init();
  /* USER CODE BEGIN 2 */

  /* USER CODE END 2 */

  /* Infinite loop */
  /* USER CODE BEGIN WHILE */
  while (1)
  {
    /* USER CODE END WHILE */
	  int status;
	  do{
		  status=0;
			// Test I2C1
		  	TestI2C1();

//		  	TestI2C3();

//		  	TestI3C1();
	  }while(status != HAL_OK);

    /* USER CODE BEGIN 3 */
  }
  return 0;
  /* USER CODE END 3 */
}
/* USER CODE BEGIN CLK 1 */
/* USER CODE END CLK 1 */

/**
  * @brief System Clock Configuration
  * @retval None
  */
void SystemClock_Config(void)
{
  RCC_OscInitTypeDef RCC_OscInitStruct = {0};
  RCC_ClkInitTypeDef RCC_ClkInitStruct = {0};

  /** Configure the System Power Supply
  */
  if (HAL_PWREx_ConfigSupply(PWR_SMPS_SUPPLY) != HAL_OK)
  {
    Error_Handler();
  }

  /** Configure the main internal regulator output voltage
  */
  if (HAL_PWREx_ControlVoltageScaling(PWR_REGULATOR_VOLTAGE_SCALE1) != HAL_OK)
  {
    Error_Handler();
  }

  /* Enable HSI */
  RCC_OscInitStruct.OscillatorType = RCC_OSCILLATORTYPE_HSI;
  RCC_OscInitStruct.HSIState = RCC_HSI_ON;
  RCC_OscInitStruct.HSIDiv = RCC_HSI_DIV1;
  RCC_OscInitStruct.HSICalibrationValue = RCC_HSICALIBRATION_DEFAULT;
  RCC_OscInitStruct.PLL1.PLLState = RCC_PLL_NONE;
  RCC_OscInitStruct.PLL2.PLLState = RCC_PLL_NONE;
  RCC_OscInitStruct.PLL3.PLLState = RCC_PLL_NONE;
  RCC_OscInitStruct.PLL4.PLLState = RCC_PLL_NONE;
  if (HAL_RCC_OscConfig(&RCC_OscInitStruct) != HAL_OK)
  {
    Error_Handler();
  }

  /** Get current CPU/System buses clocks configuration and if necessary switch
 to intermediate HSI clock to ensure target clock can be set
  */
  HAL_RCC_GetClockConfig(&RCC_ClkInitStruct);
  if ((RCC_ClkInitStruct.CPUCLKSource == RCC_CPUCLKSOURCE_IC1) ||
     (RCC_ClkInitStruct.SYSCLKSource == RCC_SYSCLKSOURCE_IC2_IC6_IC11))
  {
    RCC_ClkInitStruct.ClockType = (RCC_CLOCKTYPE_CPUCLK | RCC_CLOCKTYPE_SYSCLK);
    RCC_ClkInitStruct.CPUCLKSource = RCC_CPUCLKSOURCE_HSI;
    RCC_ClkInitStruct.SYSCLKSource = RCC_SYSCLKSOURCE_HSI;
    if (HAL_RCC_ClockConfig(&RCC_ClkInitStruct) != HAL_OK)
    {
      /* Initialization Error */
      Error_Handler();
    }
  }

  /** Initializes the RCC Oscillators according to the specified parameters
  * in the RCC_OscInitTypeDef structure.
  */
  RCC_OscInitStruct.OscillatorType = RCC_OSCILLATORTYPE_NONE;
  RCC_OscInitStruct.PLL1.PLLState = RCC_PLL_ON;
  RCC_OscInitStruct.PLL1.PLLSource = RCC_PLLSOURCE_HSI;
  RCC_OscInitStruct.PLL1.PLLM = 4;
  RCC_OscInitStruct.PLL1.PLLN = 75;
  RCC_OscInitStruct.PLL1.PLLFractional = 0;
  RCC_OscInitStruct.PLL1.PLLP1 = 1;
  RCC_OscInitStruct.PLL1.PLLP2 = 1;
  RCC_OscInitStruct.PLL2.PLLState = RCC_PLL_NONE;
  RCC_OscInitStruct.PLL3.PLLState = RCC_PLL_NONE;
  RCC_OscInitStruct.PLL4.PLLState = RCC_PLL_NONE;
  if (HAL_RCC_OscConfig(&RCC_OscInitStruct) != HAL_OK)
  {
    Error_Handler();
  }

  /** Initializes the CPU, AHB and APB buses clocks
  */
  RCC_ClkInitStruct.ClockType = RCC_CLOCKTYPE_CPUCLK|RCC_CLOCKTYPE_HCLK
                              |RCC_CLOCKTYPE_SYSCLK|RCC_CLOCKTYPE_PCLK1
                              |RCC_CLOCKTYPE_PCLK2|RCC_CLOCKTYPE_PCLK5
                              |RCC_CLOCKTYPE_PCLK4;
  RCC_ClkInitStruct.CPUCLKSource = RCC_CPUCLKSOURCE_IC1;
  RCC_ClkInitStruct.SYSCLKSource = RCC_SYSCLKSOURCE_IC2_IC6_IC11;
  RCC_ClkInitStruct.AHBCLKDivider = RCC_HCLK_DIV2;
  RCC_ClkInitStruct.APB1CLKDivider = RCC_APB1_DIV1;
  RCC_ClkInitStruct.APB2CLKDivider = RCC_APB2_DIV1;
  RCC_ClkInitStruct.APB4CLKDivider = RCC_APB4_DIV1;
  RCC_ClkInitStruct.APB5CLKDivider = RCC_APB5_DIV1;
  RCC_ClkInitStruct.IC1Selection.ClockSelection = RCC_ICCLKSOURCE_PLL1;
  RCC_ClkInitStruct.IC1Selection.ClockDivider = 3;
  RCC_ClkInitStruct.IC2Selection.ClockSelection = RCC_ICCLKSOURCE_PLL1;
  RCC_ClkInitStruct.IC2Selection.ClockDivider = 3;
  RCC_ClkInitStruct.IC6Selection.ClockSelection = RCC_ICCLKSOURCE_PLL1;
  RCC_ClkInitStruct.IC6Selection.ClockDivider = 4;
  RCC_ClkInitStruct.IC11Selection.ClockSelection = RCC_ICCLKSOURCE_PLL1;
  RCC_ClkInitStruct.IC11Selection.ClockDivider = 3;

  if (HAL_RCC_ClockConfig(&RCC_ClkInitStruct) != HAL_OK)
  {
    Error_Handler();
  }
}

/**
  * @brief I2C1 Initialization Function
  * @param None
  * @retval None
  */
static void MX_I2C1_Init(void)
{

  /* USER CODE BEGIN I2C1_Init 0 */

  /* USER CODE END I2C1_Init 0 */

  /* USER CODE BEGIN I2C1_Init 1 */

  /* USER CODE END I2C1_Init 1 */
  hi2c1.Instance = I2C1;
  hi2c1.Init.Timing = 0x60300F32;
  hi2c1.Init.OwnAddress1 = 0;
  hi2c1.Init.AddressingMode = I2C_ADDRESSINGMODE_7BIT;
  hi2c1.Init.DualAddressMode = I2C_DUALADDRESS_DISABLE;
  hi2c1.Init.OwnAddress2 = 0;
  hi2c1.Init.OwnAddress2Masks = I2C_OA2_NOMASK;
  hi2c1.Init.GeneralCallMode = I2C_GENERALCALL_DISABLE;
  hi2c1.Init.NoStretchMode = I2C_NOSTRETCH_DISABLE;
  if (HAL_I2C_Init(&hi2c1) != HAL_OK)
  {
    Error_Handler();
  }

  /** Configure Analogue filter
  */
  if (HAL_I2CEx_ConfigAnalogFilter(&hi2c1, I2C_ANALOGFILTER_ENABLE) != HAL_OK)
  {
    Error_Handler();
  }

  /** Configure Digital filter
  */
  if (HAL_I2CEx_ConfigDigitalFilter(&hi2c1, 0) != HAL_OK)
  {
    Error_Handler();
  }
  /* USER CODE BEGIN I2C1_Init 2 */

  /* USER CODE END I2C1_Init 2 */

}

/**
  * @brief GPIO Initialization Function
  * @param None
  * @retval None
  */
static void MX_GPIO_Init(void)
{
  /* USER CODE BEGIN MX_GPIO_Init_1 */
  GPIO_InitTypeDef THRMPL1_PD_GPIO_InitStruct = {0};

  GPIO_InitTypeDef THRMPL1_ALERT_GPIO_InitStruct = {0};

  GPIO_InitTypeDef THRMPL2_PD_GPIO_InitStruct = {0};

  GPIO_InitTypeDef THRMPL2_ALERT_GPIO_InitStruct = {0};

  GPIO_InitTypeDef TOF_RESET_GPIO_InitStruct = {0};

  GPIO_InitTypeDef TOF_INIT_GPIO_InitStruct = {0};

  /* USER CODE END MX_GPIO_Init_1 */

  /* GPIO Ports Clock Enable */
  __HAL_RCC_GPIOE_CLK_ENABLE();

  /* USER CODE BEGIN MX_GPIO_Init_2 */

  HAL_GPIO_WritePin(THRMPL1_PD_GPIO_Port, THRMPL1_PD_Pin, GPIO_PIN_SET);

  /*Configure GPIO pin : THRMPL1_PD */
  THRMPL1_PD_GPIO_InitStruct.Pin = THRMPL1_PD_Pin;
  THRMPL1_PD_GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
  THRMPL1_PD_GPIO_InitStruct.Pull = GPIO_PULLDOWN;
  THRMPL1_PD_GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_VERY_HIGH;
  HAL_GPIO_Init(THRMPL1_PD_GPIO_Port, &THRMPL1_PD_GPIO_InitStruct);

  HAL_GPIO_WritePin(THRMPL1_ALERT_GPIO_Port, THRMPL1_ALERT_Pin, GPIO_PIN_SET);

  /*Configure GPIO pin : THRMPL1_ALERT */
  THRMPL1_ALERT_GPIO_InitStruct.Pin = THRMPL1_ALERT_Pin;
  THRMPL1_ALERT_GPIO_InitStruct.Mode = GPIO_MODE_INPUT;
  THRMPL1_ALERT_GPIO_InitStruct.Pull = GPIO_PULLUP;
  THRMPL1_ALERT_GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_VERY_HIGH;
  HAL_GPIO_Init(THRMPL1_ALERT_GPIO_Port, &THRMPL1_ALERT_GPIO_InitStruct);


  HAL_GPIO_WritePin(THRMPL2_PD_GPIO_Port, THRMPL2_PD_Pin, GPIO_PIN_SET);

  /*Configure GPIO pin : THRMPL2_PD */
  THRMPL2_PD_GPIO_InitStruct.Pin = THRMPL2_PD_Pin;
  THRMPL2_PD_GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
  THRMPL2_PD_GPIO_InitStruct.Pull = GPIO_PULLDOWN;
  THRMPL2_PD_GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_VERY_HIGH;
  HAL_GPIO_Init(THRMPL2_PD_GPIO_Port, &THRMPL2_PD_GPIO_InitStruct);


  HAL_GPIO_WritePin(THRMPL2_ALERT_GPIO_Port, THRMPL2_ALERT_Pin, GPIO_PIN_SET);

  /*Configure GPIO pin : THRMPL2_ALERT */
  THRMPL2_ALERT_GPIO_InitStruct.Pin = THRMPL2_ALERT_Pin;
  THRMPL2_ALERT_GPIO_InitStruct.Mode = GPIO_MODE_INPUT;
  THRMPL2_ALERT_GPIO_InitStruct.Pull = GPIO_PULLUP;
  THRMPL2_ALERT_GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_VERY_HIGH;
  HAL_GPIO_Init(THRMPL2_ALERT_GPIO_Port, &THRMPL2_ALERT_GPIO_InitStruct);


  HAL_GPIO_WritePin(TOF_RESET_Port, TOF_RESET_Pin, GPIO_PIN_SET);
  /*Configure GPIO pin : TOF_RESET */
  TOF_RESET_GPIO_InitStruct.Pin = TOF_RESET_Pin;
  TOF_RESET_GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
  TOF_RESET_GPIO_InitStruct.Pull = GPIO_PULLDOWN;
  TOF_RESET_GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_HIGH;
  HAL_GPIO_Init(TOF_RESET_Port, &TOF_RESET_GPIO_InitStruct);

  HAL_GPIO_WritePin(TOF_INIT_Port, TOF_INIT_Pin, GPIO_PIN_SET);
  /*Configure GPIO pin : TOF_INIT */
  TOF_INIT_GPIO_InitStruct.Pin = TOF_INIT_Pin;
  TOF_INIT_GPIO_InitStruct.Mode = GPIO_MODE_INPUT;
  TOF_INIT_GPIO_InitStruct.Pull = GPIO_PULLUP;
  TOF_INIT_GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_HIGH;
  HAL_GPIO_Init(TOF_RESET_Port, &TOF_INIT_GPIO_InitStruct);

  /* USER CODE END MX_GPIO_Init_2 */
}

/* USER CODE BEGIN 4 */
///**
//  * @brief  Controller Transmit Complete callback.
//  * @param  hi3c : [IN] Pointer to an I3C_HandleTypeDef structure that contains the configuration information
//  *                     for the specified I3C.
//  * @retval None
//  */
//void HAL_I3C_CtrlTxCpltCallback(I3C_HandleTypeDef *hi3c)
//{
//  /* Toggle LD1: Transfer in transmission process is correct */
////  BSP_LED_Toggle(LD1);
//}
//
///**
//  * @brief  Controller Reception Complete callback.
//  * @param  hi3c : [IN] Pointer to an I3C_HandleTypeDef structure that contains the configuration information
//  *                     for the specified I3C.
//  * @retval None
//  */
//void HAL_I3C_CtrlRxCpltCallback(I3C_HandleTypeDef *hi3c)
//{
//  /* Toggle LD1: Transfer in Reception process is correct */
////  BSP_LED_Toggle(LD1);
//}
//
///**
//  * @brief  Error callback.
//  * @param  hi3c : [IN] Pointer to an I3C_HandleTypeDef structure that contains the configuration information
//  *                     for the specified I3C.
//  * @retval None
//  */
//void HAL_I3C_ErrorCallback(I3C_HandleTypeDef *hi3c)
//{
//  /* Error_Handler() function is called when error occurs. */
//  Error_Handler();
//}

/**
  * @brief  Compares two buffers.
  * @param  pBuffer1, pBuffer2: buffers to be compared.
  * @param  BufferLength: buffer's length
  * @retval 0  : pBuffer1 identical to pBuffer2
  *         >0 : pBuffer1 differs from pBuffer2
  */
static uint16_t Buffercmp(uint8_t *pBuffer1, uint8_t *pBuffer2, uint16_t BufferLength)
{
  while (BufferLength--)
  {
    if ((*pBuffer1) != *pBuffer2)
    {
      return BufferLength;
    }
    pBuffer1++;
    pBuffer2++;
  }

  return 0;
}

void TestI2C1()
{
	int status;
	do{

	    uint8_t Addr_0x7f = 0x00;
		status = HAL_I2C_Mem_Write(
				&hi2c1, (uint16_t)I2C_ADDRESS_ACC1,
				0x7F,
				1,
				(uint8_t *)&Addr_0x7f,
				1,
				10000
				);

		/* Error_Handler() function is called when Timeout error occurs.
		   When Acknowledge failure occurs (Slave don't acknowledge its address)
		   Master restarts communication */
		if (HAL_I2C_GetError(&hi2c1) != HAL_I2C_ERROR_AF && status != HAL_OK) {
			Error_Handler();
		}

		if(status != HAL_OK){
			break;
		}

		{
			// Get Part ID
			status = HAL_I2C_Mem_Read(
					&hi2c1,(uint16_t)I2C_ADDRESS_ACC1,
					0x00,
					1,
					(uint8_t *)aRxBuffer,
					2,
					10000);

			if (HAL_I2C_GetError(&hi2c1) != HAL_I2C_ERROR_AF && status != HAL_OK) {
				Error_Handler();
			}

			if(status != HAL_OK){
				break;
			}
		}

		if (aRxBuffer[0] != 0x71 && aRxBuffer[1] != 0x2) {
			break;
		}

		{
		    // Switch to Bank0, Register 0x7F write 0x00
			uint8_t Addr_0x7f = 0x00;
			status = HAL_I2C_Mem_Write(
					&hi2c1, (uint16_t)I2C_ADDRESS_ACC1,
					0x7F,
					1,
					(uint8_t *)&Addr_0x7f,
					1,
					10000
					);

			if (HAL_I2C_GetError(&hi2c1) != HAL_I2C_ERROR_AF && status != HAL_OK) {
				Error_Handler();
			}

			if(status != HAL_OK){
				break;
			}
		}

		{
		    // Code Reset, Register 0x7D write 0x5A
		    uint8_t Addr_0x7d = 0x5A;
			status = HAL_I2C_Mem_Write(
					&hi2c1, (uint16_t)I2C_ADDRESS_ACC1,
					0x7D,
					1,
					(uint8_t *)&Addr_0x7d,
					1,
					10000
					);

			if (HAL_I2C_GetError(&hi2c1) != HAL_I2C_ERROR_AF && status != HAL_OK) {
				Error_Handler();
			}

			if(status != HAL_OK){
				break;
			}
		}

		// Wait for 120ms
		HAL_Delay(120);

		{
			// Get Status Flag 0x05
			status = HAL_I2C_Mem_Read(
					&hi2c1,(uint16_t)I2C_ADDRESS_ACC1,
					0x05,
					1,
					(uint8_t *)aRxBuffer,
					2,
					10000);
			printf("Test>Status Flag: %x", (aRxBuffer[0]));

//			if (HAL_I2C_GetError(&hi2c1) != HAL_I2C_ERROR_AF && status != HAL_OK) {
//				Error_Handler();
//			}
//
//			if(status != HAL_OK){
//				break;
//			}
		}

		if (((aRxBuffer[0] >> 6) & 0x01) != 1){
			break;
		}


		{
			// Get Alert_Mode 0x03
			status = HAL_I2C_Mem_Read(
					&hi2c1,(uint16_t)I2C_ADDRESS_ACC1,
					0x03,
					1,
					(uint8_t *)aRxBuffer,
					2,
					10000);

			printf("Test>Alert_Mode: %x", (aRxBuffer[0] & 0x03));

			// Get One-Shot 0x26
			status = HAL_I2C_Mem_Read(
					&hi2c1,(uint16_t)I2C_ADDRESS_ACC1,
					0x26,
					1,
					(uint8_t *)aRxBuffer,
					2,
					10000);

			printf("Test>One-Shot: 0x%x", (aRxBuffer[0] & 0x03));

		}

	}while(0);

	return;
}

//
//void TestI2C3()
//{
//	int status;
//
//	HAL_GPIO_TogglePin(TOF_RESET_Port, TOF_RESET_Pin);
//	status = HAL_GPIO_ReadPin(TOF_RESET_Port, TOF_RESET_Pin);
//
//	status = HAL_GPIO_ReadPin(TOF_INIT_Port, TOF_INIT_Pin);
//
//	do{
//
//	    uint8_t Addr_0x7f = 0x00;
////		status = HAL_I2C_Mem_Write(
////				&hi2c3, (uint16_t)I2C_ADDRESS_TOF,
////				0x7F,
////				1,
////				(uint8_t *)&Addr_0x7f,
////				1,
////				10000
////				);
//		while (HAL_I2C_Mem_Write(
//				&hi2c3, (uint16_t)I2C_ADDRESS_TOF,
//				0x7F,
//				1,
//				(uint8_t *)&Addr_0x7f,
//				1,
//				10000
//			) != HAL_OK) {
//			/* Error_Handler() function is called when Timeout error occurs.
//			   When Acknowledge failure occurs (Slave don't acknowledge its address)
//			   Master restarts communication */
//			if (HAL_I2C_GetError(&hi2c3) != HAL_I2C_ERROR_AF) {
//				Error_Handler();
//			}
//		}
//		/* Error_Handler() function is called when Timeout error occurs.
//		   When Acknowledge failure occurs (Slave don't acknowledge its address)
//		   Master restarts communication */
//		if (HAL_I2C_GetError(&hi2c3) != HAL_I2C_ERROR_AF && status != HAL_OK) {
//			Error_Handler();
//		}
//
//		if(status != HAL_OK){
//			break;
//		}
//
//		status = HAL_I2C_Mem_Read(
//				&hi2c3,(uint16_t)I2C_ADDRESS_TOF,
//				0x00,
//				1,
//				(uint8_t *)aRxBuffer,
//				2,
//				10000);
//
//		if (HAL_I2C_GetError(&hi2c3) != HAL_I2C_ERROR_AF && status != HAL_OK) {
//			Error_Handler();
//		}
//
//		if(status != HAL_OK){
//			break;
//		}
//	}while(0);
//
//	return;
//}
//
//
///* Context buffer related to Frame context, contain different buffer value for a communication */
//I3C_XferTypeDef aI3C1_ContextBuffers[2] __attribute__((section("noncacheable_buffer")));
///* Buffer used by HAL to compute control data for the Private Communication */
//uint32_t aI3C1_ControlBuffer[0xF] __attribute__((section("noncacheable_buffer")));
//
///* Buffer used for transmission */
//uint8_t aI3C1_TxBuffer[] = {0x7f, 0x00};
//
//#define I3C1_TXBUFFERSIZE                      (COUNTOF(aI3C1_TxBuffer) - 1)
//#define I3C1_RXBUFFERSIZE                      I3C1_TXBUFFERSIZE
//
///* Buffer used for reception */
//uint8_t aI3C1_RxBuffer[I3C1_RXBUFFERSIZE] __attribute__((section("noncacheable_buffer")));
//
//
///* Descriptor for private data transmit */
//I3C_PrivateTypeDef aPrivateDescriptor[2] = \
//{
//	{SENSOR_ADDRESS_1, {aI3C1_TxBuffer, I3C1_TXBUFFERSIZE}, {NULL, 0}, HAL_I3C_DIRECTION_WRITE},
//	{SENSOR_ADDRESS_1, {NULL, 0}, {aI3C1_RxBuffer, I3C1_RXBUFFERSIZE}, HAL_I3C_DIRECTION_READ}
//};
//
//void TestI3C1()
//{
//	int status;
//	HAL_GPIO_TogglePin(THRMPL1_PD_GPIO_Port, THRMPL1_PD_Pin);
//	HAL_GPIO_TogglePin(THRMPL1_PD_GPIO_Port, THRMPL1_PD_Pin);
//	// Test I3C1
//	/*##- Prepare context buffers process ##################################*/
//	/* Prepare Transmit context buffer with the different parameters */
//	aI3C1_ContextBuffers[I3C_IDX_FRAME_1].CtrlBuf.pBuffer = aI3C1_ControlBuffer;
//	aI3C1_ContextBuffers[I3C_IDX_FRAME_1].CtrlBuf.Size    = 1;
//	aI3C1_ContextBuffers[I3C_IDX_FRAME_1].TxBuf.pBuffer   = aI3C1_TxBuffer;
//	aI3C1_ContextBuffers[I3C_IDX_FRAME_1].TxBuf.Size      = I3C1_TXBUFFERSIZE;
//
//	/* Prepare Receive context buffer with the different parameters */
//	aI3C1_ContextBuffers[I3C_IDX_FRAME_2].CtrlBuf.pBuffer = aI3C1_ControlBuffer;
//	aI3C1_ContextBuffers[I3C_IDX_FRAME_2].CtrlBuf.Size    = 1;
//	aI3C1_ContextBuffers[I3C_IDX_FRAME_2].RxBuf.pBuffer   = aI3C1_RxBuffer;
//	aI3C1_ContextBuffers[I3C_IDX_FRAME_2].RxBuf.Size      = I3C1_RXBUFFERSIZE;
//
//	/*##- Add context buffer transmit in Frame context #####################*/
//	if (HAL_I3C_AddDescToFrame(&hi3c1,
//							   NULL,
//							   &aPrivateDescriptor[I3C_IDX_FRAME_1],
//							   &aI3C1_ContextBuffers[I3C_IDX_FRAME_1],
//							   aI3C1_ContextBuffers[I3C_IDX_FRAME_1].CtrlBuf.Size,
//							   I2C_PRIVATE_WITHOUT_ARB_STOP)
//			!= HAL_OK)
//	{
//		/* Error_Handler() function is called when error occurs. */
//		Error_Handler();
//	}
//
//	status = HAL_I3C_Ctrl_Transmit_IT(&hi3c1, &aI3C1_ContextBuffers[I3C_IDX_FRAME_1]) ;
//	if (status!= HAL_OK)
//	{
//		/* Error_Handler() function is called when error occurs. */
//		Error_Handler();
//	}
//
//	while (HAL_I3C_GetState(&hi3c1) != HAL_I3C_STATE_READY)
//	{
//		status = 1;
//	}
//
//	return;
//}

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
#ifdef USE_FULL_ASSERT
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
