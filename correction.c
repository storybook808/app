/* File Name: correction.c
 * Project: EE 396
 * Team: TSM
 * Members: Mark Miw
 *          Steven Chen
 *          Joseph Felix Jr.
 */

#include "correction.h"

/* Nice P values
 * PR = 0.01
 * PL = 0.02
 *
 * Good for Speed 100
 * DR = 0.1
 * DL = 0.1
 *
 * Decent for Speed 200
 * PR = 0.0037
 * PL = 0.0077
 * DR = 0.001
 * DL = 0.001
 *
 * Decent
 * .007
 */

extern TIM_HandleTypeDef htim2;

void frontCorrection() {

	double k = 0.3;
	setVelocity(0);
	bool right = false;
	bool left = false;

	double currentFrontRight;
	double currentFrontLeft;

	double errorR;
	double errorL;

	while(!right || !left)
	{
		currentFrontRight = readSensor(RIGHT_DET);
		currentFrontLeft = readSensor(LEFT_DET);

		errorR = currentFrontRight - getWall(CENTERRIGHTFRONT);
		errorL = currentFrontLeft - getWall(CENTERLEFTFRONT);

		// Set Upper & Lower Bound of speed via error
		if (errorR*k > 20) {
			errorR = 20/k;
		}
		if (errorL*k > 20) {
			errorL = 20/k;
		}
		if (errorR*k < -20) {
			errorR = -20/k;
		}
		if (errorL*k < -20) {
			errorL = -20/k;
		}
		if (right) {
			errorR = 0;
		}
		if (left) {
			errorL = 0;
		}

		// Check if we have reached our ideal location
		if (abs(errorR) < CORRECTION_FRONT_THRESH) {
			errorR = 0;
			brakeRight();
			right = true;
		}
		else {
			setRightVelocity(errorR*k);
		}
		if (abs(errorL) < CORRECTION_FRONT_THRESH && !left) {
			errorL = 0;
			brakeLeft();
			left = true;
		}
		else {
			setLeftVelocity(errorL*k);
		}
	}

	HAL_Delay(100);
	HAL_TIM_Base_Stop_IT(&htim2);
	setSpeed(RIGHTMOTOR,0);
	setSpeed(LEFTMOTOR,0);
}

void brakeCorrection(int startR, int startL) {
	double k = 0.03;
	bool right = false;
	bool left = false;

	int currentFrontRight;
	int currentFrontLeft;

	double errorR;
	double errorL;

	while(!right || !left)
	{
		currentFrontRight = getEncoder(RIGHTENCODER);
		currentFrontLeft = getEncoder(LEFTENCODER);

		errorR = startR - currentFrontRight;
		errorL = startL - currentFrontLeft;


		// Set Upper Bound of speed via error
		if (errorR*k > 20) {
			errorR = 20/k;
		}
		if (errorL*k > 20) {
			errorL = 20/k;
		}

		if (abs(errorR) < CORRECTION_FRONT_THRESH) {
			errorR = 0;
			brakeRight();
			right = true;
		}
		else {
			setRightVelocity(errorR*k);
		}
		if (abs(errorL) < CORRECTION_FRONT_THRESH && !left) {
			errorL = 0;
			brakeLeft();
			left = true;
		}
		else {
			setLeftVelocity(errorL*k);
		}
	}
	HAL_Delay(100);
	HAL_TIM_Base_Stop_IT(&htim2);
	setSpeed(LEFTMOTOR,0);
	setSpeed(RIGHTMOTOR,0);
	resetEncoder(RIGHTENCODER);
	resetEncoder(LEFTENCODER);
}

void correction(uint8_t wall, double base_speed) {

	double errorP;
	double errorD;
	double error_total;

	const double kP = .2;
	const double kD = 0;

	double left_side_sensor = readSensor(LEFT_CEN_DET);
	double right_side_sensor = readSensor(RIGHT_CEN_DET);

	HAL_TIM_Base_Start_IT(&htim2);

	if (wall == 0) {
		errorP = getWall(IDEALRIGHTCENTER) - right_side_sensor;
		errorD = errorP - last_errorP;
		error_total = errorP * kP + errorD * kD;

		setLeftVelocity(base_speed - error_total);
		setRightVelocity(base_speed + error_total);
	}
	else if (wall == 1) {
		errorP = getWall(IDEALLEFTCENTER) - left_side_sensor;
		errorD = errorP - last_errorP;
		error_total = errorP * kP + errorD * kD;

		setLeftVelocity(base_speed + error_total);
		setRightVelocity(base_speed - error_total);
	}
	else {
		setVelocity(base_speed);
	}
	last_errorP = errorP;
}

void correction2(double base_speed) {

	double errorLeftP,errorRightP;
	double errorLeftD,errorRightD;
	double error_left_total,error_right_total;

	const double kP = .1;//.5 speed 50
	const double kD = .1;//.06 speed 50

	double left_side_sensor = readSensor(LEFT_CEN_DET);
	double right_side_sensor = readSensor(RIGHT_CEN_DET);

	HAL_TIM_Base_Start_IT(&htim2);

	errorLeftP = getWall(IDEALLEFTCENTER)-10 - left_side_sensor;
	errorRightP = getWall(IDEALRIGHTCENTER)-10 - right_side_sensor;
	errorRightD = errorRightP - last_rightErrorP;
	errorLeftD = errorLeftP - last_leftErrorP;

	error_right_total = errorRightP*kP + errorRightD*kD;
	error_left_total = errorLeftP*kP + errorLeftD*kD;

	if (errorLeftP < 0) {
		error_left_total = 0;
	}
	if (errorRightP < 0) {
		error_right_total = 0;
	}

	setRightVelocity(base_speed - error_left_total);
	setLeftVelocity(base_speed - error_right_total);

	last_leftErrorP = errorLeftP;
	last_rightErrorP = errorRightP;
}
