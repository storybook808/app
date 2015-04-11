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

//	calibrateSensors();

	while(!getButton()) {
		batteryFault();
		toggleLED(WHITE);
		HAL_Delay(100);
	}

	resetLED(WHITE);

	testChaser(1,250);

	resetEncoder(LEFTENCODER);
	resetEncoder(RIGHTENCODER);

	// Test Flash Memory Variables
	// 0x08010000  == 64KB
	// 0x08040000  == 256KB
	// 0x08080000  == 512KB
	// 0x080C0000  == 768KB
	// 0x080E0000  == 896KB
	// 32768 words in sectors 5 through 11
	uint32_t startAddress = ADDR_FLASH_SECTOR_6;
	uint32_t sectorSize = 32768;
	uint32_t *data = (uint32_t *)ADDR_FLASH_SECTOR_6;
	int thing[5] = {1,2,3,4,5};
	int i;
	// Read Data
		printInt(*data);
		printComma();
		printInt(i);
		printNL();
	// Erase Data
		flash_erase(startAddress, sectorSize);
	// Read Data
		printInt(*data);
		printNL();
	// Write Data
		flash_write(startAddress, thing, sizeof(thing)/4);
	// Read Data
		printInt(*data);
		printNL();
	// Read Data
		data+=1;
		printInt(*data);
		printNL();
	// Read Data
	data+=1;
	printInt(*data);
	printNL();
	// Read Data
	data+=1;
	printInt(*data);
	printNL();
	// Read Data
	data+=1;
	printInt(*data);
	printNL();

	while(1) {
		while(!getButton()) {
			toggleLED(WHITE);
			HAL_Delay(100);
		}
	}
}
