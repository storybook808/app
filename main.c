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
	bool rightWall, leftWall, frontWall;
	rightWall = true;
	leftWall = true;
	frontWall = false;

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
			// Map values to map here

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
		}

		// If front wall
		if (frontWall) {
			// Stop & correct off wall
			hardBrake();
			frontCorrection();
			turnLeft();
		}

		correction2(base_speed);
	}
}
