/* USER CODE BEGIN Header */
/**
 ******************************************************************************
 * @file           : main.c
 * @brief          : Main program body
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
/* Includes ------------------------------------------------------------------*/
#include "main.h"

/* Private includes ----------------------------------------------------------*/
/* USER CODE BEGIN Includes */
#include <stdio.h>
#include "ism330is.h"
#include "custom_bus.h"
#include <string.h>
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
MDF_HandleTypeDef AdfHandle0;
MDF_FilterConfigTypeDef AdfFilterConfig0;

DMA_HandleTypeDef handle_GPDMA1_Channel11;

TIM_HandleTypeDef htim1;

UART_HandleTypeDef huart1;
UART_HandleTypeDef huart2;

/* USER CODE BEGIN PV */
// SHT4X
uint32_t sht4x_serialnumber;

//ISM330IS
ISM330IS_Object_t ism330_obj_o;
ISM330IS_IO_t ism330_ctx;
uint8_t ism330_id;
ISM330IS_Capabilities_t ism330_cap;

typedef struct {
	int32_t x;
	int32_t y;
	int32_t z;
} acc;

typedef struct {
	int32_t x;
	int32_t y;
	int32_t z;
} gyro;

typedef struct {
	acc acc;
	gyro gyro;
} imu_data;

imu_data imu_sensor_data;

int32_t imu_sensor_array[768];
int32_t gyro_sensor_array[384];
int32_t acc_sensor_array[384];

//IMP34DT05
#define AUDIO_BUFFER_SIZE 1024
int16_t audio_rx[AUDIO_BUFFER_SIZE];
uint8_t AUDIO_MODE = 0;
uint8_t audio_test = 0;

uint8_t imu_error = 0;
uint8_t sht_error = 0;
uint8_t audio_error = 0;

uint8_t counter_timer = 0;
uint8_t flag_working = 0;

typedef enum {
	ONLY_ACCELEROMETER, ONLY_GYRO, ACCELEROMETER_AND_GYRO
} SensorMode;
SensorMode sensor_mode = ONLY_ACCELEROMETER;
uint32_t pins_led[3] = { LED6_Pin, LED7_Pin, LED8_Pin };

typedef enum {
	ACC_DISABLE = 0x00U, ACC_ENABLE = 0x01U
} IMU_AccConfigTypeDef;

typedef enum {
	GYRO_DISABLE = 0x00U, GYRO_ENABLE = 0x01U
} IMU_GyroConfigTypeDef;

/* USER CODE END PV */

/* Private function prototypes -----------------------------------------------*/
void SystemClock_Config(void);
static void MX_GPIO_Init(void);
static void MX_GPDMA1_Init(void);
static void MX_ICACHE_Init(void);
static void MX_ADF1_Init(void);
static void MX_USART1_UART_Init(void);
static void MX_USART2_UART_Init(void);
static void MX_TIM1_Init(void);
/* USER CODE BEGIN PFP */
char receivedChar[1];
volatile uint8_t rx_flag = 0;
uint16_t counter_state = 0;
uint8_t state = 0;
uint8_t i = 0;
char receivedDataBuffer[25];
uint8_t ism330_sensor_init(void);
void ism330is_sensor_check(void);
void sht40_sensor_check(void);
void imp34dt05_sensor_check(void);
void esp32_at_check(void);
void fill_accelerator_and_gyro_buffer(void);
void fill_accelerator_buffer(void);
void fill_gyro_buffer(void);
void StartTimer(void);
void StopTimer(void);
void ResetTimerCounter(void);
//void CheckButton(void);
void HAL_GPIO_EXTI_Rising_Callback(uint16_t GPIO_Pin);
void HAL_TIM_PeriodElapsedCallback(TIM_HandleTypeDef *htim);
uint8_t Audio_Init(void);

#ifdef __GNUC__
#define PUTCHAR_PROTOTYPE int __io_putchar(int ch)
#else
#define PUTCHAR_PROTOTYPE int fputc(int ch, FILE *f)
#endif

PUTCHAR_PROTOTYPE {
	HAL_UART_Transmit(&huart1, (uint8_t*) &ch, 1, HAL_MAX_DELAY);
	return ch;
}
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
  MX_GPDMA1_Init();
  MX_ICACHE_Init();
  MX_ADF1_Init();
  MX_USART1_UART_Init();
  MX_USART2_UART_Init();
  MX_TIM1_Init();
  /* USER CODE BEGIN 2 */
	ism330_sensor_init();
	HAL_GPIO_WritePin(GPIOB, LED6_Pin, GPIO_PIN_SET);
	counter_state = sensor_mode;
  /* USER CODE END 2 */

  /* Infinite loop */
  /* USER CODE BEGIN WHILE */
	while (1) {
		if ((HAL_GPIO_ReadPin(USER_BUTTON_GPIO_Port, USER_BUTTON_Pin) == GPIO_PIN_RESET)
				&& (flag_working == 0)) {
			StartTimer();
			while (HAL_GPIO_ReadPin(USER_BUTTON_GPIO_Port, USER_BUTTON_Pin) == GPIO_PIN_RESET)
				;
			if (counter_timer > 2) {
				if (counter_state == 0) {
					counter_state += 2;
				} else
					counter_state--;
				sensor_mode = counter_state;
				HAL_GPIO_WritePin(GPIOB, pins_led[counter_state],
						GPIO_PIN_RESET);
				HAL_Delay(500);
				HAL_GPIO_WritePin(GPIOB, pins_led[counter_state], GPIO_PIN_SET);
				HAL_Delay(500);
				StopTimer();
				ResetTimerCounter();
				flag_working = 1;
			} else {
				StopTimer();
				ResetTimerCounter();
				HAL_GPIO_WritePin(GPIOB, LED6_Pin, GPIO_PIN_RESET);
				HAL_GPIO_WritePin(GPIOB, LED7_Pin, GPIO_PIN_RESET);
				HAL_GPIO_WritePin(GPIOB, LED8_Pin, GPIO_PIN_RESET);
				HAL_GPIO_WritePin(GPIOB, pins_led[counter_state], GPIO_PIN_SET);
				if (counter_state == 0) {
					HAL_GPIO_WritePin(GPIOB, pins_led[counter_state + 2],
							GPIO_PIN_RESET);
				} else {
					HAL_GPIO_WritePin(GPIOB, pins_led[counter_state - 1],
							GPIO_PIN_RESET);
				}

			}
		}
		if (flag_working == 1) {
			HAL_NVIC_DisableIRQ(EXTI12_IRQn);
			switch (sensor_mode) {
			case ONLY_GYRO:
				while (1) {
					fill_gyro_buffer();

					for (int i = 0; i < 384; i++) {
						printf("%ld", gyro_sensor_array[i]);
						if (i != 383) {
							printf(" ");
						}
					}
					printf(" \n\r");
				}

				break;
			case ONLY_ACCELEROMETER:
				while (1) {
					fill_accelerator_buffer();

					for (int i = 0; i < 384; i++) {
						printf("%ld", acc_sensor_array[i]);
						if (i != 383) {
							printf(" ");
						}
					}
					printf("\r\n");
				}
				break;
			case ACCELEROMETER_AND_GYRO:
				while (1) {
					fill_accelerator_and_gyro_buffer();
					for (int i = 0; i < 768; i++) {
						printf("%ld", imu_sensor_array[i]);
						if (i != 767) {
							printf(" ");
						}
					}
					printf(" \n\r");
				}
				break;
			default:
				printf("ERROR.\n");
				break;
			}
		}
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

  /** Configure the main internal regulator output voltage
  */
  if (HAL_PWREx_ControlVoltageScaling(PWR_REGULATOR_VOLTAGE_SCALE1) != HAL_OK)
  {
    Error_Handler();
  }

  /** Initializes the CPU, AHB and APB buses clocks
  */
  RCC_OscInitStruct.OscillatorType = RCC_OSCILLATORTYPE_MSI|RCC_OSCILLATORTYPE_MSIK;
  RCC_OscInitStruct.MSIState = RCC_MSI_ON;
  RCC_OscInitStruct.MSICalibrationValue = RCC_MSICALIBRATION_DEFAULT;
  RCC_OscInitStruct.MSIClockRange = RCC_MSIRANGE_4;
  RCC_OscInitStruct.MSIKClockRange = RCC_MSIKRANGE_3;
  RCC_OscInitStruct.MSIKState = RCC_MSIK_ON;
  RCC_OscInitStruct.PLL.PLLState = RCC_PLL_ON;
  RCC_OscInitStruct.PLL.PLLSource = RCC_PLLSOURCE_MSI;
  RCC_OscInitStruct.PLL.PLLMBOOST = RCC_PLLMBOOST_DIV1;
  RCC_OscInitStruct.PLL.PLLM = 1;
  RCC_OscInitStruct.PLL.PLLN = 80;
  RCC_OscInitStruct.PLL.PLLP = 2;
  RCC_OscInitStruct.PLL.PLLQ = 2;
  RCC_OscInitStruct.PLL.PLLR = 2;
  RCC_OscInitStruct.PLL.PLLRGE = RCC_PLLVCIRANGE_0;
  RCC_OscInitStruct.PLL.PLLFRACN = 0;
  if (HAL_RCC_OscConfig(&RCC_OscInitStruct) != HAL_OK)
  {
    Error_Handler();
  }

  /** Initializes the CPU, AHB and APB buses clocks
  */
  RCC_ClkInitStruct.ClockType = RCC_CLOCKTYPE_HCLK|RCC_CLOCKTYPE_SYSCLK
                              |RCC_CLOCKTYPE_PCLK1|RCC_CLOCKTYPE_PCLK2
                              |RCC_CLOCKTYPE_PCLK3;
  RCC_ClkInitStruct.SYSCLKSource = RCC_SYSCLKSOURCE_PLLCLK;
  RCC_ClkInitStruct.AHBCLKDivider = RCC_SYSCLK_DIV1;
  RCC_ClkInitStruct.APB1CLKDivider = RCC_HCLK_DIV1;
  RCC_ClkInitStruct.APB2CLKDivider = RCC_HCLK_DIV1;
  RCC_ClkInitStruct.APB3CLKDivider = RCC_HCLK_DIV1;

  if (HAL_RCC_ClockConfig(&RCC_ClkInitStruct, FLASH_LATENCY_4) != HAL_OK)
  {
    Error_Handler();
  }
}

/**
  * @brief ADF1 Initialization Function
  * @param None
  * @retval None
  */
static void MX_ADF1_Init(void)
{

  /* USER CODE BEGIN ADF1_Init 0 */

  /* USER CODE END ADF1_Init 0 */

  /* USER CODE BEGIN ADF1_Init 1 */

  /* USER CODE END ADF1_Init 1 */

  /**
    AdfHandle0 structure initialization and HAL_MDF_Init function call
  */
  AdfHandle0.Instance = ADF1_Filter0;
  AdfHandle0.Init.CommonParam.ProcClockDivider = 1;
  AdfHandle0.Init.CommonParam.OutputClock.Activation = ENABLE;
  AdfHandle0.Init.CommonParam.OutputClock.Pins = MDF_OUTPUT_CLOCK_0;
  AdfHandle0.Init.CommonParam.OutputClock.Divider = 4;
  AdfHandle0.Init.CommonParam.OutputClock.Trigger.Activation = DISABLE;
  AdfHandle0.Init.SerialInterface.Activation = ENABLE;
  AdfHandle0.Init.SerialInterface.Mode = MDF_SITF_NORMAL_SPI_MODE;
  AdfHandle0.Init.SerialInterface.ClockSource = MDF_SITF_CCK0_SOURCE;
  AdfHandle0.Init.SerialInterface.Threshold = 31;
  AdfHandle0.Init.FilterBistream = MDF_BITSTREAM0_FALLING;
  if (HAL_MDF_Init(&AdfHandle0) != HAL_OK)
  {
    Error_Handler();
  }

  /**
    AdfFilterConfig0 structure initialization

    WARNING : only structure is filled, no specific init function call for filter
  */
  AdfFilterConfig0.DataSource = MDF_DATA_SOURCE_BSMX;
  AdfFilterConfig0.Delay = 0;
  AdfFilterConfig0.CicMode = MDF_ONE_FILTER_SINC4;
  AdfFilterConfig0.DecimationRatio = 64;
  AdfFilterConfig0.Gain = 0;
  AdfFilterConfig0.ReshapeFilter.Activation = DISABLE;
  AdfFilterConfig0.HighPassFilter.Activation = DISABLE;
  AdfFilterConfig0.SoundActivity.Activation = ENABLE;
  AdfFilterConfig0.SoundActivity.Mode = MDF_SAD_VOICE_ACTIVITY_DETECTOR;
  AdfFilterConfig0.SoundActivity.FrameSize = MDF_SAD_8_PCM_SAMPLES;
  AdfFilterConfig0.SoundActivity.Hysteresis = DISABLE;
  AdfFilterConfig0.SoundActivity.SoundTriggerEvent = MDF_SAD_ENTER_DETECT;
  AdfFilterConfig0.SoundActivity.DataMemoryTransfer = MDF_SAD_MEMORY_TRANSFER_ALWAYS;
  AdfFilterConfig0.SoundActivity.MinNoiseLevel = 200;
  AdfFilterConfig0.SoundActivity.HangoverWindow = MDF_SAD_HANGOVER_4_FRAMES;
  AdfFilterConfig0.SoundActivity.LearningFrames = MDF_SAD_LEARNING_2_FRAMES;
  AdfFilterConfig0.SoundActivity.AmbientNoiseSlope = 0;
  AdfFilterConfig0.SoundActivity.SignalNoiseThreshold = MDF_SAD_SIGNAL_NOISE_18DB;
  AdfFilterConfig0.SoundActivity.SoundLevelInterrupt = DISABLE;
  AdfFilterConfig0.AcquisitionMode = MDF_MODE_ASYNC_CONT;
  AdfFilterConfig0.FifoThreshold = MDF_FIFO_THRESHOLD_NOT_EMPTY;
  AdfFilterConfig0.DiscardSamples = 0;
  /* USER CODE BEGIN ADF1_Init 2 */

  /* USER CODE END ADF1_Init 2 */

}

/**
  * @brief GPDMA1 Initialization Function
  * @param None
  * @retval None
  */
static void MX_GPDMA1_Init(void)
{

  /* USER CODE BEGIN GPDMA1_Init 0 */

  /* USER CODE END GPDMA1_Init 0 */

  /* Peripheral clock enable */
  __HAL_RCC_GPDMA1_CLK_ENABLE();

  /* GPDMA1 interrupt Init */
    HAL_NVIC_SetPriority(GPDMA1_Channel11_IRQn, 0, 0);
    HAL_NVIC_EnableIRQ(GPDMA1_Channel11_IRQn);

  /* USER CODE BEGIN GPDMA1_Init 1 */

  /* USER CODE END GPDMA1_Init 1 */
  handle_GPDMA1_Channel11.Instance = GPDMA1_Channel11;
  handle_GPDMA1_Channel11.InitLinkedList.Priority = DMA_HIGH_PRIORITY;
  handle_GPDMA1_Channel11.InitLinkedList.LinkStepMode = DMA_LSM_FULL_EXECUTION;
  handle_GPDMA1_Channel11.InitLinkedList.LinkAllocatedPort = DMA_LINK_ALLOCATED_PORT0;
  handle_GPDMA1_Channel11.InitLinkedList.TransferEventMode = DMA_TCEM_LAST_LL_ITEM_TRANSFER;
  handle_GPDMA1_Channel11.InitLinkedList.LinkedListMode = DMA_LINKEDLIST_CIRCULAR;
  if (HAL_DMAEx_List_Init(&handle_GPDMA1_Channel11) != HAL_OK)
  {
    Error_Handler();
  }
  if (HAL_DMA_ConfigChannelAttributes(&handle_GPDMA1_Channel11, DMA_CHANNEL_NPRIV) != HAL_OK)
  {
    Error_Handler();
  }
  /* USER CODE BEGIN GPDMA1_Init 2 */

  /* USER CODE END GPDMA1_Init 2 */

}

/**
  * @brief ICACHE Initialization Function
  * @param None
  * @retval None
  */
static void MX_ICACHE_Init(void)
{

  /* USER CODE BEGIN ICACHE_Init 0 */

  /* USER CODE END ICACHE_Init 0 */

  /* USER CODE BEGIN ICACHE_Init 1 */

  /* USER CODE END ICACHE_Init 1 */

  /** Enable instruction cache in 1-way (direct mapped cache)
  */
  if (HAL_ICACHE_ConfigAssociativityMode(ICACHE_1WAY) != HAL_OK)
  {
    Error_Handler();
  }
  if (HAL_ICACHE_Enable() != HAL_OK)
  {
    Error_Handler();
  }
  /* USER CODE BEGIN ICACHE_Init 2 */

  /* USER CODE END ICACHE_Init 2 */

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
  htim1.Init.Prescaler = 16000-1;
  htim1.Init.CounterMode = TIM_COUNTERMODE_UP;
  htim1.Init.Period = 9999;
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
  sMasterConfig.MasterOutputTrigger2 = TIM_TRGO2_RESET;
  sMasterConfig.MasterSlaveMode = TIM_MASTERSLAVEMODE_DISABLE;
  if (HAL_TIMEx_MasterConfigSynchronization(&htim1, &sMasterConfig) != HAL_OK)
  {
    Error_Handler();
  }
  /* USER CODE BEGIN TIM1_Init 2 */

  /* USER CODE END TIM1_Init 2 */

}

/**
  * @brief USART1 Initialization Function
  * @param None
  * @retval None
  */
static void MX_USART1_UART_Init(void)
{

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
  if (HAL_UART_Init(&huart1) != HAL_OK)
  {
    Error_Handler();
  }
  if (HAL_UARTEx_SetTxFifoThreshold(&huart1, UART_TXFIFO_THRESHOLD_1_8) != HAL_OK)
  {
    Error_Handler();
  }
  if (HAL_UARTEx_SetRxFifoThreshold(&huart1, UART_RXFIFO_THRESHOLD_1_8) != HAL_OK)
  {
    Error_Handler();
  }
  if (HAL_UARTEx_DisableFifoMode(&huart1) != HAL_OK)
  {
    Error_Handler();
  }
  /* USER CODE BEGIN USART1_Init 2 */

  /* USER CODE END USART1_Init 2 */

}

/**
  * @brief USART2 Initialization Function
  * @param None
  * @retval None
  */
static void MX_USART2_UART_Init(void)
{

  /* USER CODE BEGIN USART2_Init 0 */

  /* USER CODE END USART2_Init 0 */

  /* USER CODE BEGIN USART2_Init 1 */

  /* USER CODE END USART2_Init 1 */
  huart2.Instance = USART2;
  huart2.Init.BaudRate = 115200;
  huart2.Init.WordLength = UART_WORDLENGTH_8B;
  huart2.Init.StopBits = UART_STOPBITS_1;
  huart2.Init.Parity = UART_PARITY_NONE;
  huart2.Init.Mode = UART_MODE_TX_RX;
  huart2.Init.HwFlowCtl = UART_HWCONTROL_NONE;
  huart2.Init.OverSampling = UART_OVERSAMPLING_16;
  huart2.Init.OneBitSampling = UART_ONE_BIT_SAMPLE_DISABLE;
  huart2.Init.ClockPrescaler = UART_PRESCALER_DIV1;
  huart2.AdvancedInit.AdvFeatureInit = UART_ADVFEATURE_NO_INIT;
  if (HAL_UART_Init(&huart2) != HAL_OK)
  {
    Error_Handler();
  }
  if (HAL_UARTEx_SetTxFifoThreshold(&huart2, UART_TXFIFO_THRESHOLD_1_8) != HAL_OK)
  {
    Error_Handler();
  }
  if (HAL_UARTEx_SetRxFifoThreshold(&huart2, UART_RXFIFO_THRESHOLD_1_8) != HAL_OK)
  {
    Error_Handler();
  }
  if (HAL_UARTEx_DisableFifoMode(&huart2) != HAL_OK)
  {
    Error_Handler();
  }
  /* USER CODE BEGIN USART2_Init 2 */

  /* USER CODE END USART2_Init 2 */

}

/**
  * @brief GPIO Initialization Function
  * @param None
  * @retval None
  */
static void MX_GPIO_Init(void)
{
  GPIO_InitTypeDef GPIO_InitStruct = {0};
/* USER CODE BEGIN MX_GPIO_Init_1 */
/* USER CODE END MX_GPIO_Init_1 */

  /* GPIO Ports Clock Enable */
  __HAL_RCC_GPIOC_CLK_ENABLE();
  __HAL_RCC_GPIOA_CLK_ENABLE();
  __HAL_RCC_GPIOB_CLK_ENABLE();

  /*Configure GPIO pin Output Level */
  HAL_GPIO_WritePin(GPIOC, LED9_Pin|LED10_Pin, GPIO_PIN_RESET);

  /*Configure GPIO pin Output Level */
  HAL_GPIO_WritePin(GPIOA, LED4_Pin|LED1_Pin|MDL_EN_Pin, GPIO_PIN_RESET);

  /*Configure GPIO pin Output Level */
  HAL_GPIO_WritePin(GPIOB, LED2_Pin|LED3_Pin|LED5_Pin|LED6_Pin
                          |LED7_Pin|LED8_Pin, GPIO_PIN_RESET);

  /*Configure GPIO pins : LED9_Pin LED10_Pin */
  GPIO_InitStruct.Pin = LED9_Pin|LED10_Pin;
  GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
  HAL_GPIO_Init(GPIOC, &GPIO_InitStruct);

  /*Configure GPIO pins : LED4_Pin LED1_Pin MDL_EN_Pin */
  GPIO_InitStruct.Pin = LED4_Pin|LED1_Pin|MDL_EN_Pin;
  GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
  HAL_GPIO_Init(GPIOA, &GPIO_InitStruct);

  /*Configure GPIO pins : INT1_ISM330_Pin INT2_ISM330_Pin */
  GPIO_InitStruct.Pin = INT1_ISM330_Pin|INT2_ISM330_Pin;
  GPIO_InitStruct.Mode = GPIO_MODE_IT_RISING;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  HAL_GPIO_Init(GPIOA, &GPIO_InitStruct);

  /*Configure GPIO pins : LED2_Pin LED3_Pin LED5_Pin LED6_Pin
                           LED7_Pin LED8_Pin */
  GPIO_InitStruct.Pin = LED2_Pin|LED3_Pin|LED5_Pin|LED6_Pin
                          |LED7_Pin|LED8_Pin;
  GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
  HAL_GPIO_Init(GPIOB, &GPIO_InitStruct);

  /*Configure GPIO pin : USER_BUTTON_Pin */
  GPIO_InitStruct.Pin = USER_BUTTON_Pin;
  GPIO_InitStruct.Mode = GPIO_MODE_IT_FALLING;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  HAL_GPIO_Init(USER_BUTTON_GPIO_Port, &GPIO_InitStruct);

  /* EXTI interrupt init*/
  HAL_NVIC_SetPriority(EXTI12_IRQn, 0, 0);
  HAL_NVIC_EnableIRQ(EXTI12_IRQn);

/* USER CODE BEGIN MX_GPIO_Init_2 */
/* USER CODE END MX_GPIO_Init_2 */
}

/* USER CODE BEGIN 4 */
uint8_t ism330_sensor_init(void) {
	ism330_ctx.BusType = ISM330IS_I2C_BUS;
	ism330_ctx.Address = ISM330IS_I2C_ADD_H;
	ism330_ctx.Init = BSP_I2C1_Init;
	ism330_ctx.DeInit = BSP_I2C1_DeInit;
	ism330_ctx.ReadReg = BSP_I2C1_ReadReg;
	ism330_ctx.WriteReg = BSP_I2C1_WriteReg;
	ism330_ctx.GetTick = BSP_GetTick;

	if (ISM330IS_RegisterBusIO(&ism330_obj_o, &ism330_ctx) != ISM330IS_OK)
		return 1;

	if (ISM330IS_ReadID(&ism330_obj_o, &ism330_id) != ISM330IS_OK)
		return 1;

	if (ism330_id != ISM330IS_ID)
		return 1;

	if (ISM330IS_Init(&ism330_obj_o) != ISM330IS_OK)
		return 1;

	if (ISM330IS_ACC_Set_INT1_DRDY(&ism330_obj_o, 1) != ISM330IS_OK)
		return 1;

	if (ISM330IS_ACC_Enable(&ism330_obj_o) != ISM330IS_OK)
		return 1;

	if (ISM330IS_GYRO_Enable(&ism330_obj_o) != ISM330IS_OK)
		return 1;

	return 0;
}

void ism330_sensor_process(imu_data *sensor_data) {
	ISM330IS_Axes_t ism330_axes;
	if (ISM330IS_ACC_GetAxes(&ism330_obj_o, &ism330_axes) == 0) {
		sensor_data->acc.x = ism330_axes.x;
		sensor_data->acc.y = ism330_axes.y;
		sensor_data->acc.z = ism330_axes.z;
	} else {
		sensor_data->acc.x = 0;
		sensor_data->acc.y = 0;
		sensor_data->acc.z = 0;
	}

	if (ISM330IS_GYRO_GetAxes(&ism330_obj_o, &ism330_axes) == 0) {
		sensor_data->gyro.x = ism330_axes.x;
		sensor_data->gyro.y = ism330_axes.y;
		sensor_data->gyro.z = ism330_axes.z;
	} else {
		sensor_data->gyro.x = 0;
		sensor_data->gyro.y = 0;
		sensor_data->gyro.z = 0;
	}
}


void ism330is_sensor_check(void)
{
	printf("\nISM330 Initialized\r\n");
	if (ism330_sensor_init() != 0) {
		printf("\nISM330 Initialization error\r\n");
		imu_error = 1;
	} else {
		HAL_Delay(300);
		ism330_sensor_process(&imu_sensor_data);

		printf("\nACC X: %ld  ACC Y: %ld ACC Z: %ld\r\n", imu_sensor_data.acc.x,
				imu_sensor_data.acc.y, imu_sensor_data.acc.z);
		printf("\nGYRO X: %ld  GYRO Y: %ld GYRO Z: %ld\r\n",
				imu_sensor_data.gyro.x, imu_sensor_data.gyro.y,
				imu_sensor_data.gyro.z);
		printf("\nISM330IS OK\r\n");
	}
	ISM330IS_DeInit(&ism330_obj_o);
	printf("\nISM330IS DeInitialized\r\n");
}

void fill_accelerator_and_gyro_buffer(void)
{
	  for(int i = 0; i < 128; i ++)
	  {
		  ism330_sensor_process(&imu_sensor_data);
		  imu_sensor_array[i*6] = (float)imu_sensor_data.acc.x;
		  imu_sensor_array[i*6+1] = (float)imu_sensor_data.acc.y;
		  imu_sensor_array[i*6+2] = (float)imu_sensor_data.acc.z;
		  imu_sensor_array[i*6+3] = (float)imu_sensor_data.gyro.x/100;
		  imu_sensor_array[i*6+4] = (float)imu_sensor_data.gyro.y/100;
		  imu_sensor_array[i*6+5] = (float)imu_sensor_data.gyro.z/100;
		  HAL_Delay(8);
	  }
}

void fill_accelerator_buffer(void)
{
	  for(int i = 0; i < 128; i ++)
	  {
		  ism330_sensor_process(&imu_sensor_data);
		  acc_sensor_array[i*3] = (float)imu_sensor_data.acc.x;
		  acc_sensor_array[i*3+1] = (float)imu_sensor_data.acc.y;
		  acc_sensor_array[i*3+2] = (float)imu_sensor_data.acc.z;
		  HAL_Delay(8);
	  }
}

void fill_gyro_buffer(void)
{
	  for(int i = 0; i < 128; i ++)
	  {
		  ism330_sensor_process(&imu_sensor_data);
		  gyro_sensor_array[i*3] = (float)imu_sensor_data.gyro.x/100;
		  gyro_sensor_array[i*3+1] = (float)imu_sensor_data.gyro.y/100;
		  gyro_sensor_array[i*3+2] = (float)imu_sensor_data.gyro.z/100;
		  HAL_Delay(8);
	  }
}

//void CheckButton() {
//    if (button_is_pressed()) { // Butona basılıysa
//        counter_state = (counter_timer >= 3) ? 2 : 1; // 3 saniyeden fazla basılı tutulursa modu değiştir
//    } else {
//        counter_state = 0; // Buton serbest bırakıldığında sayaç sıfırlanır
//        counter_timer = 0;
//    }
//}

void HAL_GPIO_EXTI_Falling_Callback(uint16_t GPIO_Pin)
{
	if(GPIO_Pin == USER_BUTTON_Pin)
	{
		counter_state++;
		if(counter_state>2)
			counter_state = 0;
	}
}
void HAL_TIM_PeriodElapsedCallback(TIM_HandleTypeDef *htim)
{
	if(htim->Instance == TIM1)
	{
		counter_timer++;
	}
}

void StartTimer(void)
{
	HAL_TIM_Base_Start_IT(&htim1);
}

void StopTimer(void)
{
	HAL_TIM_Base_Stop_IT(&htim1);
}

void ResetTimerCounter(void)
{
	counter_timer = 0;
}
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
