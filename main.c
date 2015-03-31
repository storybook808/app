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

	double error;

	const double kP = 1;

	double base_speed = 130;

	int start_encoder;

	const int forward_distance = 3500;

	const int left_turn = 1200;
	const int right_turn = 1100;

	uint32_t start_tick;
//	while (1) {
//		batteryFault();
//		setSpeed(LEFTMOTOR, 500);
//		setSpeed(RIGHTMOTOR, 500);
//	}
	setLeftVelocity(0);
	setRightVelocity(0);

	while(1) {
		batteryFault();

		left_front_sensor = readSensor(LEFT_DET);
		right_front_sensor = readSensor(RIGHT_DET);

		left_side_sensor = readSensor(LEFT_CEN_DET);
		right_side_sensor = readSensor(RIGHT_CEN_DET);

		resetLEDAll();

		if (right_side_sensor >= getWall(FARRIGHTWALL)) {
			// Move forward, brake, turn right, and move forward
			setLED(BLUE);
			setSpeed(LEFTMOTOR, base_speed);
			setSpeed(RIGHTMOTOR, base_speed);
			start_encoder = getEncoder(RIGHTENCODER);
			while (getEncoder(RIGHTENCODER) < (start_encoder + forward_distance));
			setSpeed(LEFTMOTOR, 0);
			setSpeed(RIGHTMOTOR, 0);
			HAL_TIM_Base_Start_IT(&htim2);
			HAL_Delay(500);
			HAL_TIM_Base_Stop_IT(&htim2);
			setSpeed(LEFTMOTOR, 200);
			setSpeed(RIGHTMOTOR, -200);
			start_encoder = getEncoder(LEFTENCODER);
			while (getEncoder(LEFTENCODER) < (start_encoder + right_turn));
			setSpeed(LEFTMOTOR, 0);
			setSpeed(RIGHTMOTOR, 0);
			HAL_TIM_Base_Start_IT(&htim2);
			HAL_Delay(500);
			HAL_TIM_Base_Stop_IT(&htim2);
		}


		else {

			if (left_front_sensor <= getWall(IDEALLEFTFRONT) && right_front_sensor <= getWall(IDEALRIGHTFRONT)) {
				// Brake & turn left
				setLED(WHITE);
				setSpeed(LEFTMOTOR, 0);
				setSpeed(RIGHTMOTOR, 0);
				HAL_TIM_Base_Start_IT(&htim2);
				HAL_Delay(500);
				HAL_TIM_Base_Stop_IT(&htim2);

//				start_tick = HAL_GetTick();
//				while(HAL_GetTick() < (start_tick + 1000)) {
//					if (left_front_sensor > getWall(IDEALLEFTFRONT)) {
//						setSpeed(LEFTMOTOR, base_speed);
//					}
//
//					else if (left_front_sensor < getWall(IDEALLEFTFRONT)) {
//						setSpeed(LEFTMOTOR, -base_speed);
//					}
//
//					else {
//						setSpeed(LEFTMOTOR, 0);
//					}
//
//					if (right_front_sensor > getWall(IDEALRIGHTFRONT)) {
//						setSpeed(RIGHTMOTOR, base_speed);
//					}
//
//					else if (right_front_sensor < getWall(IDEALRIGHTFRONT)) {
//						setSpeed(RIGHTMOTOR, -base_speed);
//					}
//
//					else {
//						setSpeed(RIGHTMOTOR, 0);
//					}
//				}
//				resetLED(RED);

				setLED(RED);
				frontCorrection();
				resetLED(RED);


				setSpeed(LEFTMOTOR, -200);
				setSpeed(RIGHTMOTOR, 200);
				start_encoder = getEncoder(RIGHTENCODER);
				while (getEncoder(RIGHTENCODER) < (start_encoder + left_turn));
				setSpeed(LEFTMOTOR, 0);
				setSpeed(RIGHTMOTOR, 0);
				HAL_TIM_Base_Start_IT(&htim2);
				HAL_Delay(500);
				HAL_TIM_Base_Stop_IT(&htim2);
			}

			else {
				// Move forward correcting off the right wall
				error = getWall(IDEALRIGHTCENTER) - right_side_sensor;
				setSpeed(LEFTMOTOR, (int)(base_speed - (error * kP)));
				setSpeed(RIGHTMOTOR, (int)(base_speed + (error * kP)));

			}
		}



	}
}
