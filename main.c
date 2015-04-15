/* File Name: main.c
 * Project: EE 396
 * Team: TSM
 * Members: Mark Miw
 *          Steven Chen
 *          Joseph Felix Jr.
 */

#include "adc.h"
#include "calibration.h"
#include "correction.h"
#include "defines.h"
#include "encoder.h"
#include "global_libs.h"
#include "HAL_includes.h"
#include "interface.h"
#include "motor.h"
#include "stm32f4xx_it.h"
#include "tracking.h"
#include "usart.h"
#include "system.h"
#include "flash.h"

extern TIM_HandleTypeDef htim2;

void main(void) {
	HAL_Init();
	SystemClock_Config();
	initSystem();

	// Test USART Connection
	printString("Hello world!");
	printNL();

	calibrateSensors();

	while(!getButton()) {
		toggleLED(WHITE);
		HAL_Delay(100);
	}

	printFloat(getWall(CENTERRIGHTFRONT));
	printNL();
	printFloat(getWall(CENTERLEFTFRONT));
	printNL();
	printFloat(getWall(IDEALRIGHTFRONT));
	printNL();
	printFloat(getWall(IDEALLEFTFRONT));
	printNL();
	printFloat(getWall(IDEALRIGHTCENTER));
	printNL();
	printFloat(getWall(IDEALLEFTCENTER));
	printNL();
	printFloat(getWall(FARRIGHTWALL));
	printNL();
	printFloat(getWall(FARLEFTWALL));
	printNL();

	resetLED(WHITE);

	testChaser(1,250);

	resetEncoder(LEFTENCODER);
	resetEncoder(RIGHTENCODER);

	while(1) {

	}
}
