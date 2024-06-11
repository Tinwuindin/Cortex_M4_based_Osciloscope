#include <gui/osc_screen/OscView.hpp>
#include "stm32f4xx_hal.h"
/*
extern int adc_datas;
extern float buffer[20];
extern uint8_t espera;
*/
//extern uint16_t adc_values[320];
extern uint32_t cantidad;
float valor;
int pasos = 0;
extern ADC_HandleTypeDef hadc1;
extern TIM_HandleTypeDef htim7;

OscView::OscView()
{
	//tickCounter = 0;
	//adc_datas = 0;
	//espera = 1;
	HAL_ADC_Start(&hadc1);
	HAL_TIM_Base_Start_IT(&htim7);
}

void OscView::handleTickEvent()
{
	while(pasos){
		dynamicGraph1.addDataPoint(valor); // add to the graph
		pasos --;
	}
	if(pasos == 0){
		HAL_TIM_Base_Start_IT(&htim7);
	}
}

void OscView::setupScreen()
{
    OscViewBase::setupScreen();
}

void OscView::tearDownScreen()
{
	//espera = 0;
	HAL_TIM_Base_Stop_IT(&htim7);
    OscViewBase::tearDownScreen();
}
