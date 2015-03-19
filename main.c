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


void main(void) {
	HAL_Init();
	SystemClock_Config();
	initSystem();
	calibrateWall(FRONT);

	//LED start up sequence
	testChaser(1, 250);

	 // Test USART Connection
	 printString("Hello world!");
	 printNL();
	 setVelocity(50);
	 HAL_Delay(200);
	 brake();

	while(1) {
		batteryFault();

	}
}
