#include "calibration.h"

static double farLeftWall, farRightWall;
static double idealLeftFront, idealRightFront;
static double idealLeftCenter, idealRightCenter;

/*=Public Functions===========================================================*/

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
	setFarLeftWall(readLeftCenterSensor());
}

void calibrateRightWall(void) {
	setFarRightWall(readRightCenterSensor());
}

void calibrateFrontWall(void) {
	setIdealLeftFront(readLeftSensor());
	setIdealRightFront(readRightSensor());
}

void calibrateCenter(void) {
	setIdealLeftCenter(readLeftCenterSensor());
	setIdealRightCenter(readRightCenterSensor());
}

double getFarLeftWall(void) {
	return farLeftWall;
}

double getFarRightWall(void) {
	return farRightWall;
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
static void setFarLeftWall(double value) {
	farLeftWall  = value;
}

static void setFarRightWall(double value) {
	farRightWall = value;
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
