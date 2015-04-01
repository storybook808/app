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

	calibrateSensors();

	// Test USART Connection
	printString("Hello world!");
	printNL();

	while(!getButton()) {
		toggleLED(WHITE);
	}

	resetLED(WHITE);

	testChaser(1,250);

	double left_front_sensor, right_front_sensor;
	double left_side_sensor, right_side_sensor;

	double base_speed = 50;

	setVelocity(0);

	int x;
	int startL, startR;
	resetEncoder(LEFTENCODER);
	resetEncoder(RIGHTENCODER);
	while(1) {
		batteryFault();


//		left_side_sensor = readSensor(LEFT_CEN_DET);
//		right_side_sensor = readSensor(RIGHT_CEN_DET);

		for (x = 0; x < 14; x++) {

			startL = getEncoder(LEFTENCODER);
			startR = getEncoder(RIGHTENCODER);
			resetLEDAll();

			while(1) {
				left_front_sensor = readSensor(LEFT_DET);
				right_front_sensor = readSensor(RIGHT_DET);

				if (right_front_sensor <= getWall(IDEALRIGHTFRONT) && left_front_sensor <= getWall(IDEALLEFTFRONT)) {
					setLED(WHITE);
					brake();
					break;
				}

				correction(0, base_speed);

//				if (getEncoder(LEFTENCODER) > startL + 4100) {
//					brake();
//					break;
//				}

				if (getEncoder(RIGHTENCODER) > startR + 4200) {
					brake();
					break;
				}

//				if (flagL && flagR) {
//					setLED(RED);
//					brake();
//					break;
//				}
			}
		}
		while(!getButton());
		HAL_Delay(500);
	}
}
