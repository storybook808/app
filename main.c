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
#include "flash.h"

extern TIM_HandleTypeDef htim2;

void main(void) {
	HAL_Init();
	SystemClock_Config();
	initSystem();

	// Test USART Connection
	printString("Hello world!");
	printNL();

	loadCalibration();
	loadMap();

	resetLED(WHITE);

	testChaser(1,250);

	resetEncoder(LEFTENCODER);
	resetEncoder(RIGHTENCODER);

	Mode mode;

	int i;

	while(1) {

//		sensor_value = readSensor(RIGHT_CEN_DET);
//		printFloat(sensor_value);
//		printNL();
//		HAL_Delay(500);

		mode = menu();

		if (mode == MODE1) {
			printMap();
		}

		else if (mode == MODE2) {
			emptyMap();
			mapSlow();
			saveMap();
		}

		else if (mode == MODE3) {
			for (i = 0; i < 16; ++i) {
				printInt(row[i]);
				printNL();
			}
			if(saveMap()) playBuzzer(5000,100);
			loadMap();
			for (i = 0; i < 16; ++i) {
				printInt(row[i]);
				printNL();
			}
		}

		else {
			resetLEDAll();
			calibrateSensors();
			setLED(WHITE);
			setLED(GREEN);
			if (eraseCalibration()) {
				while(1) {
					printInt(41);
					printNL();
				}
			}
			setLEDAll();
			HAL_Delay(500);
			resetLEDAll();
			setLED(BLUE);
			setLED(RED);
			if (writeCalibration()) {
				while(1) {
					printInt(42);
					printNL();
				}
			}
			resetLEDAll();
			toggleLED(RED);
			HAL_Delay(100);
			toggleLED(RED);
			HAL_Delay(100);
			toggleLED(RED);
			HAL_Delay(100);
			toggleLED(RED);
			HAL_Delay(100);
			toggleLED(RED);
			HAL_Delay(100);
			toggleLED(RED);
			HAL_Delay(100);
		}
	}
}
