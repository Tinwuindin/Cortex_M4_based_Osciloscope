/*
 * Func.cpp
 *
 *  Created on: Apr 4, 2024
 *      Author: Pollo
 */


#include "Func.h"
#include "main.h"
#include "stm32f4xx_hal.h"


int speed = 2, adc_datas = 1;
uint16_t time_to_start, extra_time;
uint8_t condicion = 1, espera = 0;
float adjuster;
float buffer[20];


/*
void Tick_count(void){
	if(time_to_start){
		time_to_start --; // Minus 1
		TIM7 -> CNT = 0; // clean the timer
		HAL_TIM_Base_Start(&htim7); // start the timer
	}else{
		// Pre reload the timer with the complement
		TIM7 -> CNT = extra_time;
	}
}
*/

