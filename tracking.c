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
	int endL = getEncoder(LEFTENCODER) + TURN_L-75;

	double errorR;
	double errorL;

	while(!right || !left)
	{
		currentFrontRight = getEncoder(RIGHTENCODER);
		currentFrontLeft = getEncoder(LEFTENCODER);

		errorR = endR - currentFrontRight;
		errorL = endL - currentFrontLeft;

		if (abs(errorR) < TURN_THRESH) {
			errorR = 0;
			brakeRight();
			right = true;
		}
		else {
			setRightVelocity(errorR*k);
		}
		if (abs(errorL) < TURN_THRESH) {
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

		if (abs(errorR) < TURN_THRESH) {
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

	int endL = getEncoder(LEFTENCODER) + 2*TURN_L-150;

	double errorL;

	while(!left)
	{
		brakeRight();
		currentFrontLeft = getEncoder(LEFTENCODER);

		errorL = endL - currentFrontLeft;

		if (abs(errorL) < TURN_THRESH) {
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

		if (abs(errorR) < TURN_THRESH) {
			errorR = 0;
			brakeRight();
			right = true;
		}
		else {
			setRightVelocity(errorR*k);
		}
		if (abs(errorL) < TURN_THRESH) {
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
	resetEncoder(RIGHTENCODER);
	resetEncoder(LEFTENCODER);
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

		if (abs(errorR) < TURN_THRESH) {
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
	resetEncoder(RIGHTENCODER);
	resetEncoder(LEFTENCODER);
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
		if (abs(errorL) < TURN_THRESH) {
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

	printInt(endTick);
	printNL();

	while(!done)
	{
		frontLeft = readSensor(LEFT_DET);
		frontRight = readSensor(RIGHT_DET);
		temp = HAL_GetTick();
		diff = endTick - temp;
		dt = 1 - diff/1000;

		if (dt >= 1) {
			dt = 1;
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

void brakeInCell(double base_speed) {

	int location = getEncoder(LEFTENCODER);
	int endL = ((location/CELL_L)+1)*CELL_L;
	int endR = ((getEncoder(RIGHTENCODER)/CELL_R)+1)*CELL_R;
	int total = endL - location;
	bool done = false;

	int currentLeft;

	double errorL;
	double errorPercent;
	double frontLeft,frontRight;

	while(!done)
	{
		frontLeft = readSensor(LEFT_DET);
		frontRight = readSensor(RIGHT_DET);

		if (frontLeft <= getWall(IDEALLEFTFRONT) && frontRight <= getWall(IDEALRIGHTFRONT)) {
			hardBrake();
			frontCorrection();
			break;
		}

		// Grab current location
		currentLeft = getEncoder(LEFTENCODER);

		// Calculate Error given location
		errorL = endL - currentLeft;
		errorPercent = errorL/total;

		// If position has passed desired location
		if (endL <= currentLeft) {
			brakeCorrection(endR, endL);
			done = true;
		}
		else {
			if (base_speed*errorPercent < SLOW) {
				correction2(SLOW);
			}
			else {
				correction2(base_speed*errorPercent);
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

void startCellStop() {
	if (!brb) {
		if (x == 0 && y == 0 && dir == 0) {
			brb = !brb;
			while(!getButton()) {
				toggleLEDAll();
				HAL_Delay(100);
			}
			HAL_Delay(500);
		}
	}
	if (x != 0 || y != 0) {
		brb = !brb;
	}
}

void searchSlow() {
	double base_speed = 50;
	double frontRight, frontLeft;
	double centerRight, centerLeft;
	int location;
	bool rightWall, leftWall, frontWall, backWall;
	bool map = true;

	x = 0;
	y = 0;
	dir = 0;

	rightWall = true;
	leftWall = true;
	frontWall = false;
	backWall = true;

	while(1) {

		// Get Sensor Readings
		frontRight = readSensor(RIGHT_DET);
		frontLeft = readSensor(LEFT_DET);
		centerRight = readSensor(RIGHT_CEN_DET);
		centerLeft = readSensor(LEFT_CEN_DET);

		// Determine walls

		frontWall = hasFrontWall(frontRight,frontLeft);

		// Determine location
		location = getEncoder(LEFTENCODER);

		// If we are half-way through a cell
		if (location%CELL_L >= CELL_L/2 && map) {
			// Check side walls
			rightWall = hasRightWall(centerRight);
			leftWall = hasLeftWall(centerLeft);
			backWall = frontWall;
			// Map values to map here
			switch (dir) {
				case 0:
					y++;
					break;
				case 1:
					x++;
					break;
				case 2:
					y--;
					break;
				case 3:
					x--;
					break;
				default:
					break;
			}
			// Disable Mapping
			map = false;
		}
		// If we are a quarter in a cell
		if (location%CELL_L <= CELL_L/4 && !map) {
			// Enable mapping
			map = true;
		}

		if (!rightWall) {
			// Brake at center of cell
			brakeInCell(base_speed);
			rightWall = true;
			leftWall = true;
			// Turn right
			turnRight();
			if (dir == 3) {
				dir = 0;
			}
			else dir++;
			frontWall = false;
			backWall = leftWall;
		}

		// If front wall
		if (frontWall && rightWall) {
			// Stop & correct off wall
			hardBrake();
			frontCorrection();
			turnLeft();
			if (dir == 0) {
				dir = 3;
			}
			else dir--;
			frontWall = false;
			rightWall = true;
			backWall = rightWall;
			leftWall = true;
		}

		switch (dir) {
			case 0:
				resetLEDAll();
				setLED(WHITE);
				break;
			case 1:
				resetLEDAll();
				setLED(BLUE);
				break;
			case 2:
				resetLEDAll();
				setLED(GREEN);
				break;
			case 3:
				resetLEDAll();
				setLED(RED);
				break;
			default:
				resetLEDAll();
				break;
		}

		startCellStop();

		correction2(base_speed);
}

}
