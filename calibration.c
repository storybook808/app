#include <stdint.h>

#include "adc.h"
#include "calibration.h"

static double leftWall, rightWall;
static double idealLeftFront, idealRightFront;
static double idealLeftCenter, idealRightCenter;

/*=Public Functions===========================================================*/
void calibrateLeftWall(void) {
	setLeftWall(leftCenterSensorDistance(readLeftCenterSensor()));
}

void calibrateRightWall(void) {
	setRightWall(rightCenterSensorDistance(readRightCenterSensor()));
}

void calibrateFrontWall(void) {
	setIdealLeftFront(leftSensorDistance(readLeftSensor()));
	setIdealRightFront(rightSensorDistance(readRightSensor()));
}

void calibrateCenter(void) {
	setIdealLeftCenter(leftCenterSensorDistance(readLeftCenterSensor()));
	setIdealRightCenter(rightCenterSensorDistance(readRightCenterSensor()));
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
