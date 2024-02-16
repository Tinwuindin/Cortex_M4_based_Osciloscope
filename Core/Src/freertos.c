/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * File Name          : freertos.c
  * Description        : Code for freertos applications
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

/* Includes ------------------------------------------------------------------*/
#include "FreeRTOS.h"
#include "task.h"
#include "main.h"
#include "cmsis_os.h"

/* Private includes ----------------------------------------------------------*/
/* USER CODE BEGIN Includes */
#include "adc.h"
#include "stm32f429i_discovery.h"
#include "stm32f429i_discovery_lcd.h"
#include "stm32f429i_discovery_ts.h"
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
uint16_t ADC_Lecture[1000];
/* USER CODE END Variables */
osThreadId defaultTaskHandle;
osThreadId ScreenHandle;
osThreadId ADCHandle;
osThreadId DACHandle;
osMessageQId ADC_dataHandle;

/* Private function prototypes -----------------------------------------------*/
/* USER CODE BEGIN FunctionPrototypes */

/* USER CODE END FunctionPrototypes */

void StartDefaultTask(void const * argument);
void Screen_task(void const * argument);
void ADC_Ctrl(void const * argument);
void DAC_Ctrl(void const * argument);

void MX_FREERTOS_Init(void); /* (MISRA C 2004 rule 8.1) */

/* GetIdleTaskMemory prototype (linked to static allocation support) */
void vApplicationGetIdleTaskMemory( StaticTask_t **ppxIdleTaskTCBBuffer, StackType_t **ppxIdleTaskStackBuffer, uint32_t *pulIdleTaskStackSize );

/* Hook prototypes */
void vApplicationIdleHook(void);
void vApplicationStackOverflowHook(xTaskHandle xTask, signed char *pcTaskName);
void vApplicationMallocFailedHook(void);

/* USER CODE BEGIN 2 */
__weak void vApplicationIdleHook( void )
{
   /* vApplicationIdleHook() will only be called if configUSE_IDLE_HOOK is set
   to 1 in FreeRTOSConfig.h. It will be called on each iteration of the idle
   task. It is essential that code added to this hook function never attempts
   to block in any way (for example, call xQueueReceive() with a block time
   specified, or call vTaskDelay()). If the application makes use of the
   vTaskDelete() API function (as this demo application does) then it is also
   important that vApplicationIdleHook() is permitted to return to its calling
   function, because it is the responsibility of the idle task to clean up
   memory allocated by the kernel to any task that has since been deleted. */
}
/* USER CODE END 2 */

/* USER CODE BEGIN 4 */
__weak void vApplicationStackOverflowHook(xTaskHandle xTask, signed char *pcTaskName)
{
   /* Run time stack overflow checking is performed if
   configCHECK_FOR_STACK_OVERFLOW is defined to 1 or 2. This hook function is
   called if a stack overflow is detected. */
}
/* USER CODE END 4 */

/* USER CODE BEGIN 5 */
__weak void vApplicationMallocFailedHook(void)
{
   /* vApplicationMallocFailedHook() will only be called if
   configUSE_MALLOC_FAILED_HOOK is set to 1 in FreeRTOSConfig.h. It is a hook
   function that will get called if a call to pvPortMalloc() fails.
   pvPortMalloc() is called internally by the kernel whenever a task, queue,
   timer or semaphore is created. It is also called by various parts of the
   demo application. If heap_1.c or heap_2.c are used, then the size of the
   heap available to pvPortMalloc() is defined by configTOTAL_HEAP_SIZE in
   FreeRTOSConfig.h, and the xPortGetFreeHeapSize() API function can be used
   to query the size of free heap space that remains (although it does not
   provide information on how the remaining heap might be fragmented). */
}
/* USER CODE END 5 */

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

  /* Create the queue(s) */
  /* definition and creation of ADC_data */
  osMessageQDef(ADC_data, 100, uint16_t);
  ADC_dataHandle = osMessageCreate(osMessageQ(ADC_data), NULL);

  /* USER CODE BEGIN RTOS_QUEUES */
  /* add queues, ... */
  /* USER CODE END RTOS_QUEUES */

  /* Create the thread(s) */
  /* definition and creation of defaultTask */
  osThreadDef(defaultTask, StartDefaultTask, osPriorityIdle, 0, 4096);
  defaultTaskHandle = osThreadCreate(osThread(defaultTask), NULL);

  /* definition and creation of Screen */
  osThreadDef(Screen, Screen_task, osPriorityNormal, 0, 256);
  ScreenHandle = osThreadCreate(osThread(Screen), NULL);

  /* definition and creation of ADC */
  osThreadDef(ADC, ADC_Ctrl, osPriorityAboveNormal, 0, 128);
  ADCHandle = osThreadCreate(osThread(ADC), NULL);

  /* definition and creation of DAC */
  osThreadDef(DAC, DAC_Ctrl, osPriorityNormal, 0, 128);
  DACHandle = osThreadCreate(osThread(DAC), NULL);

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

/* USER CODE BEGIN Header_Screen_task */
/**
* @brief Function implementing the Screen thread.
* @param argument: Not used
* @retval None
*/
/* USER CODE END Header_Screen_task */
void Screen_task(void const * argument)
{
  /* USER CODE BEGIN Screen_task */
  /* Infinite loop */
	
	uint16_t Sc_W = 238; // Width of the screen
	uint16_t Sc_H = 101; // Height og the screen
	uint16_t qw, step = 100;
	float resize_factor = 0.03;
	float plot_data;
	// Background of where r the screen
			BSP_LCD_SetTextColor(LCD_COLOR_BLACK); // Put black the background
			BSP_LCD_FillRect(1,1,Sc_W,Sc_H); // Clear the backgroud	
  for(;;)
  {
		// Plot the data from the ADC
		for(qw = 1; qw <= Sc_W; qw++){
			if(step != 1000)
					step++; // move 1 the data to read
			else{
				step = 0; // return to the start
			}
			/* Working with primitive graphics */
			BSP_LCD_SetTextColor(LCD_COLOR_BLACK); // Background color
			BSP_LCD_DrawVLine(qw, 1,Sc_H); // Clean the data line
			BSP_LCD_SetTextColor(LCD_COLOR_WHITE); // Put color to the midline
			BSP_LCD_DrawLine(1, Sc_H / 2, Sc_W, Sc_H / 2); // Draw the midline
			
			plot_data = (float)ADC_Lecture[step]; // get the data of the ADC
			
			plot_data -= 2047; // remove the offset (1.5)
			plot_data /= 4095; // divide by the resolution (12 bits)
			plot_data *= 3; // multiply by the voltage reference
			plot_data /= resize_factor; // Adjust to the actual zoom
			if(plot_data > 0){// Voltage is positive
				BSP_LCD_SetTextColor(LCD_COLOR_LIGHTRED);
				BSP_LCD_DrawLine(qw, ((Sc_H - 1) / 2 - 1), qw, ((Sc_H - 1) / 2 - 1) - plot_data);// draw a line with the lenght of the data 
			}else{
				BSP_LCD_SetTextColor(LCD_COLOR_DARKBLUE);
				BSP_LCD_DrawVLine(qw, ((Sc_H - 1) / 2 + 1), plot_data * -1);// draw a line with the lenght of the data
			}
			
		}
    osDelay(1);
  }
  /* USER CODE END Screen_task */
}

/* USER CODE BEGIN Header_ADC_Ctrl */
/**
* @brief Function implementing the ADC thread.
* @param argument: Not used
* @retval None
*/
/* USER CODE END Header_ADC_Ctrl */
void ADC_Ctrl(void const * argument)
{
  /* USER CODE BEGIN ADC_Ctrl */
  /* Infinite loop */
	HAL_ADC_Start_DMA(&hadc1,(uint32_t *)ADC_Lecture,1000); // Get 100 lectures every 1ms (it should give an 100kHz freq of sampling)
	
  for(;;)
  {
		//if(HAL_ADC_GetState(&hadc3) == HAL_ADC_STATE_REG_EOC){ // If the ADC isn't ready wait to start the lectures
			
		//}
		//xQueueSend(ADC_dataHandle, &ADC_Lecture,((TickType_t) 100)); // Send the data
		
    osDelay(1);
  }
  /* USER CODE END ADC_Ctrl */
}

/* USER CODE BEGIN Header_DAC_Ctrl */
/**
* @brief Function implementing the DAC thread.
* @param argument: Not used
* @retval None
*/
/* USER CODE END Header_DAC_Ctrl */
void DAC_Ctrl(void const * argument)
{
  /* USER CODE BEGIN DAC_Ctrl */
  /* Infinite loop */
  for(;;)
  {
    osDelay(1);
  }
  /* USER CODE END DAC_Ctrl */
}

/* Private application code --------------------------------------------------*/
/* USER CODE BEGIN Application */

/* USER CODE END Application */
