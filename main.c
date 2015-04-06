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
	bool is_left_wall        = true;
	bool is_right_wall       = true;
	bool is_left_wall_track  = true;
	bool is_right_wall_track = true;

	// Wall tracking variables.
	const double track_threshold = 30.0;

//	int correction_state = 0;

	// Sensor information variables.
	double left_front_sensor, right_front_sensor;
	double left_side_sensor, right_side_sensor;

	// Controller variables.
	const double kP    = 1.0;
	const double kD    = 0.0;
	double errorP      = 0;
	double last_errorP = 0;
	double errorD      = 0;
	double error_total = 0;

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

		// Update tracking states.
		if (left_side_sensor <= getWall(IDEALLEFTCENTER) + track_threshold) {
			is_left_wall_track = true;
		}

		else {
			is_left_wall_track = false;
		}

		if (right_side_sensor <= getWall(IDEALRIGHTCENTER) + track_threshold) {
			is_right_wall_track = true;
		}

		else {
			is_right_wall_track = false;
		}

		// Output the state via LED.
		if (is_left_wall) setLED(WHITE);
		else resetLED(WHITE);

		if (is_right_wall) setLED(BLUE);
		else resetLED(BLUE);

		// Both walls are present.
		if (is_left_wall_track && is_right_wall_track) {
			// Right wall tracking.
//			correction_state = 0;

			last_errorP = errorP;
			errorP = getWall(IDEALRIGHTCENTER) - right_side_sensor;
			errorD = errorP - last_errorP;
			error_total = errorP * kP + errorD * kD;
			setSpeed(LEFTMOTOR, (int)(base_speed - error_total));
			setSpeed(RIGHTMOTOR, (int)(base_speed + error_total));
		}

		// Only the right wall is present.
		else if (!is_left_wall_track && is_right_wall_track) {
			// Right wall tracking.
//			correction_state = 0;

			last_errorP = errorP;
			errorP = getWall(IDEALRIGHTCENTER) - right_side_sensor;
			errorD = errorP - last_errorP;
			error_total = errorP * kP + errorD * kD;
			setSpeed(LEFTMOTOR, (int)(base_speed - error_total));
			setSpeed(RIGHTMOTOR, (int)(base_speed + error_total));
		}

		// Only the left wall is present.
		else if (is_left_wall_track && !is_right_wall_track) {
			// Left wall tracking.
//			correction_state = 1;

			last_errorP = errorP;
			errorP = getWall(IDEALLEFTCENTER) - right_side_sensor;
			errorD = errorP - last_errorP;
			error_total = errorP * kP + errorD * kD;
			setSpeed(LEFTMOTOR, (int)(base_speed + error_total));
			setSpeed(RIGHTMOTOR, (int)(base_speed) - error_total);
		}

		// No walls are present.
		else {
			// No wall tracking.
//			correction_state = 2;

			setSpeed(LEFTMOTOR, base_speed);
			setSpeed(RIGHTMOTOR, base_speed);
		}
	}
}
