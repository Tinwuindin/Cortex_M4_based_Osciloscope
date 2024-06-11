/*
 * Func.h
 *
 *  Created on: Apr 4, 2024
 *      Author: Pollo
 */

#ifndef FUNC_H_
#define FUNC_H_


#endif /* FUNC_H_ */

#define Time_section 80

#include "main.h"

extern int speed, adc_datas;
extern uint16_t time_to_start, extra_time;
extern uint8_t condicion, espera;
extern float adjuster;
extern float buffer[20];

void ADC_ticker(){
	// Change the state of condicion
	if (espera){
	condicion = 0;
	// Adjust the timer for read an data every tick
	adjuster = SystemCoreClock ; // Get the MCU Freq
	switch (speed){
		case 0:
			// 1s per segment
			break;
		case 1:
			adjuster /= 2; // 500ms per segment
			break;
		case 2:
			adjuster /= 4; // 250ms per segment
			break;
		case 3:
			adjuster /= 10; // 100ms per segment
			break;
		case 4:
			adjuster /= 20; // 50ms per segment
			break;
		case 5:
			adjuster /= 40; // 25ms per segment
			break;
		case 6:
			adjuster /= 100; // 1ms per segment
			break;
		case 7:
			adjuster /= 200; // 500us per segment
			break;
		case 8:
			adjuster /= 400; // 250us per segment
			break;
		case 9:
			adjuster /= 1000; // 100us per segment
			break;
	}

	adjuster /= Time_section; //80 samples per time section
	adjuster /= 65535; // Divide by the timer size to get how many times the timer have to pass
	time_to_start = (uint16_t)(adjuster); // get the complete times
	adjuster -= time_to_start; // get the decimals
	extra_time = adjuster * 65535; // get the complement for the timer


	// Save the data into the buffer
	buffer[adc_datas] = (ADC1 -> DR | 0x0000FFFF); // Keeping only the first 16 bits
	buffer[adc_datas] = buffer[adc_datas] * 3.3 / 4095; // Convert to voltage
	adc_datas ++; // step into the data count
	// Launch the ADC conversion
	ADC1 -> CR2 |=  ADC_CR2_ADON;
	// Start the timer
	TIM7 -> CR1 |= 1;
	}

}
