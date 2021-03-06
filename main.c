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
	}

	resetLED(WHITE);

	testChaser(1,250);

	double base_speed = 100;

	setVelocity(0);

	resetEncoder(LEFTENCODER);
	resetEncoder(RIGHTENCODER);

	double right;
	double left;
	int rightEncoder = 0;
	int leftEncoder = 0;
	int startL = getEncoder(LEFTENCODER);
	int startR = getEncoder(RIGHTENCODER);

	while(1) {
		batteryFault();
		moveCells(8,base_speed);
		while(!getButton());
		HAL_Delay(500);
	}
}
