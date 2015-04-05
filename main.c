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
		toggleLED(WHITE);
	}

	resetLED(WHITE);

	testChaser(1,250);

	double base_speed = 100;

	setVelocity(0);

	resetEncoder(LEFTENCODER);
	resetEncoder(RIGHTENCODER);

	double right;
	double left;
	int rightEncoder = 0;
	int leftEncoder = 0;

	int startL[5];
	startL[0] = getEncoder(LEFTENCODER);
	int startR[5];
	startR[0] = getEncoder(RIGHTENCODER);

	int endL[5];
	int endR[10];

	float averageL = 0.0;
	float averageR = 0.0;

	double left_front_sensor;
	double right_front_sensor;

	int x = 0;

	while(1) {
		batteryFault();

		left_front_sensor = readSensor(LEFT_DET);
		right_front_sensor = readSensor(RIGHT_DET);

		if (right_front_sensor <= getWall(IDEALRIGHTFRONT) && left_front_sensor <= getWall(IDEALLEFTFRONT)) {
			setLED(WHITE);
			brake();
			frontCorrection();

			endL[x] = getEncoder(LEFTENCODER);
			endR [x]= getEncoder(RIGHTENCODER);

			x++;

			while(!getButton());

			if (x == 5) {
				for (x = 0; x < 5; x++) {
					averageL += endL[x] - startL[x];
					averageR += endR[x] - startR[x];
				}

				averageL /= 5;
				averageR /= 5;

				setLED(RED);

				while(!getButton());
				HAL_Delay(100);
				while(!getButton());
				HAL_Delay(100);
				while(!getButton());

				printFloat(averageL);
				printComma();
				printFloat(averageR);
				printNL();

				setLED(GREEN);

				while(1) batteryFault();

			}

			HAL_Delay(500);

			resetEncoder(LEFTENCODER);
			resetEncoder(RIGHTENCODER);

			startL[x] = getEncoder(LEFTENCODER);
			startR[x] = getEncoder(RIGHTENCODER);
		}

		correction(0, base_speed);
	}
}
