/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * File Name          : freertos.c
  * Description        : Code for freertos applications
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
#include "FreeRTOS.h"
#include "task.h"
#include "main.h"
#include "cmsis_os.h"

/* Private includes ----------------------------------------------------------*/
/* USER CODE BEGIN Includes */
#include "usart.h"
#include "vl53l0x_api.h"
#include <stdio.h>
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
/* USER CODE BEGIN Variables */

extern I2C_HandleTypeDef hi2c1;
extern UART_HandleTypeDef huart2;
VL53L0X_RangingMeasurementData_t RangingData;

/* USER CODE END Variables */
/* Definitions for defaultTask */
osThreadId_t defaultTaskHandle;
const osThreadAttr_t defaultTask_attributes = {
  .name = "defaultTask",
  .stack_size = 128 * 4,
  .priority = (osPriority_t) osPriorityNormal,
};
/* Definitions for SensorTask */
osThreadId_t SensorTaskHandle;
const osThreadAttr_t SensorTask_attributes = {
  .name = "SensorTask",
  .stack_size = 512 * 4,
  .priority = (osPriority_t) osPriorityHigh,
};
/* Definitions for CANTxTask */
osThreadId_t CANTxTaskHandle;
const osThreadAttr_t CANTxTask_attributes = {
  .name = "CANTxTask",
  .stack_size = 256 * 4,
  .priority = (osPriority_t) osPriorityNormal,
};
/* Definitions for MQTTTask */
osThreadId_t MQTTTaskHandle;
const osThreadAttr_t MQTTTask_attributes = {
  .name = "MQTTTask",
  .stack_size = 256 * 4,
  .priority = (osPriority_t) osPriorityLow,
};
/* Definitions for FeedbackTask */
osThreadId_t FeedbackTaskHandle;
const osThreadAttr_t FeedbackTask_attributes = {
  .name = "FeedbackTask",
  .stack_size = 256 * 4,
  .priority = (osPriority_t) osPriorityNormal,
};

/* Private function prototypes -----------------------------------------------*/
/* USER CODE BEGIN FunctionPrototypes */

/* USER CODE END FunctionPrototypes */

void StartDefaultTask(void *argument);
void StartSensorTask(void *argument);
void StartCANTxTask(void *argument);
void StartMQTTTask(void *argument);
void StartFeedbackTask(void *argument);

void MX_FREERTOS_Init(void); /* (MISRA C 2004 rule 8.1) */

/**
  * @brief  FreeRTOS initialization
  * @param  None
  * @retval None
  */
void MX_FREERTOS_Init(void) {
  /* USER CODE BEGIN Init */

  /* USER CODE END Init */

  /* USER CODE BEGIN RTOS_MUTEX */
  /* add mutexes, ... */
  /* USER CODE END RTOS_MUTEX */

  /* USER CODE BEGIN RTOS_SEMAPHORES */
  /* add semaphores, ... */
  /* USER CODE END RTOS_SEMAPHORES */

  /* USER CODE BEGIN RTOS_TIMERS */
  /* start timers, add new ones, ... */
  /* USER CODE END RTOS_TIMERS */

  /* USER CODE BEGIN RTOS_QUEUES */
  /* add queues, ... */
  /* USER CODE END RTOS_QUEUES */

  /* Create the thread(s) */
  /* creation of defaultTask */
  defaultTaskHandle = osThreadNew(StartDefaultTask, NULL, &defaultTask_attributes);

  /* creation of SensorTask */
  SensorTaskHandle = osThreadNew(StartSensorTask, NULL, &SensorTask_attributes);

  /* creation of CANTxTask */
  CANTxTaskHandle = osThreadNew(StartCANTxTask, NULL, &CANTxTask_attributes);

  /* creation of MQTTTask */
  MQTTTaskHandle = osThreadNew(StartMQTTTask, NULL, &MQTTTask_attributes);

  /* creation of FeedbackTask */
  FeedbackTaskHandle = osThreadNew(StartFeedbackTask, NULL, &FeedbackTask_attributes);

  /* USER CODE BEGIN RTOS_THREADS */
  /* add threads, ... */
  /* USER CODE END RTOS_THREADS */

  /* USER CODE BEGIN RTOS_EVENTS */
  /* add events, ... */
  /* USER CODE END RTOS_EVENTS */

}

/* USER CODE BEGIN Header_StartDefaultTask */
/**
  * @brief  Function implementing the defaultTask thread.
  * @param  argument: Not used
  * @retval None
  */
/* USER CODE END Header_StartDefaultTask */
void StartDefaultTask(void *argument)
{
  /* USER CODE BEGIN StartDefaultTask */
  /* Infinite loop */
  for(;;)
  {
    osDelay(1);
  }
  /* USER CODE END StartDefaultTask */
}

/* USER CODE BEGIN Header_StartSensorTask */
/**
* @brief Function implementing the SensorTask thread.
* @param argument: Not used
* @retval None
*/
/* USER CODE END Header_StartSensorTask */
void StartSensorTask(void *argument)
{
  /* USER CODE BEGIN StartSensorTask */
  char msg[100];
  VL53L0X_Dev_t MyDevice;
  VL53L0X_DEV Dev = &MyDevice;
  uint32_t refSpadCount;
  uint8_t isApertureSpads;
  uint8_t VhvSettings;
  uint8_t PhaseCal;

  Dev->I2cDevAddr = 0x52;

  VL53L0X_DataInit(Dev);
  VL53L0X_StaticInit(Dev);
  VL53L0X_PerformRefCalibration(Dev, &VhvSettings, &PhaseCal);
  VL53L0X_PerformRefSpadManagement(Dev, &refSpadCount, &isApertureSpads);
  VL53L0X_SetDeviceMode(Dev, VL53L0X_DEVICEMODE_SINGLE_RANGING);

  for(;;)
  {
    VL53L0X_PerformSingleRangingMeasurement(Dev, &RangingData);
    sprintf(msg, "Distance: %d mm\r\n", RangingData.RangeMilliMeter);
    HAL_UART_Transmit(&huart2, (uint8_t*)msg, strlen(msg), 100);
    osDelay(200);
  }
  /* USER CODE END StartSensorTask */
}

/* USER CODE BEGIN Header_StartCANTxTask */
/**
* @brief Function implementing the CANTxTask thread.
* @param argument: Not used
* @retval None
*/
/* USER CODE END Header_StartCANTxTask */
void StartCANTxTask(void *argument)
{
  /* USER CODE BEGIN StartCANTxTask */
  /* Infinite loop */
  for(;;)
  {
    osDelay(1);
  }
  /* USER CODE END StartCANTxTask */
}

/* USER CODE BEGIN Header_StartMQTTTask */
/**
* @brief Function implementing the MQTTTask thread.
* @param argument: Not used
* @retval None
*/
/* USER CODE END Header_StartMQTTTask */
void StartMQTTTask(void *argument)
{
  /* USER CODE BEGIN StartMQTTTask */
  /* Infinite loop */
  for(;;)
  {
    osDelay(1);
  }
  /* USER CODE END StartMQTTTask */
}

/* USER CODE BEGIN Header_StartFeedbackTask */
/**
* @brief Function implementing the FeedbackTask thread.
* @param argument: Not used
* @retval None
*/
/* USER CODE END Header_StartFeedbackTask */
void StartFeedbackTask(void *argument)
{
  /* USER CODE BEGIN StartFeedbackTask */
  for(;;)
  {
    if (RangingData.RangeMilliMeter < 300) {
      HAL_GPIO_WritePin(GPIOA, GPIO_PIN_7, GPIO_PIN_SET);
    } else {
      HAL_GPIO_WritePin(GPIOA, GPIO_PIN_7, GPIO_PIN_RESET);
    }
    osDelay(100);
  }
  /* USER CODE END StartFeedbackTask */
}

/* Private application code --------------------------------------------------*/
/* USER CODE BEGIN Application */

/* USER CODE END Application */

