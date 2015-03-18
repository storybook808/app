#include <stdint.h>
#include <math.h>

#include "adc.h"
#include "calibration.h"
#include "led.h"

static double leftWall, rightWall;
static double idealLeftFront, idealRightFront;
static double idealLeftCenter, idealRightCenter;

/*=Public Functions===========================================================*/
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
	HAL_Delay(1000);

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

double getLeftWall(void) {
	return leftWall;
}

double getRightWall(void) {
	return rightWall;
}

double getIdealLeftFront(void) {
	return idealLeftFront;
}

double getIdealRightFront(void) {
	return idealRightFront;
}

double getIdealLeftCenter(void) {
	return idealLeftCenter;
}

double getIdealRightCenter(void) {
	return idealRightCenter;
}

double toLinear(uint16_t input) {
	return (double)5000/log((double)input);
}

/*=Private Functions==========================================================*/
static void setLeftWall(double value) {
	leftWall  = value;
}

static void setRightWall(double value) {
	rightWall = value;
}

static void setIdealLeftFront(double value) {
	idealLeftFront = value;
}

static void setIdealRightFront(double value) {
	idealRightFront = value;
}

static void setIdealLeftCenter(double value) {
	idealLeftCenter = value;
}

static void setIdealRightCenter(double value) {
	idealRightCenter = value;
}
