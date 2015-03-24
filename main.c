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

	double base_speed = 50;

	int start_encoder;

	const int forward_distance = 3500;

	const int left_turn = 1200;
	const int right_turn = 1100;

//	setVelocity(0);
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
//			setVelocity(base_speed);
			setSpeed(LEFTMOTOR, base_speed);
			setSpeed(RIGHTMOTOR, base_speed);
			start_encoder = getEncoder(RIGHTENCODER);
			while (getEncoder(RIGHTENCODER) < (start_encoder + forward_distance));
			setSpeed(LEFTMOTOR, 0);
			setSpeed(RIGHTMOTOR, 0);
			HAL_TIM_Base_Start_IT(&htim2);
//			brakeLeft();
//			brakeRight();
			HAL_Delay(500);
			HAL_TIM_Base_Stop_IT(&htim2);
			setSpeed(LEFTMOTOR, 200);
			setSpeed(RIGHTMOTOR, -200);
//			setLeftVelocity(base_speed);
//			setRightVelocity(-base_speed);
			start_encoder = getEncoder(LEFTENCODER);
			while (getEncoder(LEFTENCODER) < (start_encoder + right_turn));
			setSpeed(LEFTMOTOR, 0);
			setSpeed(RIGHTMOTOR, 0);
			HAL_TIM_Base_Start_IT(&htim2);
//			brakeLeft();
//			brakeRight();
			HAL_Delay(500);
			HAL_TIM_Base_Stop_IT(&htim2);
//			while(1) {
//				batteryFault();
//				if (getButton()) break;
//			}
		}


		else {

			if (left_front_sensor <= getWall(IDEALLEFTFRONT) && right_front_sensor <= getWall(IDEALRIGHTFRONT)) {
				// Brake & turn left
				setLED(WHITE);
//				brakeLeft();
//				brakeRight();
				setSpeed(LEFTMOTOR, 0);
				setSpeed(RIGHTMOTOR, 0);
				HAL_TIM_Base_Start_IT(&htim2);
				HAL_Delay(500);
				HAL_TIM_Base_Stop_IT(&htim2);
				setSpeed(LEFTMOTOR, -200);
				setSpeed(RIGHTMOTOR, 200);
//				setLeftVelocity(-base_speed);
//				setRightVelocity(base_speed);
				start_encoder = getEncoder(RIGHTENCODER);
				while (getEncoder(RIGHTENCODER) < (start_encoder + left_turn));
//				brakeLeft();
//				brakeRight();
				setSpeed(LEFTMOTOR, 0);
				setSpeed(RIGHTMOTOR, 0);
				HAL_TIM_Base_Start_IT(&htim2);
				HAL_Delay(500);
				HAL_TIM_Base_Stop_IT(&htim2);
//				while(1) {
//					batteryFault();
//					if (getButton()) break;
//				}
			}

			else {
				// Move forward correcting off the right wall
				error = getWall(IDEALRIGHTCENTER) - right_side_sensor;

				setSpeed(LEFTMOTOR, (int)(base_speed - (error * kP)));
				setSpeed(RIGHTMOTOR, (int)(base_speed + (error * kP)));
//				setLeftVelocity(base_speed - (error * kP));
//				setRightVelocity(base_speed + (error * kP));

			}
		}



	}
}
