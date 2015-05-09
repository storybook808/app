/* File Name: interface.c
 * Project: EE 396
 * Team: TSM
 * Members: Mark Miw
 *          Steven Chen
 *          Joseph Felix Jr.
 */

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

void testChaser(int mode, int period) {
 	switch (mode) {
 	case 0:
 		setLED(WHITE);
 		HAL_Delay(period);
 		setLED(BLUE);
 		HAL_Delay(period);
 		setLED(GREEN);
 		HAL_Delay(period);
 		setLED(RED);
 		HAL_Delay(period);
 		break;
 	case 1:
 		setLED(WHITE);
 		HAL_Delay(period);
 		setLED(BLUE);
 		HAL_Delay(period);
 		setLED(GREEN);
 		HAL_Delay(period);
 		setLED(RED);
 		HAL_Delay(period);
 		toggleLEDAll();
 		HAL_Delay(period);
 		toggleLEDAll();
 		HAL_Delay(period);
 		toggleLEDAll();
 		HAL_Delay(period);
 		toggleLEDAll();
 		HAL_Delay(period);
 		toggleLEDAll();
 		break;
 	case 2:
 		setLED(WHITE);
 		HAL_Delay(period);
 		resetLED(WHITE);
 		setLED(BLUE);
 		HAL_Delay(period);
 		resetLED(BLUE);
 		setLED(GREEN);
 		HAL_Delay(period);
 		resetLED(GREEN);
 		setLED(RED);
 		HAL_Delay(period);
 		break;
 	}
 }

Mode menu() {
	// Mode for Menu
	int mode = 0;

	// Size for Encoder ticks to move through Menu
	int size = 400;

	// Variable to place Encoder value
	int count;
	double rightSensor;
	double thresh = 700;

	resetLEDAll();

	while(1) {
		rightSensor = readSensor(RIGHT_CEN_DET);
		count = getEncoder(RIGHTENCODER);
		count = count%(size*7);


		if (count >= 0 && count <= size) {
			resetLEDAll();
			setLED(WHITE);
//			resetLED(RED);
//			resetLED(BLUE);
			mode = MODE1;
		} else if (count > size && count <= (size*2)) {
			resetLEDAll();
			setLED(BLUE);
//			resetLED(WHITE);
//			resetLED(GREEN);
			mode = MODE2;
		} else if (count > (size*2) && count <= (size*3)) {
			resetLEDAll();
			setLED(GREEN);
//			resetLED(BLUE);
//			resetLED(RED);
			mode = MODE3;
		} else if (count > (size*3) && count <= (size*4)) {
			resetLEDAll();
			setLED(RED);
//			resetLED(WHITE);
//			resetLED(GREEN);
			mode = MODE4;
		} else if (count > (size*4) && count <= (size*5)) {
			setLEDAll();
//			resetLED(WHITE);
//			resetLED(GREEN);
			mode = MODE5;
		} else if (count > (size*5) && count <= (size*6)) {
			testChaser(1,250);
			setBuzzerTone(E8);
			playBuzzer(100,50);
			setBuzzerTone(E8);
			playBuzzer(100,50);
			setBuzzerTone(E8);
			playBuzzer(300,50);
			setBuzzerTone(C7);
			playBuzzer(100,50);
			setBuzzerTone(E8);
			playBuzzer(100,50);
			setBuzzerTone(G8);
			playBuzzer(200,50);
			setBuzzerTone(G7);
			playBuzzer(200,50);
			mode = MODE6;
		} else if (count > (size*6) && count <= (size*7)) {
			resetLEDAll();
			setLED(WHITE);
			HAL_Delay(100);
			setLED(BLUE);
			HAL_Delay(100);
			resetLED(WHITE);
			setLED(GREEN);
			HAL_Delay(100);
			resetLED(BLUE);
			setLED(RED);
			HAL_Delay(100);
			resetLED(GREEN);
			mode = MODE7;
		}

		if (rightSensor <= thresh) {
			if (confirm(mode)) {
				buzzerConfirm();
				return mode;
			}
			buzzerCancel();
		}
		HAL_Delay(100);
	}

	return mode;
}

void playBuzzer(int duration, int pause) {
	setLED(BLUE);
	setBuzzer(1);
	HAL_Delay(duration);
	setBuzzer(0);
	resetLED(BLUE);
	HAL_Delay(pause);
}

void buzzerConfirm() {
	setBuzzerTone(C8);
	playBuzzer(100,0);
	setBuzzerTone(C8);
	playBuzzer(100,0);
	setBuzzerTone(C8);
	playBuzzer(100,0);
	setBuzzerTone(C8);
	playBuzzer(300,0);
	setBuzzerTone(G7);
	playBuzzer(300,0);
	setBuzzerTone(A7);
	playBuzzer(300,0);
	setBuzzerTone(C8);
	playBuzzer(200,0);
	setBuzzerTone(A7);
	playBuzzer(100,0);
	setBuzzerTone(C8);
	playBuzzer(300,0);
}

void buzzerCancel() {
	setBuzzerTone(C9);
	playBuzzer(100,100);
	setBuzzerTone(C7);
	playBuzzer(100,100);
}

bool confirm(Mode mode) {
	double rightSensor;
	double thresh = 700;
	Led color;
	int cancel;
	resetEncoder(LEFTENCODER);

	switch (mode) {
		case MODE1:
			color = WHITE;
			break;
		case MODE2:
			color = BLUE;
			break;
		case MODE3:
			color = GREEN;
			break;
		case MODE4:
			color = RED;
			break;
		default:
			break;
	}

	while(1) {
		cancel = getEncoder(LEFTENCODER);
		rightSensor = readSensor(RIGHT_CEN_DET);

		if (rightSensor <= thresh) {
			return true;
		}
		if (cancel <= -400) {
			return false;
		}
		toggleLED(color);
		HAL_Delay(100);
	}

}


void testMenu(int channel) {
	int size = 400;
	int count = getEncoder(channel);
	if (count < 0) {
		count = 0;
	} else if (count >= 0 && count <= size) {
		setLED(RED);
		resetLED(GREEN);
	} else if (count > size && count <= (size*2)) {
		resetLED(RED);
		setLED(GREEN);
		resetLED(BLUE);
	} else if (count > (size*2) && count <= (size*3)) {
		resetLED(GREEN);
		setLED(BLUE);
		resetLED(WHITE);
	} else if (count > (size*3) && count <= (size*4)) {
		resetLED(BLUE);
		setLED(WHITE);
	} else if (count > (size*4)) {
		count = (size*4);
	}
}

void batteryFault() {
 	//Take a reading from the voltage detector
 	uint32_t batteryLevel = readBattery();
 	//Check to see if voltage level is above 7V
 	//Voltage detector is a voltage divider where 7V is measured as 2.3333V
 	//2.3333V translate to roughly 2333 from the 12 bit ADC
 	if (batteryLevel < 2666) {
 		//Disable all motors
// 		brake();
 		//Enable buzzer
 		setBuzzer(ON);
 		//Disable all LEDs
// 		resetLEDAll();
// 		//Disable Motors
// 		HAL_TIM_Base_Stop_IT(&htim2);
// 		setSpeed(RIGHTMOTOR,0);
// 		setSpeed(LEFTMOTOR,0);
// 		//Flash red LED every half second.
// 		while (1) {
// 			//Invert the state of the red LED located closest to the STM
// 			toggleLED(RED);
// 			//Half second delay
// 			HAL_Delay(500);
// 		}
 	}
 }

bool getButton(void) {
	if (HAL_GPIO_ReadPin(GPIOC, GPIO_PIN_13) == GPIO_PIN_SET) return true;
	else return false;
}
