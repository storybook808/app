/* File Name: tracking.c
 * Project: EE 396
 * Team: TSM
 * Members: Mark Miw
 *          Steven Chen
 *          Joseph Felix Jr.
 */

#include "tracking.h"
//#define PRINT

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

bool hasFarFrontWall(double valueRight, double valueLeft) {
	if ((valueRight < getWall(FLOODRIGHTFRONT)) && (valueLeft < getWall(FLOODLEFTFRONT))) return true;
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

//	HAL_Delay(100);
	HAL_TIM_Base_Stop_IT(&htim2);
	setSpeed(RIGHTMOTOR,0);
	setSpeed(LEFTMOTOR,0);

	resetEncoder(RIGHTENCODER);
	resetEncoder(LEFTENCODER);
	last_leftErrorP = 0;
	last_rightErrorP = 0;
	setLeftVelocity(0);
	setRightVelocity(0);
	if (dir == 3) dir = 0;
	else dir++;
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

	int endR = getEncoder(RIGHTENCODER) + TURN_R+80;
	int endL = getEncoder(LEFTENCODER) - TURN_L-80;

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

//	HAL_Delay(100);
	HAL_TIM_Base_Stop_IT(&htim2);
	setSpeed(RIGHTMOTOR,0);
	setSpeed(LEFTMOTOR,0);

	resetEncoder(RIGHTENCODER);
	resetEncoder(LEFTENCODER);
	last_leftErrorP = 0;
	last_rightErrorP = 0;
	setLeftVelocity(0);
	setRightVelocity(0);
	if (dir == 0) dir = 3;
	else dir--;
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
	resetEncoder(RIGHTENCODER);
	resetEncoder(LEFTENCODER);

	int endL = getEncoder(LEFTENCODER) + cells*CELL_L;
	int endR = getEncoder(RIGHTENCODER) + cells*CELL_R;
	bool done = false;

	int currentLeft;
	int currentRight;

	double errorL;
	double errorPercent, dePercent;
	double frontLeft,frontRight;
	double speed;
	double dt;
	double diff;
	int temp;
	int endTick = HAL_GetTick()+1000;

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
			frontCorrection();
			break;
		}

		// Grab current location
		currentLeft = getEncoder(LEFTENCODER);
		currentRight = getEncoder(RIGHTENCODER);

		// Calculate Error given location
		errorL = endL - currentLeft;
		errorPercent = errorL/(cells*CELL_L);
		dePercent = errorL/(cells*CELL_L*UPPER_BOUND);

		// If position has passed desired location
		if (endL <= currentLeft || endR <= currentRight) {
			brakeCorrection(endR, endL);
			done = true;
		}
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
//	HAL_Delay(100);
	HAL_TIM_Base_Stop_IT(&htim2);
	setSpeed(LEFTMOTOR,0);
	setSpeed(RIGHTMOTOR,0);
	resetEncoder(RIGHTENCODER);
	resetEncoder(LEFTENCODER);
	switch (dir) {
		case NORTH:
			y+=cells;
			break;
		case SOUTH:
			y-=cells;
			break;
		case EAST:
			x+=cells;
			break;
		case WEST:
			x-=cells;
			break;
		default:
			break;
	}
}

void moveSmart(int cells) {
	resetEncoder(RIGHTENCODER);
	resetEncoder(LEFTENCODER);

	int endL = getEncoder(LEFTENCODER) + cells*CELL_L;
	int endR = getEncoder(RIGHTENCODER) + cells*CELL_R;
	bool done = false;

	int currentLeft;
	int currentRight;

	double dePercent;
	double frontLeft,frontRight;
	double speed;
	double dt;
	double diff;

	double base_speed;
	int dist;
	int decelPoint;

	switch (cells) {
		case 1:
			base_speed = 75;
			dist = 0.4*CELL_L;
			break;
		case 2:
			base_speed = 125;
			dist = 1*CELL_L;
			break;
		case 3:
		case 4:
		case 5:
		case 6:
			base_speed = 150;
			dist = 1.5*CELL_L;
			break;
		case 7:
		case 8:
			base_speed = 175;
			dist = 3*CELL_L;
			break;
		case 9:
		case 10:
		case 11:
		case 12:
		case 13:
		case 14:
		case 15:
			base_speed = 200;
			dist = 4*CELL_L;
			break;
	}
	// Initialize deceleration point
	decelPoint = endL-dist;

	while(!done)
	{
		// Grab Sensor readings for emergency stop
		frontLeft = readSensor(LEFT_DET);
		frontRight = readSensor(RIGHT_DET);

		if (frontLeft <= getWall(IDEALLEFTFRONT) && frontRight <= getWall(IDEALRIGHTFRONT)) {
			frontCorrection();
			break;
		}

		// Grab current location
		currentLeft = getEncoder(LEFTENCODER);
		currentRight = getEncoder(RIGHTENCODER);

		// Calculate Speed for acceleration given distance to target distance
		diff = dist - currentLeft;
		dt = 1 - diff/dist;

		// Safety limits for dt
		if (dt >= 1) dt = 1;
		if (dt <= 0) dt = 0;

		// Safety lower limits for velocity to avoid blowing motors
		if (base_speed*dt < 30) speed = 30;
		else speed = base_speed*dt;

		// Calculate Deceleration given location;
		dePercent = (endL-currentLeft)/dist;

		// If position has passed desired location brakeCorrect and finish
		if (endL <= currentLeft || endR <= currentRight) {
			brakeCorrection(endR, endL);
			done = true;
		}
		// If we haven't reached deceleration point keep going forward at speed
		else if (currentLeft < decelPoint) {
			correction2(speed);
		}
		// If we have reached deceleration point
		else {
			// If slow speed is less than 30, go 30
			if (speed*dePercent < 30) {
				correction2(30);
			}
			else {
				correction2(speed*dePercent);
			}
		}
	}
	HAL_TIM_Base_Stop_IT(&htim2);
	setSpeed(LEFTMOTOR,0);
	setSpeed(RIGHTMOTOR,0);
	resetEncoder(RIGHTENCODER);
	resetEncoder(LEFTENCODER);
	switch (dir) {
		case NORTH:
			y+=cells;
			break;
		case SOUTH:
			y-=cells;
			break;
		case EAST:
			x+=cells;
			break;
		case WEST:
			x-=cells;
			break;
		default:
			break;
	}
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

bool startCellStop() {
	if (!brb) {
		if (x == 0 && y == 0) {
			hardBrake();
			playBuzzer(100,100);
			playBuzzer(100,100);
			playBuzzer(100,100);
			brb = true;;
			return true;
		}
	}
	if (x != 0 || y != 0) {
		brb = false;
	}
	return false;
}

bool centerCellStop() {
	if (!brb) {
		if ((x == 7 && y == 7)||(x == 7 && y == 8)||(x == 8 && y == 7)||(x == 8 && y == 8)) {
			playBuzzer(10,0);
			brb = true;;
			return true;
		}
	}
	if (x != 0 || y != 0) {
		brb = false;
	}
	return false;
}

void searchSlow() {
	double base_speed = 50;
	double frontRight, frontLeft;
	double centerRight;
	int location;
	bool rightWall, frontWall;
	bool map = true;

	x = 0;
	y = 0;
	dir = 0;

	rightWall = true;
	frontWall = false;

	resetEncoder(RIGHTENCODER);
	resetEncoder(LEFTENCODER);

	while(1) {

		// Get Sensor Readings
		frontRight = readSensor(RIGHT_DET);
		frontLeft = readSensor(LEFT_DET);
		centerRight = readSensor(RIGHT_CEN_DET);

		// Determine walls

		frontWall = hasFrontWall(frontRight,frontLeft);

		// Determine location
		location = getEncoder(LEFTENCODER);

		// If we are half-way through a cell
		if (location%CELL_L >= CELL_L/2 && map) {
			// Check side walls
			rightWall = hasRightWall(centerRight);
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
			setBuzzer(OFF);
			rightWall = true;
			// Turn right
			turnRight();
			if (dir == 3) {
				dir = 0;
			}
			else dir++;
			frontWall = false;
		}

		// If front wall
		if (frontWall && rightWall) {
			// Stop & correct off wall
			hardBrake();
			if(location%CELL_L >= CELL_L/2 && map) {
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
			}
			frontCorrection();
			turnLeft();
			if (dir == 0) {
				dir = 3;
			}
			else dir--;
			frontWall = false;
			rightWall = true;
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

void mapSlow() {
	double base_speed = 50;
	double frontRight, frontLeft;
	double centerRight, centerLeft;
	int location;
	bool rightWall, leftWall, frontWall, backWall;
	bool map = true;
	Cell here;

	x = 0;
	y = 0;
	dir = 0;

	rightWall = true;
	leftWall = true;
	frontWall = false;
	backWall = true;

	resetEncoder(RIGHTENCODER);
	resetEncoder(LEFTENCODER);

	while(1) {

		// Get Sensor Readings
		frontRight = readSensor(RIGHT_DET);
		frontLeft = readSensor(LEFT_DET);
		centerRight = readSensor(RIGHT_CEN_DET);
		centerLeft = readSensor(LEFT_CEN_DET);

		// Determine walls

		frontWall = hasFrontWall(frontRight, frontLeft);

		// Determine location
		location = getEncoder(LEFTENCODER);

		if (location >= CELL_L) {
			resetEncoder(RIGHTENCODER);
			resetEncoder(LEFTENCODER);
		}

		// If we are half-way through a cell
		if (location%CELL_L >= CELL_L/2 && map) {
			// Check side walls
			rightWall = hasRightWall(centerRight);
			leftWall = hasLeftWall(centerLeft);
			// Check front wall
			 frontWall = hasFrontWall(frontRight,frontLeft);
			backWall = false;
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
			// Map values to map here
			switch (dir) {
			// North
				case 0:
					here.north = frontWall;
					here.south = backWall;
					here.west = leftWall;
					here.east = rightWall;
					break;
			// East
				case 1:
					here.north = leftWall;
					here.south = rightWall;
					here.west = backWall;
					here.east = frontWall;
					break;
			// South
				case 2:
					here.north = backWall;
					here.south = frontWall;
					here.west = rightWall;
					here.east = leftWall;
					break;
			//West
				case 3:
					here.north = rightWall;
					here.south = leftWall;
					here.west = frontWall;
					here.east = backWall;
					break;
				default:
					break;
			}
			mapSideWalls(x,y,here);

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
			frontRight = readSensor(RIGHT_DET);
			frontLeft = readSensor(LEFT_DET);
			if (hasFrontWall(frontRight,frontLeft)) {
				frontCorrection();
				mapFrontWall(x,y,true);
			}
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
//			hardBrake();
			frontCorrection();
//			if(location%CELL_L >= CELL_L/2 && map) {
//				switch (dir) {
//					case 0:
//						y++;
//						break;
//					case 1:
//						x++;
//						break;
//					case 2:
//						y--;
//						break;
//					case 3:
//						x--;
//						break;
//					default:
//						break;
//				}
//				map = false;
//			}
			mapFrontWall(x,y,true);
			turnLeft();
			if (dir == 0) {
				dir = 3;
			}
			else dir--;
			frontWall = leftWall;;
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

		if (startCellStop()) break;

		correction2(base_speed);
	}
}

void floodSlow(double base_speed) {
	double frontRight, frontLeft;
	double centerRight, centerLeft;
	int location;
	bool rightWall, leftWall, frontWall, backWall;
	bool mapTime = true;
	Cell here;
	bool center = false;

	rightWall = true;
	leftWall = true;
	frontWall = false;
	backWall = true;

	uint8_t decision;

	float floodRight, floodFront, floodLeft, floodBack;

	resetEncoder(RIGHTENCODER);
	resetEncoder(LEFTENCODER);

	bool flag = false;

	while(1) {

		// Get Sensor Readings
		frontRight = readSensor(RIGHT_DET);
		frontLeft = readSensor(LEFT_DET);
		centerRight = readSensor(RIGHT_CEN_DET);
		centerLeft = readSensor(LEFT_CEN_DET);

		// Determine walls

		// Determine location
		location = getEncoder(LEFTENCODER);

		if (location >= CELL_L) {
			resetEncoder(RIGHTENCODER);
			resetEncoder(LEFTENCODER);
		}

		// If we are half-way through a cell
		if (location%CELL_L >= CELL_L/2 && mapTime) {
			// Check side walls.
			rightWall = hasRightWall(centerRight);
			leftWall = hasLeftWall(centerLeft);
			// Check front wall.
			frontWall = hasFarFrontWall(frontRight,frontLeft);
			backWall = false;

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
			// Map values to map here
			if (1) { //map[x][y].mapped == false
				switch (dir) {
				// North
					case 0:
						here.north = frontWall;
						here.south = backWall;
						here.west = leftWall;
						here.east = rightWall;
						break;
				// East
					case 1:
						here.north = leftWall;
						here.south = rightWall;
						here.west = backWall;
						here.east = frontWall;
						break;
				// South
					case 2:
						here.north = backWall;
						here.south = frontWall;
						here.west = rightWall;
						here.east = leftWall;
						break;
				//West
					case 3:
						here.north = rightWall;
						here.south = leftWall;
						here.west = frontWall;
						here.east = backWall;
						break;
					default:
						break;
				}
				mapSideWalls(x,y,here);
			}
			else {
				here = getWallsForCell(x,y);
				switch (dir) {
				// North
					case 0:
						frontWall = here.north;
						backWall = here.south;
						leftWall = here.west;
						rightWall = here.east;
						break;
				// East
					case 1:
						leftWall = here.north;
						rightWall = here.south;
						backWall = here.west;
						frontWall = here.east;
						break;
				// South
					case 2:
						backWall = here.north;
						frontWall = here.south;
						rightWall = here.west;
						leftWall = here.east;
						break;
				//West
					case 3:
						rightWall = here.north;
						leftWall = here.south;
						frontWall = here.west;
						backWall = here.east;
						break;
					default:
						break;
				}
			}

			decision = 0 + leftWall;
			decision <<= 1;
			decision += frontWall;
			decision <<= 1;
			decision += rightWall;

			if (!flag) {
				if (x == 8 && y == 8) {
					flag = true;
				}
			}

			if (!flag) {
				floodCenter();
			}
			else {
				floodStart();
			}

			switch(decision) {
			case 0:
				// Left: 0 Front: 0 Right: 0

				switch (dir) {
				case NORTH:
					floodLeft = getFloodValue(setCoordinate(x,y),WEST);
					floodFront = getFloodValue(setCoordinate(x,y),NORTH);
					floodRight = getFloodValue(setCoordinate(x,y),EAST);
					floodBack = getFloodValue(setCoordinate(x,y),SOUTH);
					break;
				case EAST:
					floodLeft = getFloodValue(setCoordinate(x,y),NORTH);
					floodFront = getFloodValue(setCoordinate(x,y),EAST);
					floodRight = getFloodValue(setCoordinate(x,y),SOUTH);
					floodBack = getFloodValue(setCoordinate(x,y),WEST);
					break;
				case SOUTH:
					floodLeft = getFloodValue(setCoordinate(x,y),EAST);
					floodFront = getFloodValue(setCoordinate(x,y),SOUTH);
					floodRight = getFloodValue(setCoordinate(x,y),WEST);
					floodBack = getFloodValue(setCoordinate(x,y),NORTH);
					break;
				case WEST:
					floodLeft = getFloodValue(setCoordinate(x,y),SOUTH);
					floodFront = getFloodValue(setCoordinate(x,y),WEST);
					floodRight = getFloodValue(setCoordinate(x,y),NORTH);
					floodBack = getFloodValue(setCoordinate(x,y),EAST);
					break;
				}

				if (floodFront <= floodLeft && floodFront <= floodRight && floodFront <= floodBack) {

				}

				else if (floodRight <= floodLeft && floodRight <= floodBack) {
					brakeInCell(base_speed);
					turnRight();
				}

				else if (floodLeft <= floodBack) {
					brakeInCell(base_speed);
					turnLeft();
				}

				else {
					brakeInCell(base_speed);
					turnLeft();
					turnLeft();
				}

				break;
			case 1:
				// Left: 0 Front: 0 Right: 1

				switch (dir) {
				case NORTH:
					floodLeft = getFloodValue(setCoordinate(x,y),WEST);
					floodFront = getFloodValue(setCoordinate(x,y),NORTH);
					break;
				case EAST:
					floodLeft = getFloodValue(setCoordinate(x,y),NORTH);
					floodFront = getFloodValue(setCoordinate(x,y),EAST);
					break;
				case SOUTH:
					floodLeft = getFloodValue(setCoordinate(x,y),EAST);
					floodFront = getFloodValue(setCoordinate(x,y),SOUTH);
					break;
				case WEST:
					floodLeft = getFloodValue(setCoordinate(x,y),SOUTH);
					floodFront = getFloodValue(setCoordinate(x,y),WEST);
					break;
				}

				if (floodFront <= floodLeft) {

				}

				else {
					brakeInCell(base_speed);
					turnLeft();
				}

				break;
			case 2:
				// Left: 0 Front: 1 Right: 0
				frontCorrection();
				mapFrontWall(x, y, true);

				switch (dir) {
				case NORTH:
					floodLeft = getFloodValue(setCoordinate(x,y),WEST);
					floodRight = getFloodValue(setCoordinate(x,y),EAST);
					break;
				case EAST:
					floodLeft = getFloodValue(setCoordinate(x,y),NORTH);
					floodRight = getFloodValue(setCoordinate(x,y),SOUTH);
					break;
				case SOUTH:
					floodLeft = getFloodValue(setCoordinate(x,y),EAST);
					floodRight = getFloodValue(setCoordinate(x,y),WEST);
					break;
				case WEST:
					floodLeft = getFloodValue(setCoordinate(x,y),SOUTH);
					floodRight = getFloodValue(setCoordinate(x,y),NORTH);
					break;
				}

				if (floodRight <= floodLeft) {
					brakeInCell(base_speed);
					turnRight();
				}

				else {
					brakeInCell(base_speed);
					turnLeft();
				}

				break;
			case 3:
				// Left: 0 Front: 1 Right: 1
				frontCorrection();
				mapFrontWall(x, y, true);
				turnLeft();
				break;
			case 4:
				// Left: 1 Front: 0 Right: 0
				switch (dir) {
				case NORTH:
					floodFront = getFloodValue(setCoordinate(x,y),NORTH);
					floodRight = getFloodValue(setCoordinate(x,y),EAST);
					break;
				case EAST:
					floodFront = getFloodValue(setCoordinate(x,y),EAST);
					floodRight = getFloodValue(setCoordinate(x,y),SOUTH);
					break;
				case SOUTH:
					floodFront = getFloodValue(setCoordinate(x,y),SOUTH);
					floodRight = getFloodValue(setCoordinate(x,y),WEST);
					break;
				case WEST:
					floodFront = getFloodValue(setCoordinate(x,y),WEST);
					floodRight = getFloodValue(setCoordinate(x,y),NORTH);
					break;
				}

				if (floodFront <= floodRight) {

				}

				else {
					brakeInCell(base_speed);
					turnRight();
				}
				break;
			case 5:
				// Left: 1 Front: 0 Right: 1
				break;
			case 6:
				// Left: 1 Front: 1 Right: 0
				frontCorrection();
				mapFrontWall(x, y, true);
				turnRight();
				break;
			case 7:
				// Left: 1 Front: 1 Right: 1
				frontCorrection();
				mapFrontWall(x,y,true);
				turnLeft();
				frontCorrection();
				mapFrontWall(x,y,true);
				turnLeft();
				break;
			default:
				setBuzzer(ON);
				break;
			}

			// Disable Mapping
			mapTime = false;
		}
		// If we are a quarter in a cell
		if (location%CELL_L <= CELL_L/4 && !mapTime) {
			// Enable mapping
			mapTime = true;
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

		if (startCellStop()) break;
		if (!center) {
			if (centerCellStop()) center = true;
		}

		correction2(base_speed);
	}
}

void resetPath(Path *path) {
	path->moves = 0;
}

void addPathMove(Path *path, Move move) {
	path->path[path->moves] = move;
	path->moves++;
}

uint8_t getDirFromFloodValues(Coordinate here) {
	Cell currentCell = getWallsForCell(here.x,here.y);
	float floodNorth = getFloodValue(here,NORTH);
	float floodSouth = getFloodValue(here,SOUTH);
	float floodEast = getFloodValue(here,EAST);
	float floodWest = getFloodValue(here,WEST);

	FloodDir temp;
	temp.value = 255;

	if (!currentCell.west) {
		if (temp.value > floodWest) {
			temp.value = floodWest;
			temp.dir = WEST;
		}
	}
	if (!currentCell.south) {
		if (temp.value > floodSouth) {
			temp.value = floodSouth;
			temp.dir = SOUTH;
		}
	}
	if (!currentCell.east) {
		if (temp.value > floodEast) {
			temp.value = floodEast;
			temp.dir = EAST;
		}
	}
	if (!currentCell.north) {
		if (temp.value > floodNorth) {
			temp.value = floodNorth;
			temp.dir = NORTH;
		}
	}

	return temp.dir;
}

void updateCoordinate(Coordinate *here, uint8_t direction) {
	switch (direction) {
		case NORTH:
			here->y++;
			break;
		case SOUTH:
			here->y--;
			break;
		case EAST:
			here->x++;
			break;
		case WEST:
			here->x--;
			break;
		default:
			break;
	}
}

void updateDirection(uint8_t *direction, Move turn) {
	if (turn == RT90) {
		if (*direction == 3) {
			*direction = 0;
		}
		else *direction = *direction + 1;
	}
	else if (turn == LT90) {
		if (*direction == 0) {
			*direction = 3;
		}
		else *direction = *direction - 1;
	}
	else if(turn == T180) {
		*direction+=2;
		*direction%=4;
	}
}

Path findShortestPath() {
	// Find Center Entrance Cell's back cell
	Coordinate center = farCenterCell();

	// Set initial Conditions of Direction North and coordinate (0,0)
	uint8_t direction = NORTH;
	uint8_t floodDirection;
	Coordinate currentCell = setCoordinate(0,0);

	// Flood to back center cell
	floodToCell(center.x,center.y);

	// Initialize move counter for stack
	Path moves;
	resetPath(&moves);

	// Fill path until we have reached end cell
	while (currentCell.x!=center.x || currentCell.y!=center.y) {
		floodDirection = getDirFromFloodValues(currentCell);
#ifdef PRINT
		printString("Cell:(");
		printInt(currentCell.x);
		printComma();
		printInt(currentCell.y);
		printString(") dir:");
		printInt(direction);
		printString(" Go:");
		printInt(floodDirection);
		printNL();
#endif
		// If flood values tell us to move forward
		if (direction == floodDirection) {
			// Add forward command to path
			addPathMove(&moves,FWD);
			// Update cell coordinate for next round
			updateCoordinate(&currentCell, direction);
		}
		// If flood value is not in the same direction, determine turn
		else {
			// From current Direction, decide turn
			switch (direction) {
				case NORTH:
					if (floodDirection == WEST) {
						addPathMove(&moves,LT90);
						updateDirection(&direction,LT90);
					}
					else if (floodDirection == EAST) {
						addPathMove(&moves,RT90);
						updateDirection(&direction,RT90);
					}
					else {
						addPathMove(&moves,T180);
						updateDirection(&direction,T180);
					}
					break;
				case EAST:
					if (floodDirection == NORTH) {
						addPathMove(&moves, LT90);
						updateDirection(&direction,LT90);
					}
					else if (floodDirection == SOUTH) {
						addPathMove(&moves, RT90);
						updateDirection(&direction, RT90);
					}
					else {
						addPathMove(&moves,T180);
						updateDirection(&direction,T180);
					}
					break;
				case SOUTH:
					if (floodDirection == EAST) {
						addPathMove(&moves, LT90);
						updateDirection(&direction,LT90);
					}
					else if (floodDirection == WEST) {
						addPathMove(&moves, RT90);
						updateDirection(&direction, RT90);
					}
					else {
						addPathMove(&moves,T180);
						updateDirection(&direction,T180);
					}
					break;
				case WEST:
					if (floodDirection == SOUTH) {
						addPathMove(&moves, LT90);
						updateDirection(&direction,LT90);
					}
					else if (floodDirection == NORTH) {
						addPathMove(&moves, RT90);
						updateDirection(&direction, RT90);
					}
					else {
						addPathMove(&moves,T180);
						updateDirection(&direction,T180);
					}
					break;
				default:
					break;
			}
		}
	}
	return moves;
}

void printPath(Path *moves) {
	int count = moves->moves;
	int i;
	Move decision;
	for (i = 0; i < count; ++i) {
		decision = moves->path[i];
		switch (decision) {
			case FWD:
				printString("FWD");
				break;
			case LT90:
				printString("LT90");
				break;
			case RT90:
				printString("RT90");
				break;
			case T180:
				printString("T180");
				break;
			default:
				break;
		}
		printNL();
	}
}

void speed1(double base_speed) {
	Path path = findShortestPath();
	int i;
	int count = path.moves;
	int forward = 0;

	for (i = 0; i < count; ++i) {
		if (path.path[i] == FWD) {
			forward++;
		}
		else if (path.path[i] == RT90) {
			moveSmart(forward);
			forward = 0;
			turnRight();
		}
		else if (path.path[i] == LT90) {
			move(forward, base_speed);
			forward = 0;
			turnLeft();
		}
	}
}

void curveTurn(Wall turn, double base_speed) {
//	resetEncoder(RIGHTENCODER);
//	resetEncoder(LEFTENCODER);
	int startLeft = getEncoder(LEFTENCODER);
	int startRight = getEncoder(RIGHTENCODER);
	int currentLeft;
	int currentRight;
	bool done = false;

	float curveMinor = 0.466;
	float curveMajor = 1.4281;

	while(!done) {
		currentLeft = getEncoder(LEFTENCODER);
		correction2(base_speed);
		if (currentLeft >= startLeft+486) {
			done = true;
		}
	}

	done = false;

	if (turn == RIGHT) {
		setRightVelocity(base_speed*curveMinor);
		setLeftVelocity(base_speed*curveMajor);
		while(!done) {
			currentLeft = getEncoder(LEFTENCODER);
			if ((currentLeft - (startLeft+486)) >= 4362) {
				done = true;
			}
		}
		if (dir == 3) dir = 0;
		else dir++;
		setVelocity(base_speed);
	}
	else if (turn == LEFT) {
		setLeftVelocity(base_speed*curveMinor);
		setRightVelocity(base_speed*curveMajor);
		while(!done) {
			currentRight = getEncoder(RIGHTENCODER);
			if ((currentRight - (startRight+486))>= 4382) {
				done = true;
			}
		}
		if (dir == 0) dir = 3;
		else dir--;
		setVelocity(base_speed);
	}
//	setEncoder(RIGHTENCODER,(CELL_L/2-((CELL_L*10)/18)));
//	setEncoder(LEFTENCODER,(CELL_L/2-((CELL_L*10)/18)));
}

void floodSlow2() {
	double frontRight, frontLeft;
	double centerRight, centerLeft;
	double base_speed = 75;
	int location;
	bool rightWall, leftWall, frontWall, backWall;
	bool mapTime = true;
	Cell here;
	bool curveFlag = false;

	rightWall = true;
	leftWall = true;
	frontWall = false;
	backWall = true;

	uint8_t decision;

	float floodRight, floodFront, floodLeft;

	resetEncoder(RIGHTENCODER);
	resetEncoder(LEFTENCODER);

	bool flag = false;

	while(1) {

		// Get Sensor Readings
		frontRight = readSensor(RIGHT_DET);
		frontLeft = readSensor(LEFT_DET);
		centerRight = readSensor(RIGHT_CEN_DET);
		centerLeft = readSensor(LEFT_CEN_DET);

		// Determine walls

		// Determine location
		location = getEncoder(LEFTENCODER);

		if (location >= CELL_L) {
			resetEncoder(RIGHTENCODER);
			resetEncoder(LEFTENCODER);
		}

		// If we are half-way through a cell
		if ((location%CELL_L >= (CELL_L/2-243) && mapTime) || curveFlag) {
			if (curveFlag) {
				setEncoder(RIGHTENCODER,CELL_L/2-243);
				setEncoder(LEFTENCODER,CELL_R/2-243);
				curveFlag = false;
			}
			// Check side walls.
			rightWall = hasRightWall(centerRight);
			leftWall = hasLeftWall(centerLeft);
			// Check front wall.
			frontWall = hasFarFrontWall(frontRight,frontLeft);
			backWall = false;

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
			// Map values to map here
			if (1) { //map[x][y].mapped == false
				switch (dir) {
				// North
					case 0:
						here.north = frontWall;
						here.south = backWall;
						here.west = leftWall;
						here.east = rightWall;
						break;
				// East
					case 1:
						here.north = leftWall;
						here.south = rightWall;
						here.west = backWall;
						here.east = frontWall;
						break;
				// South
					case 2:
						here.north = backWall;
						here.south = frontWall;
						here.west = rightWall;
						here.east = leftWall;
						break;
				//West
					case 3:
						here.north = rightWall;
						here.south = leftWall;
						here.west = frontWall;
						here.east = backWall;
						break;
					default:
						break;
				}
				mapSideWalls(x,y,here);
			}
			else {
				here = getWallsForCell(x,y);
				switch (dir) {
				// North
					case 0:
						frontWall = here.north;
						backWall = here.south;
						leftWall = here.west;
						rightWall = here.east;
						break;
				// East
					case 1:
						leftWall = here.north;
						rightWall = here.south;
						backWall = here.west;
						frontWall = here.east;
						break;
				// South
					case 2:
						backWall = here.north;
						frontWall = here.south;
						rightWall = here.west;
						leftWall = here.east;
						break;
				//West
					case 3:
						rightWall = here.north;
						leftWall = here.south;
						frontWall = here.west;
						backWall = here.east;
						break;
					default:
						break;
				}
			}

			decision = 0 + leftWall;
			decision <<= 1;
			decision += frontWall;
			decision <<= 1;
			decision += rightWall;

			if (!flag) {
				if (x == 8 && y == 8) {
					flag = true;
				}
			}

			if (!flag) {
				floodCenter();
			}
			else {
				floodStart();
			}

			switch(decision) {
			case 0:
				// Left: 0 Front: 0 Right: 0

				switch (dir) {
				case NORTH:
					floodLeft = getFloodValue(setCoordinate(x,y),WEST);
					floodFront = getFloodValue(setCoordinate(x,y),NORTH);
					floodRight = getFloodValue(setCoordinate(x,y),EAST);
					break;
				case EAST:
					floodLeft = getFloodValue(setCoordinate(x,y),NORTH);
					floodFront = getFloodValue(setCoordinate(x,y),EAST);
					floodRight = getFloodValue(setCoordinate(x,y),SOUTH);
					break;
				case SOUTH:
					floodLeft = getFloodValue(setCoordinate(x,y),EAST);
					floodFront = getFloodValue(setCoordinate(x,y),SOUTH);
					floodRight = getFloodValue(setCoordinate(x,y),WEST);
					break;
				case WEST:
					floodLeft = getFloodValue(setCoordinate(x,y),SOUTH);
					floodFront = getFloodValue(setCoordinate(x,y),WEST);
					floodRight = getFloodValue(setCoordinate(x,y),NORTH);
					break;
				}

				if (floodFront <= floodLeft && floodFront <= floodRight) {

				}

				else if (floodRight <= floodLeft) {
					curveTurn(RIGHT,base_speed);
					curveFlag = true;

				}

				else {
					curveTurn(LEFT,base_speed);
					curveFlag = true;

				}

				break;
			case 1:
				// Left: 0 Front: 0 Right: 1

				switch (dir) {
				case NORTH:
					floodLeft = getFloodValue(setCoordinate(x,y),WEST);
					floodFront = getFloodValue(setCoordinate(x,y),NORTH);
					break;
				case EAST:
					floodLeft = getFloodValue(setCoordinate(x,y),NORTH);
					floodFront = getFloodValue(setCoordinate(x,y),EAST);
					break;
				case SOUTH:
					floodLeft = getFloodValue(setCoordinate(x,y),EAST);
					floodFront = getFloodValue(setCoordinate(x,y),SOUTH);
					break;
				case WEST:
					floodLeft = getFloodValue(setCoordinate(x,y),SOUTH);
					floodFront = getFloodValue(setCoordinate(x,y),WEST);
					break;
				}

				if (floodFront <= floodLeft) {

				}

				else {
					curveTurn(LEFT,base_speed);
					curveFlag = true;

				}

				break;
			case 2:
				// Left: 0 Front: 1 Right: 0
				mapFrontWall(x, y, true);

				switch (dir) {
				case NORTH:
					floodLeft = getFloodValue(setCoordinate(x,y),WEST);
					floodRight = getFloodValue(setCoordinate(x,y),EAST);
					break;
				case EAST:
					floodLeft = getFloodValue(setCoordinate(x,y),NORTH);
					floodRight = getFloodValue(setCoordinate(x,y),SOUTH);
					break;
				case SOUTH:
					floodLeft = getFloodValue(setCoordinate(x,y),EAST);
					floodRight = getFloodValue(setCoordinate(x,y),WEST);
					break;
				case WEST:
					floodLeft = getFloodValue(setCoordinate(x,y),SOUTH);
					floodRight = getFloodValue(setCoordinate(x,y),NORTH);
					break;
				}

				if (floodRight <= floodLeft) {
					curveTurn(RIGHT,base_speed);
					curveFlag = true;
				}

				else {
					curveTurn(LEFT,base_speed);
					curveFlag = true;
				}

				break;
			case 3:
				// Left: 0 Front: 1 Right: 1
				mapFrontWall(x, y, true);
				curveTurn(LEFT,base_speed);
				curveFlag = true;

				break;
			case 4:
				// Left: 1 Front: 0 Right: 0
				switch (dir) {
				case NORTH:
					floodFront = getFloodValue(setCoordinate(x,y),NORTH);
					floodRight = getFloodValue(setCoordinate(x,y),EAST);
					break;
				case EAST:
					floodFront = getFloodValue(setCoordinate(x,y),EAST);
					floodRight = getFloodValue(setCoordinate(x,y),SOUTH);
					break;
				case SOUTH:
					floodFront = getFloodValue(setCoordinate(x,y),SOUTH);
					floodRight = getFloodValue(setCoordinate(x,y),WEST);
					break;
				case WEST:
					floodFront = getFloodValue(setCoordinate(x,y),WEST);
					floodRight = getFloodValue(setCoordinate(x,y),NORTH);
					break;
				}

				if (floodFront <= floodRight) {

				}

				else {
					curveTurn(RIGHT,base_speed);
					curveFlag = true;

				}
				break;
			case 5:
				// Left: 1 Front: 0 Right: 1
				break;
			case 6:
				// Left: 1 Front: 1 Right: 0
				mapFrontWall(x, y, true);
				curveTurn(RIGHT,base_speed);
				curveFlag = true;

				break;
			case 7:
				// Left: 1 Front: 1 Right: 1
				frontCorrection();
				mapFrontWall(x,y,true);
				turnLeft();
				frontCorrection();
				mapFrontWall(x,y,true);
				turnLeft();
				break;
			default:
				setBuzzer(ON);
				break;
			}

			// Disable Mapping
			mapTime = false;
		}
		// If we are a quarter in a cell
		if (location%CELL_L <= CELL_L/4 && !mapTime) {
			// Enable mapping
			mapTime = true;
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

		correction2(base_speed);
	}
}
