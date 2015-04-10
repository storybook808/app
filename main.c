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
		batteryFault();
		toggleLED(WHITE);
	}

	resetLED(WHITE);

	testChaser(1,250);

	double base_speed = 50;
	double speed = base_speed;

	resetEncoder(LEFTENCODER);
	resetEncoder(RIGHTENCODER);

	double frontRight, frontLeft;
	double centerRight, centerLeft;
	bool rightWall, leftWall, frontWall, backWall;
	rightWall = true;
	leftWall = true;
	frontWall = false;
	backWall = true;

	int start = getEncoder(LEFTENCODER);
	int location;
	int end;
	bool map = true;
	bool cells[16][2];
	cells[0][0] = true;
	cells[0][1] = true;
	int cell = 0;
	int i;
	int value;
	bool stop;

	turnLeft();
	frontCorrection();
	turnLeft();
	frontCorrection();
	turnLeft();
	frontCorrection();
	turnLeft();

	while(1) {

		batteryFault();

		// Get Sensor Readings
		frontRight = readSensor(RIGHT_DET);
		frontLeft = readSensor(LEFT_DET);
		centerRight = readSensor(RIGHT_CEN_DET);
		centerLeft = readSensor(LEFT_CEN_DET);

		// Determine walls

		frontWall = hasFrontWall(frontRight,frontLeft);
		cell = location/CELL_L;

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
			if (x == 2 && y == 3) {
				setBuzzer(ON);
			}

			else setBuzzer(OFF);
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
