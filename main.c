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

//	calibrateSensors();

	while(!getButton()) {
		batteryFault();
		toggleLED(WHITE);
	}

	resetLED(WHITE);

	testChaser(1,250);

	resetEncoder(LEFTENCODER);
	resetEncoder(RIGHTENCODER);

	// Test Flash Memory Variables
	// 0x08010000  == 64KB
	// 0x08040000  == 256KB
	// 0x08080000  == 512KB
	// 0x080C0000  == 768KB
	// 0x080E0000  == 896KB
	uint32_t startAddress = 0x080E0000;

	while(1) {

	}
}
