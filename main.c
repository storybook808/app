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

	double base_speed = 50;

	resetEncoder(LEFTENCODER);
	resetEncoder(RIGHTENCODER);

	double left_front_sensor, right_front_sensor;
	double left_side_sensor, right_side_sensor;

	bool is_left_wall  = true;
	bool is_right_wall = true;
	bool detect = false;
	int wall_start = 0;
	int startR = getEncoder(RIGHTENCODER);
	int i;
	int gyro;
	while(1) {
		batteryFault();
		gyro = readADC(GYRO);
		printInt(gyro);
		printNL();
	}

// Right wall follower
//	while(1) {
//		batteryFault();
//		left_front_sensor  = readSensor(LEFT_DET);
//		right_front_sensor = readSensor(RIGHT_DET);
//		left_side_sensor   = readSensor(LEFT_CEN_DET);
//		right_side_sensor  = readSensor(RIGHT_CEN_DET);
//
//		// Detect front wall stop.
//		if (right_front_sensor <= getWall(IDEALRIGHTFRONT) && left_front_sensor <= getWall(IDEALLEFTFRONT)) {
//			hardBrake();
//			frontCorrection();
//			turnLeft();
//			HAL_Delay(500);
//		}
//		// Update wall states.
//		if (left_side_sensor <= getWall(FARLEFTWALL)) {
//			is_left_wall = true;
//		}
//		else {
//			is_left_wall = false;
//		}
//		if (right_side_sensor <= getWall(FARRIGHTWALL)) {
//			is_right_wall = true;
//		}
//		else {
//			is_right_wall = false;
//		}
//		// Output the state via LED.
//		if (is_left_wall) setLED(WHITE);
//		else resetLED(WHITE);
//
//		if (is_right_wall) setLED(BLUE);
//		else resetLED(BLUE);
//
//		if (!is_right_wall && !detect) {
//			detect = true;
//			wall_start = getEncoder(RIGHTENCODER);
//		}
//		if (detect) {
//			if (getEncoder(RIGHTENCODER) >= wall_start + CELL_R-300) {
//				detect = false;
//				brakeCorrection();
//				turnRight();
//			}
//		}
//
//		correction2(50);
//	}
}
