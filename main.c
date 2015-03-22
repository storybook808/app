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


//	while(HAL_GPIO_ReadPin(GPIOC, GPIO_PIN_13) != GPIO_PIN_SET);

	//LED start up sequence
	testChaser(1, 250);

	calibrateSensors();

	// Test USART Connection
	printString("Hello world!");
	printNL();

	double left_front_sensor, right_front_sensor;
	double left_side_sensor, right_side_sensor;

	while(1) {
		batteryFault();

		left_front_sensor = readSensor(LEFT_DET);
		right_front_sensor = readSensor(RIGHT_DET);

		left_side_sensor = readSensor(LEFT_CEN_DET);
		right_side_sensor = readADC(RIGHT_CEN_DET);

		if (left_front_sensor <= getWall(IDEALLEFTFRONT)) {
			setLED(WHITE);
		}

		else {
			resetLED(WHITE);
		}

		if (right_front_sensor <= getWall(IDEALRIGHTFRONT)) {
			setLED(BLUE);
		}

		else {
			resetLED(BLUE);
		}

		if (left_side_sensor <= getWall(IDEALLEFTCENTER)) {
			setLED(GREEN);
		}

		else {
			resetLED(GREEN);
		}

		if (right_side_sensor <= getWall (IDEALRIGHTCENTER)) {
			setLED(RED);
		}

		else {
			resetLED(RED);
		}
	}
}
