/* File Name: tracking.c
 * Project: EE 396
 * Team: TSM
 * Members: Mark Miw
 *          Steven Chen
 *          Joseph Felix Jr.
 */

#include "tracking.h"


bool hasRightWall(double value) {
	if (value < getWall(FARRIGHTWALL)) return true;
	else return false;
}

bool hasLeftWall(double value) {
	if (value < getWall(FARLEFTWALL)) return true;
	else return false;
}

bool hasFrontWall(double valueRight, double valueLeft) {
	if ((valueRight < getWall(IDEALRIGHTFRONT)) && (valueLeft < getWall(IDEALLEFTFRONT))) return true;
	else return false;
}

void moveForward(int cells, Wall_Correction wall, double base_speed) {
	int x;
	for (x = 0; x < cells; x++) {

		int startL = getEncoder(LEFTENCODER);
		double left_front_sensor;
		double right_front_sensor;
		resetLEDAll();

		while(1) {
			left_front_sensor = readSensor(LEFT_DET);
			right_front_sensor = readSensor(RIGHT_DET);

			if (right_front_sensor <= getWall(IDEALRIGHTFRONT) && left_front_sensor <= getWall(IDEALLEFTFRONT)) {
				hardBrake();
				frontCorrection();
				break;
			}

			correction(wall, base_speed);

			if (getEncoder(LEFTENCODER) > startL + CELL_L) {
				brake();
				break;
			}
		}
	}
}

void turnRight() {

	double k = 0.03;
	setVelocity(0);
	bool right = false;
	bool left = false;

	int currentFrontRight;
	int currentFrontLeft;

	int endR = getEncoder(RIGHTENCODER) - TURN_R;
	int endL = getEncoder(LEFTENCODER) + TURN_L;

	double errorR;
	double errorL;

	while(!right || !left)
	{
		currentFrontRight = getEncoder(RIGHTENCODER);
		currentFrontLeft = getEncoder(LEFTENCODER);

		errorR = endR - currentFrontRight;
		errorL = endL - currentFrontLeft;

		if (abs(errorR) < CORRECTION_FRONT_THRESH) {
			errorR = 0;
			brakeRight();
			right = true;
		}
		else {
			setRightVelocity(errorR*k);
		}
		if (abs(errorL) < CORRECTION_FRONT_THRESH) {
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

	resetEncoder(RIGHTENCODER);
	resetEncoder(LEFTENCODER);
	last_leftErrorP = 0;
	last_rightErrorP = 0;
	setLeftVelocity(0);
	setRightVelocity(0);
}

void turnRight2() {

	double k = 0.03;
	setVelocity(0);
	bool right = false;

	int currentFrontRight;

	int endR = getEncoder(RIGHTENCODER) - 2*TURN_R;

	double errorR;

	while(!right)
	{
		brakeLeft();
		currentFrontRight = getEncoder(RIGHTENCODER);

		errorR = endR - currentFrontRight;

		if (abs(errorR) < CORRECTION_FRONT_THRESH) {
			brakeRight();
			right = true;
		}
		else {
			setRightVelocity(errorR*k);
		}
	}

	HAL_Delay(100);
	HAL_TIM_Base_Stop_IT(&htim2);
	setSpeed(RIGHTMOTOR,0);
	setSpeed(LEFTMOTOR,0);

	resetEncoder(RIGHTENCODER);
	resetEncoder(LEFTENCODER);
	last_leftErrorP = 0;
	last_rightErrorP = 0;
	setLeftVelocity(0);
	setRightVelocity(0);
}

void turnRight3() {

	double k = 0.03;
	setVelocity(0);
	bool left = false;

	int currentFrontLeft;

	int endL = getEncoder(LEFTENCODER) + 2*TURN_L;

	double errorL;

	while(!left)
	{
		brakeRight();
		currentFrontLeft = getEncoder(LEFTENCODER);

		errorL = endL - currentFrontLeft;

		if (abs(errorL) < CORRECTION_FRONT_THRESH) {
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

	resetEncoder(RIGHTENCODER);
	resetEncoder(LEFTENCODER);
	last_leftErrorP = 0;
	last_rightErrorP = 0;
	setLeftVelocity(0);
	setRightVelocity(0);
}

void turnLeft() {

	double k = 0.03;
	setVelocity(0);
	bool right = false;
	bool left = false;

	int currentFrontRight;
	int currentFrontLeft;

	int endR = getEncoder(RIGHTENCODER) + TURN_R;
	int endL = getEncoder(LEFTENCODER) - TURN_L;

	double errorR;
	double errorL;

	while(!right || !left)
	{
		currentFrontRight = getEncoder(RIGHTENCODER);
		currentFrontLeft = getEncoder(LEFTENCODER);

		errorR = endR - currentFrontRight;
		errorL = endL - currentFrontLeft;

		if (abs(errorR) < CORRECTION_FRONT_THRESH) {
			errorR = 0;
			brakeRight();
			right = true;
		}
		else {
			setRightVelocity(errorR*k);
		}
		if (abs(errorL) < CORRECTION_FRONT_THRESH) {
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

	resetEncoder(RIGHTENCODER);
	resetEncoder(LEFTENCODER);
	last_leftErrorP = 0;
	last_rightErrorP = 0;
	setLeftVelocity(0);
	setRightVelocity(0);
}

void turnLeft2() {

	double k = 0.03;
	setVelocity(0);
	bool right = false;

	int currentFrontRight;

	int endR = getEncoder(RIGHTENCODER) + 2*TURN_R;

	double errorR;

	while(!right)
	{
		brakeLeft();
		currentFrontRight = getEncoder(RIGHTENCODER);

		errorR = endR - currentFrontRight;

		if (abs(errorR) < CORRECTION_FRONT_THRESH) {
			brakeRight();
			right = true;
		}
		else {
			setRightVelocity(errorR*k);
		}
	}

	HAL_Delay(100);
	HAL_TIM_Base_Stop_IT(&htim2);
	setSpeed(RIGHTMOTOR,0);
	setSpeed(LEFTMOTOR,0);

	resetEncoder(RIGHTENCODER);
	resetEncoder(LEFTENCODER);
	last_leftErrorP = 0;
	last_rightErrorP = 0;
	setLeftVelocity(0);
	setRightVelocity(0);
}

void turnLeft3() {

	double k = 0.03;
	setVelocity(0);
	bool left = false;

	int currentFrontLeft;

	int endL = getEncoder(LEFTENCODER) - 2*TURN_L;

	double errorL;

	while(!left)
	{
		brakeRight();
		currentFrontLeft = getEncoder(LEFTENCODER);

		errorL = endL - currentFrontLeft;
		if (abs(errorL) < CORRECTION_FRONT_THRESH) {
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

	resetEncoder(RIGHTENCODER);
	resetEncoder(LEFTENCODER);
	last_leftErrorP = 0;
	last_rightErrorP = 0;
	setLeftVelocity(0);
	setRightVelocity(0);
}

void moveCells(int num, double base_speed) {

	int location;
	int startL = getEncoder(LEFTENCODER);
	int startR = getEncoder(RIGHTENCODER);
	int distanceL = startL + CELL_L*(num);
	int distanceR = startR + CELL_R*(num);
	setLED(GREEN);
	double left_front_sensor, right_front_sensor;
	double left_side_sensor, right_side_sensor;
	bool is_left_wall  = true;
	bool is_right_wall = true;
	double speed = base_speed;
	bool slowDown = false;

	while(1) {
		left_front_sensor  = readSensor(LEFT_DET);
		right_front_sensor = readSensor(RIGHT_DET);
		left_side_sensor   = readSensor(LEFT_CEN_DET);
		right_side_sensor  = readSensor(RIGHT_CEN_DET);
		location = getEncoder(LEFTENCODER);

		// Stop for front walls
		if (right_front_sensor <= getWall(IDEALRIGHTFRONT) && left_front_sensor <= getWall(IDEALLEFTFRONT)) {
			setLED(RED);
			resetLED(GREEN);
			hardBrake();
			frontCorrection();
			break;
		}

		if (location > distanceL-CELL_L && !slowDown) {
			speed = 15;
			slowDown = true;
		}

		// Stop after moving number of cells
		if (location > distanceL) {
			brakeCorrection(distanceR, distanceL);
			break;
		}

		// Check for left wall
		if (left_side_sensor <= getWall(FARLEFTWALL)) {
			is_left_wall = true;
		}
		else {
			is_left_wall = false;
		}
		// Check for right wall
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

		correction2(speed);

	}
}

void move(int cells, double base_speed) {

	int endL = getEncoder(LEFTENCODER) + cells*CELL_L;
	int endR = getEncoder(RIGHTENCODER) + cells*CELL_R;
	bool done = false;

	int currentLeft;

	double errorL;
	double errorPercent, dePercent;
	double frontLeft,frontRight;
	double speed;
	double dt;
	double diff;
	int temp;
	int endTick = HAL_GetTick()+1000;
	bool dtFlag = false;

	printInt(endTick);
	printNL();

	while(!done)
	{
		batteryFault();

		frontLeft = readSensor(LEFT_DET);
		frontRight = readSensor(RIGHT_DET);
		temp = HAL_GetTick();
		diff = endTick - temp;
		dt = 1 - diff/1000;

		if (dt >= 1) {
			dt = 1;
			dtFlag = true;
		}
		if (dt <= 0) {
			dt = 0;
		}

		speed = base_speed*dt;

		if (frontLeft <= getWall(IDEALLEFTFRONT) && frontRight <= getWall(IDEALRIGHTFRONT)) {
			hardBrake();
			frontCorrection();
			break;
		}

		// Grab current location
		currentLeft = getEncoder(LEFTENCODER);

		// Calculate Error given location
		errorL = endL - currentLeft;
		errorPercent = errorL/(cells*CELL_L);
		dePercent = errorL/(cells*CELL_L*UPPER_BOUND);

		// If position has passed desired location
		if (endL <= currentLeft) {
			brakeCorrection(endR, endL);
			done = true;
		}
//		else if (errorL < CELL_L) {
//			correction2(SLOW);
//		}
		else if (errorPercent > UPPER_BOUND) {
			correction2(speed);
		}
		else {
			if (speed*dePercent < SLOW) {
				correction2(SLOW);
			}
			else {
				correction2(speed*dePercent);
			}
		}
	}
	HAL_Delay(100);
	HAL_TIM_Base_Stop_IT(&htim2);
	setSpeed(LEFTMOTOR,0);
	setSpeed(RIGHTMOTOR,0);
	resetEncoder(RIGHTENCODER);
	resetEncoder(LEFTENCODER);
}
