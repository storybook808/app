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

	// Start button.
	while(!getButton()) {
		toggleLED(WHITE);
		HAL_Delay(100);
	}

	resetLED(WHITE);

	// Start-up light sequence.
	testChaser(1,250);

	int temp = 0;

	// Base speed for this run.
	double base_speed = 130;

	// Initialize motors.
	setSpeed(LEFTMOTOR, 0);
	setSpeed(RIGHTMOTOR, 0);
	HAL_TIM_Base_Stop_IT(&htim2);

	// Initialize encoders.
	resetEncoder(LEFTENCODER);
	resetEncoder(RIGHTENCODER);

	// Wall tracking flags.
	bool is_left_wall  = true;
	bool is_right_wall = true;

	// Sensor information variables.
	double left_front_sensor, right_front_sensor;
	double left_side_sensor, right_side_sensor;

	// Controller variables.
	const double kP = 1.0;
//	double error;
	double left_error, right_error;

	// Initial sensor readings
	left_front_sensor  = readSensor(LEFT_DET);
	right_front_sensor = readSensor(RIGHT_DET);
	left_side_sensor   = readSensor(LEFT_CEN_DET);
	right_side_sensor  = readSensor(RIGHT_CEN_DET);

	while(1) {
		// Check the condition of the battery.
		// Faults if battery is @ a lower voltage level.
		batteryFault();

		// Take new sensor readings.
		left_front_sensor  = readSensor(LEFT_DET);
		right_front_sensor = readSensor(RIGHT_DET);
		left_side_sensor   = readSensor(LEFT_CEN_DET);
		right_side_sensor  = readSensor(RIGHT_CEN_DET);

		// Detect front wall stop.
		if (right_front_sensor <= getWall(IDEALRIGHTFRONT) && left_front_sensor <= getWall(IDEALLEFTFRONT)) {
			hardBrake();
			frontCorrection();
			while(!getButton()) {
				batteryFault();
				toggleLED(RED);
				HAL_Delay(100);
			}
			resetLED(RED);
			HAL_Delay(500);
		}

		// Update wall states.
		if (left_side_sensor <= getWall(FARLEFTWALL)) {
			is_left_wall = true;
		}

		else {
			is_left_wall = false;
		}

		if (right_side_sensor <= getWall(FARRIGHTWALL)) {
			is_right_wall = true;
		}

		else {
			is_right_wall = false;
		}

		// Output the state via LED.
		if (is_left_wall) setLED(WHITE);
		else resetLED(WHITE);

		if (is_right_wall) setLED(BLUE);
		else resetLED(BLUE);

//		// Both walls are present.
//		if (is_left_wall && is_right_wall) {
//			// Right wall tracking.
//			error = getWall(IDEALRIGHTCENTER) - right_side_sensor;
//			setSpeed(LEFTMOTOR, (int)(base_speed - (error * kP)));
//			setSpeed(RIGHTMOTOR, (int)(base_speed + (error * kP)));
//		}
//
//		// Only the right wall is present.
//		else if (!is_left_wall && is_right_wall) {
//			// Right wall tracking.
//			error = getWall(IDEALRIGHTCENTER) - right_side_sensor;
//			setSpeed(LEFTMOTOR, (int)(base_speed - (error * kP)));
//			setSpeed(RIGHTMOTOR, (int)(base_speed + (error * kP)));
//		}
//
//		// Only the left wall is present.
//		else if (is_left_wall && !is_right_wall) {
//			// Left wall tracking.
//			error = getWall(IDEALLEFTCENTER) - left_side_sensor;
//			setSpeed(LEFTMOTOR, (int)(base_speed + (error * kP)));
//			setSpeed(RIGHTMOTOR, (int)(base_speed) - (error * kP));
//		}
//
//		// No walls are present.
//		else {
//			// No wall tracking.
//			setSpeed(LEFTMOTOR, base_speed);
//			setSpeed(RIGHTMOTOR, base_speed);
//		}

		left_error  = getWall(IDEALLEFTCENTER) - left_side_sensor;
		right_error = getWall(IDEALRIGHTCENTER) - right_side_sensor;

		if (left_error < 0) left_error = 0;
		if (right_error < 0) right_error = 0;

		setSpeed(LEFTMOTOR, base_speed - right_error * kP);
		setSpeed(RIGHTMOTOR, base_speed - left_error * kP);

	}
}
