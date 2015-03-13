#include <stdint.h>

#include "adc.h"
#include "calibration.h"
#include "led.h"

static int leftWall, rightWall;
static int idealLeftFront, idealRightFront;
static int idealLeftCenter, idealRightCenter;

/*=Public Functions===========================================================*/
void waitForTop() {
	uint8_t startFlag = 0;
	do {
				if (readADC(FLASH) < 50) {
					startFlag = 1;
				}
				toggleLED(WHITE);
				HAL_Delay(500);
			} while (!startFlag);
	setLED(WHITE);
}

void calibrateSensors(void) {
	setLED(BLUE);
	waitForTop();
	calibrateLeftWall();
	resetLED(BLUE);
	resetLED(WHITE);
	HAL_Delay(1000);

	setLED(GREEN);
	waitForTop();
	calibrateRightWall();
	resetLED(GREEN);
	resetLED(WHITE);
	HAL_Delay(1000);

	setLED(RED);
	waitForTop();
	calibrateCenter();
	resetLED(RED);
	resetLED(WHITE);

	setLEDAll();
	waitForTop();
	calibrateFrontWall();
	resetLEDAll();

	HAL_Delay(1000);
}

void calibrateLeftWall(void) {
	setLeftWall(readLeftCenterSensor());
}

void calibrateRightWall(void) {
	setRightWall(readRightCenterSensor());
}

void calibrateFrontWall(void) {
	setIdealLeftFront(readLeftSensor());
	setIdealRightFront(readRightSensor());
}

void calibrateCenter(void) {
	setIdealLeftCenter(readLeftCenterSensor());
	setIdealRightCenter(readRightCenterSensor());
}

int getLeftWall(void) {
	return leftWall;
}

int getRightWall(void) {
	return rightWall;
}

int getIdealLeftFront(void) {
	return idealLeftFront;
}

int getIdealRightFront(void) {
	return idealRightFront;
}

int getIdealLeftCenter(void) {
	return idealLeftCenter;
}

int getIdealRightCenter(void) {
	return idealRightCenter;
}

/*=Private Functions==========================================================*/
static void setLeftWall(int value) {
	leftWall  = value;
}

static void setRightWall(int value) {
	rightWall = value;
}

static void setIdealLeftFront(int value) {
	idealLeftFront = value;
}

static void setIdealRightFront(int value) {
	idealRightFront = value;
}

static void setIdealLeftCenter(int value) {
	idealLeftCenter = value;
}

static void setIdealRightCenter(int value) {
	idealRightCenter = value;
}