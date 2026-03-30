/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * File Name          : freertos.c
  * Description        : Code for freertos applications
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
#include "FreeRTOS.h"
#include "task.h"
#include "main.h"
#include "cmsis_os.h"

/* Private includes ----------------------------------------------------------*/
/* USER CODE BEGIN Includes */
#include "bsp_imu.h"
#include "app.h"

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

/* USER CODE END Variables */
osThreadId defaultTaskHandle;
osThreadId ImuTaskHandle;
osThreadId WinchTaskHandle;
osThreadId TrjTaskHandle;
osThreadId UploadTaskHandle;
osThreadId SendPoseTaskHandle;

/* Private function prototypes -----------------------------------------------*/
/* USER CODE BEGIN FunctionPrototypes */

/* USER CODE END FunctionPrototypes */

void StartDefaultTask(void const * argument);
void StartImuTask(void const * argument);
void StartWinchTask(void const * argument);
void StartTrjTask(void const * argument);
void StartUploadTask(void const * argument);
void StartSendPoseTask(void const * argument);

void MX_FREERTOS_Init(void); /* (MISRA C 2004 rule 8.1) */

/* GetIdleTaskMemory prototype (linked to static allocation support) */
void vApplicationGetIdleTaskMemory( StaticTask_t **ppxIdleTaskTCBBuffer, StackType_t **ppxIdleTaskStackBuffer, uint32_t *pulIdleTaskStackSize );

/* USER CODE BEGIN GET_IDLE_TASK_MEMORY */
static StaticTask_t xIdleTaskTCBBuffer;
static StackType_t xIdleStack[configMINIMAL_STACK_SIZE];

void vApplicationGetIdleTaskMemory( StaticTask_t **ppxIdleTaskTCBBuffer, StackType_t **ppxIdleTaskStackBuffer, uint32_t *pulIdleTaskStackSize )
{
  *ppxIdleTaskTCBBuffer = &xIdleTaskTCBBuffer;
  *ppxIdleTaskStackBuffer = &xIdleStack[0];
  *pulIdleTaskStackSize = configMINIMAL_STACK_SIZE;
  /* place for user code */
}
/* USER CODE END GET_IDLE_TASK_MEMORY */

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
  /* definition and creation of defaultTask */
  osThreadDef(defaultTask, StartDefaultTask, osPriorityNormal, 0, 128);
  defaultTaskHandle = osThreadCreate(osThread(defaultTask), NULL);

  /* definition and creation of ImuTask */
  osThreadDef(ImuTask, StartImuTask, osPriorityIdle, 0, 128);
  ImuTaskHandle = osThreadCreate(osThread(ImuTask), NULL);

  /* definition and creation of WinchTask */
  osThreadDef(WinchTask, StartWinchTask, osPriorityIdle, 0, 128);
  WinchTaskHandle = osThreadCreate(osThread(WinchTask), NULL);

  /* definition and creation of TrjTask */
  osThreadDef(TrjTask, StartTrjTask, osPriorityIdle, 0, 128);
  TrjTaskHandle = osThreadCreate(osThread(TrjTask), NULL);

  /* definition and creation of UploadTask */
  osThreadDef(UploadTask, StartUploadTask, osPriorityIdle, 0, 128);
  UploadTaskHandle = osThreadCreate(osThread(UploadTask), NULL);

  /* definition and creation of SendPoseTask */
  osThreadDef(SendPoseTask, StartSendPoseTask, osPriorityIdle, 0, 128);
  SendPoseTaskHandle = osThreadCreate(osThread(SendPoseTask), NULL);

  /* USER CODE BEGIN RTOS_THREADS */
  /* add threads, ... */
  /* USER CODE END RTOS_THREADS */

}

/* USER CODE BEGIN Header_StartDefaultTask */
/**
  * @brief  Function implementing the defaultTask thread.
  * @param  argument: Not used
  * @retval None
  */
/* USER CODE END Header_StartDefaultTask */
void StartDefaultTask(void const * argument)
{
  /* USER CODE BEGIN StartDefaultTask */
  /* Infinite loop */
  for(;;)
  {
    osDelay(1);
  }
  /* USER CODE END StartDefaultTask */
}

/* USER CODE BEGIN Header_StartImuTask */
/**
* @brief Function implementing the ImuTask thread.
* @param argument: Not used
* @retval None
*/
/* USER CODE END Header_StartImuTask */
void StartImuTask(void const * argument)
{
  /* USER CODE BEGIN StartImuTask */
  /* Infinite loop */
  for(;;)
  {
    imu_update();
    osDelay(5);
  }
  /* USER CODE END StartImuTask */
}

/* USER CODE BEGIN Header_StartWinchTask */
/**
* @brief Function implementing the WinchTask thread.
* @param argument: Not used
* @retval None
*/
/* USER CODE END Header_StartWinchTask */
void StartWinchTask(void const * argument)
{
  /* USER CODE BEGIN StartWinchTask */
  portTickType currentTimeWinch;
  currentTimeWinch = xTaskGetTickCount();
  /* Infinite loop */
  for(;;)
  {
    APP_WinchTask();
    vTaskDelayUntil(&currentTimeWinch, 5);
  }
  /* USER CODE END StartWinchTask */
}

/* USER CODE BEGIN Header_StartTrjTask */
/**
* @brief Function implementing the TrjTask thread.
* @param argument: Not used
* @retval None
*/
/* USER CODE END Header_StartTrjTask */
void StartTrjTask(void const * argument)
{
  /* USER CODE BEGIN StartTrjTask */
  portTickType currentTimeTrj;
  currentTimeTrj = xTaskGetTickCount();
  /* Infinite loop */
  for(;;)
  {
    APP_StartTrj();
    vTaskDelayUntil(&currentTimeTrj, 10);
  }
  /* USER CODE END StartTrjTask */
}

/* USER CODE BEGIN Header_StartUploadTask */
/**
* @brief Function implementing the UploadTask thread.
* @param argument: Not used
* @retval None
*/
/* USER CODE END Header_StartUploadTask */
void StartUploadTask(void const * argument)
{
  /* USER CODE BEGIN StartUploadTask */
  portTickType currentTimeUpload;
  currentTimeUpload = xTaskGetTickCount();
  /* Infinite loop */
  for(;;)
  {
    APP_UploadData();
    vTaskDelayUntil(&currentTimeUpload, 100/portTICK_RATE_MS);
  }
  /* USER CODE END StartUploadTask */
}

/* USER CODE BEGIN Header_StartSendPoseTask */
/**
* @brief Function implementing the SendPoseTask thread.
* @param argument: Not used
* @retval None
*/
/* USER CODE END Header_StartSendPoseTask */
void StartSendPoseTask(void const * argument)
{
  /* USER CODE BEGIN StartSendPoseTask */
  portTickType currentTimeSendPose;
  currentTimeSendPose = xTaskGetTickCount();
  /* Infinite loop */
  for(;;)
  {
    APP_SendPose();
    vTaskDelayUntil(&currentTimeSendPose, 100/portTICK_RATE_MS);
  }
  /* USER CODE END StartSendPoseTask */
}

/* Private application code --------------------------------------------------*/
/* USER CODE BEGIN Application */

/* USER CODE END Application */
