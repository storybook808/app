#include "interface.h"

extern TIM_HandleTypeDef buzzerHandler;

void setBuzzer(int state) {
	if (state) HAL_TIM_Base_Start_IT(&buzzerHandler);
	else HAL_TIM_Base_Stop_IT(&buzzerHandler);
}

void waitForTop() {
	uint8_t startFlag = 0;
	do {
		if (readADC(FLASH_MEM) < 50) {
			startFlag = 1;
		}
		toggleLED(WHITE);
		HAL_Delay(500);
	} while (!startFlag);
	setLED(WHITE);
}

void setLED(Led color) {
	switch (color) {
	case WHITE:
		HAL_GPIO_WritePin(GPIOB, GPIO_PIN_11, GPIO_PIN_SET);
		break;
	case BLUE:
		HAL_GPIO_WritePin(GPIOB, GPIO_PIN_10, GPIO_PIN_SET);
		break;
	case GREEN:
		HAL_GPIO_WritePin(GPIOA, GPIO_PIN_5, GPIO_PIN_SET);
		break;
	case RED:
		HAL_GPIO_WritePin(GPIOA, GPIO_PIN_6, GPIO_PIN_SET);
		break;
	}
}

void resetLED(Led color) {
	switch (color) {
	case WHITE:
		HAL_GPIO_WritePin(GPIOB, GPIO_PIN_11, GPIO_PIN_RESET);
		break;
	case BLUE:
		HAL_GPIO_WritePin(GPIOB, GPIO_PIN_10, GPIO_PIN_RESET);
		break;
	case GREEN:
		HAL_GPIO_WritePin(GPIOA, GPIO_PIN_5, GPIO_PIN_RESET);
		break;
	case RED:
		HAL_GPIO_WritePin(GPIOA, GPIO_PIN_6, GPIO_PIN_RESET);
		break;
	}
}

//Control the state of all 4 LED at once
void setLEDAll(void) {
	setLED(WHITE);
	setLED(BLUE);
	setLED(GREEN);
	setLED(RED);
}

void resetLEDAll(void) {
	resetLED(WHITE);
	resetLED(BLUE);
	resetLED(GREEN);
	resetLED(RED);
}

void toggleLED(Led color) {
	switch (color) {
	case WHITE:
		HAL_GPIO_TogglePin(GPIOB, GPIO_PIN_11);
		break;
	case BLUE:
		HAL_GPIO_TogglePin(GPIOB, GPIO_PIN_10);
		break;
	case GREEN:
		HAL_GPIO_TogglePin(GPIOA, GPIO_PIN_5);
		break;
	case RED:
		HAL_GPIO_TogglePin(GPIOA, GPIO_PIN_6);
		break;
	}
}

//Toggle the state of all 4 LED at once
void toggleLEDAll(void) {
	toggleLED(WHITE);
	toggleLED(BLUE);
	toggleLED(GREEN);
	toggleLED(RED);
}
