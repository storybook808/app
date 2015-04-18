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

	resetLED(WHITE);

	testChaser(1,250);

	resetEncoder(LEFTENCODER);
	resetEncoder(RIGHTENCODER);

	Mode mode;

	float sensor_value;

	while(1) {

//		sensor_value = readSensor(RIGHT_CEN_DET);
//		printFloat(sensor_value);
//		printNL();
//		HAL_Delay(500);

		mode = menu();

		if (mode == MODE1) {
			searchSlow();
		}

		else if (mode == MODE2) {
			int i;
			uint32_t flash_dest = ADDR_FLASH_SECTOR_6;

			float test = 12.5;
			uint32_t *testpt;
			testpt = &test;

			flash_erase(flash_dest, 32768);

			// write the 2 calibration readings of data
			for (i = 0; i < 1; i++) {
				if (HAL_FLASH_Program(TYPEPROGRAM_WORD, flash_dest, test) != HAL_OK) {
					// error during write process
					HAL_FLASH_Lock();
					while(1) {
						printInt(21);
						printNL();
					}
				}
				// 8 is for eight bytes in the float
				flash_dest+=8;
			}

			// lock the flash
			HAL_FLASH_Lock();

		}

		else if (mode == MODE3) {
			uint32_t *data = (uint32_t *)ADDR_FLASH_SECTOR_6;
			printFloat(*data);
			printNL();
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
