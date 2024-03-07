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
#include "tim.h"
#include "stm32f429i_discovery.h"
#include "stm32f429i_discovery_lcd.h"
#include "stm32f429i_discovery_ts.h"
/* USER CODE END Includes */

/* Private typedef -----------------------------------------------------------*/
/* USER CODE BEGIN PTD */
typedef struct{
	uint16_t in_x, la_x;
	uint16_t in_y, la_y;
	uint8_t state;
	
}SGT;
/* USER CODE END PTD */

/* Private define ------------------------------------------------------------*/
/* USER CODE BEGIN PD */

#define Buttons_count 3
#define Stop_button 0
#define Left_button 1
#define Right_button 2

/* USER CODE END PD */

/* Private macro -------------------------------------------------------------*/
/* USER CODE BEGIN PM */

/* USER CODE END PM */

/* Private variables ---------------------------------------------------------*/
/* USER CODE BEGIN Variables */
uint16_t ADC_Lecture[1000];
uint16_t ADC_data;
volatile uint16_t plot_size;
volatile uint8_t adc_new = 0, scale_new;
TS_StateTypeDef touch;

SGT buttons[Buttons_count];


/* USER CODE END Variables */
osThreadId defaultTaskHandle;
osThreadId ScreenHandle;
osThreadId ADCHandle;
osThreadId DACHandle;
osThreadId InterfaceHandle;

/* Private function prototypes -----------------------------------------------*/
/* USER CODE BEGIN FunctionPrototypes */

/* USER CODE END FunctionPrototypes */

void StartDefaultTask(void const * argument);
void Screen_task(void const * argument);
void ADC_Ctrl(void const * argument);
void DAC_Ctrl(void const * argument);
void ExtInt(void const * argument);

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

  /* definition and creation of Interface */
  osThreadDef(Interface, ExtInt, osPriorityNormal, 0, 128);
  InterfaceHandle = osThreadCreate(osThread(Interface), NULL);

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
	uint8_t plt_int,plt_fra;
	uint16_t k = 0;
	uint16_t prev_y;
	uint16_t plot_slider;
	uint16_t sc_W = 238; // Width of the screen
	uint16_t sc_H = 101; // Height og the screen
	uint16_t qw, step = 100;
	float resize_factor = 0.03;
	float plot_data;
	/* Plot the background */
	BSP_LCD_SelectLayer(LCD_BACKGROUND_LAYER); // Select the background layer
	BSP_LCD_SetTextColor(LCD_COLOR_WHITE); // Color selection
	// Time's section lines
	BSP_LCD_DrawVLine(51,1,100);
	BSP_LCD_DrawVLine(101,1,100);
	BSP_LCD_DrawVLine(151,1,100);
	BSP_LCD_DrawVLine(201,1,100);
	BSP_LCD_SetTextColor(LCD_COLOR_BLACK); // Color selection
	BSP_LCD_FillRect(1,1,sc_W,sc_H); // Clear the backgroud	
	BSP_LCD_SelectLayer(LCD_FOREGROUND_LAYER); // Return the foreground layer
  for(;;)
  {
//		// Plot the data from the ADC
//		for(qw = 1; qw <= Sc_W; qw++){
//			if(step != 1000)
//					step++; // move 1 the data to read
//			else{
//				step = 0; // return to the start
//			}
//			/* Working with primitive graphics */
//			BSP_LCD_SetTextColor(LCD_COLOR_BLACK); // Background color
//			BSP_LCD_DrawVLine(qw, 1,Sc_H); // Clean the data line
//			BSP_LCD_SetTextColor(LCD_COLOR_WHITE); // Put color to the midline
//			BSP_LCD_DrawLine(1, Sc_H / 2, Sc_W, Sc_H / 2); // Draw the midline
//			
//			plot_data = (float)ADC_Lecture[step]; // get the data of the ADC
//			
//			plot_data -= 2047; // remove the offset (1.5)
//			plot_data /= 4095; // divide by the resolution (12 bits)
//			plot_data *= 3; // multiply by the voltage reference
//			plot_data /= resize_factor; // Adjust to the actual zoom
//			if(plot_data > 0){// Voltage is positive
//				BSP_LCD_SetTextColor(LCD_COLOR_LIGHTRED);
//				BSP_LCD_DrawLine(qw, ((Sc_H - 1) / 2 - 1), qw, ((Sc_H - 1) / 2 - 1) - plot_data);// draw a line with the lenght of the data 
//			}else{
//				BSP_LCD_SetTextColor(LCD_COLOR_DARKBLUE);
//				BSP_LCD_DrawVLine(qw, ((Sc_H - 1) / 2 + 1), plot_data * -1);// draw a line with the lenght of the data
//			}
//			
//		}
		if(!buttons[Stop_button].state){
			/* print the reference */
			if(scale_new){
				plt_int = plot_size / 9; // Select the time scale 
				plt_fra = plot_size % 9; // Select the time value
				switch (plt_int){
					case 1:
						BSP_LCD_DisplayStringAt(65,110,(uint8_t *)"seg.", LEFT_MODE);
						break;
					case 2:
						BSP_LCD_DisplayStringAt(65,110,(uint8_t *)"ms.", LEFT_MODE);
						break;
					case 3:
						BSP_LCD_DisplayStringAt(65,110,(uint8_t *)"us.", LEFT_MODE);
						break;
					case 4:
						BSP_LCD_DisplayStringAt(65,110,(uint8_t *)"ns.", LEFT_MODE);
						break;
				}
				switch (plt_fra){
					case 0:
						BSP_LCD_DisplayStringAt(2,110,(uint8_t *)"500", RIGHT_MODE);
						break;
					case 1:
						BSP_LCD_DisplayStringAt(2,110,(uint8_t *)"250", RIGHT_MODE);
						break;
					case 2:
						BSP_LCD_DisplayStringAt(2,110,(uint8_t *)"100", RIGHT_MODE);
						break;
					case 3:
						BSP_LCD_DisplayStringAt(2,110,(uint8_t *)"50", RIGHT_MODE);
						break;
					case 4:
						BSP_LCD_DisplayStringAt(2,110,(uint8_t *)"25", RIGHT_MODE);
						break;
					case 5:
						BSP_LCD_DisplayStringAt(2,110,(uint8_t *)"10", RIGHT_MODE);
						break;
					case 6:
						BSP_LCD_DisplayStringAt(2,110,(uint8_t *)"5", RIGHT_MODE);
						break;
					case 7:
						BSP_LCD_DisplayStringAt(2,110,(uint8_t *)"2", RIGHT_MODE);
						break;
					case 8:
						BSP_LCD_DisplayStringAt(2,110,(uint8_t *)"1", RIGHT_MODE);
						break;
				}
			}
			/* Data plot */
			while(plot_slider <= 238 && adc_new){
				BSP_LCD_SetTextColor(LCD_COLOR_TRANSPARENT); // No color
				BSP_LCD_DrawVLine(qw, 1,sc_H); // Clean the data line
				BSP_LCD_DrawPixel(plot_slider, ADC_Lecture[k], LCD_COLOR_YELLOW);
				k++;
				if (k == 1000)
					k = 0;
				plot_slider ++;
				adc_new --;
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
	//HAL_ADC_Start_DMA(&hadc1,(uint32_t *)ADC_data,1);
	
  for(;;)
  {
		// Convert the data from ADC lecture to a V
//		if(adc_new){
//			
//		}
		
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

/* USER CODE BEGIN Header_ExtInt */
/**
* @brief Function implementing the Interface thread.
* @param argument: Not used
* @retval None
*/
/* USER CODE END Header_ExtInt */
void ExtInt(void const * argument)
{
  /* USER CODE BEGIN ExtInt */
	uint8_t prev_encoder = 0;
	
  /* Infinite loop */
  for(;;)
  {
		/* Touch detection */
		BSP_TS_GetState(&touch);// Get where does the touch is detected
		if(touch.TouchDetected){ // In case that a touch was detected
			// Look for any button touch
			for(uint8_t i = 0; i != Buttons_count; i++){
				if(touch.X >= buttons[i].in_x && touch.X <= buttons[i].la_x){
					if (i == Stop_button) // This is only for the stop button
						buttons[Stop_button].state = ~buttons[Stop_button].state; // Change the state
					else
						buttons[i].state = 1; // if was pressed set the state
				}
			}
		}
		/* Encoder detections */
		// check for encoder button pressures
		if (! HAL_GPIO_ReadPin(Button_encoder_GPIO_Port,Button_encoder_Pin)) // Check for a low state in the encoder button
			// In this case, the enconder button will stop the osciloscope
			buttons[Stop_button].state = ~buttons[Stop_button].state; // Set the stop button
		// check for encoder turns
		if(prev_encoder != HAL_GPIO_ReadPin(A_encoder_GPIO_Port,A_encoder_Pin)){ // if a turn was detected
			prev_encoder = HAL_GPIO_ReadPin(A_encoder_GPIO_Port,A_encoder_Pin);// save the actual state
			if(prev_encoder == HAL_GPIO_ReadPin(B_encoder_GPIO_Port,B_encoder_Pin)) // if equals, then anti clock wise turn
				buttons[Left_button].state = 1; // Set minus time buttos
			else
				buttons[Right_button].state = 1;// Set plus time buttos
		}
		/* Buttons actions */
		for(uint8_t i = 0; i != Buttons_count; i++){
			switch (i){ // selector of each button
				case Left_button:
					if(plot_size > 15){ // If not at the limit
						plot_size --; // Decrease the size
						scale_new = 1;
					}
					break;
				case Right_button:
					if(plot_size < 44){ // If not at the limit
						plot_size ++; // Increase the size
						scale_new = 1;
					}
					break;
				default:
					break;
			}
		}
		
		
		
		
    osDelay(15); // check every 15 ms
  }
  /* USER CODE END ExtInt */
}

/* Private application code --------------------------------------------------*/
/* USER CODE BEGIN Application */

/* USER CODE END Application */
