#include <stdint.h>
#include "adc.h"
#include "motor.h"
#include "led.h"
#include <math.h>

/* Nice P values
 * PR = 0.01
 * PL = 0.02
 *
 * Good for Speed 100
 * DR = 0.1
 * DL = 0.1
 *
 * Decent for Speed 200
 * PR = 0.0037
 * PL = 0.0077
 * DR = 0.001
 * DL = 0.001
 *
 * Decent
 * .007
 */

#define PR	.01
#define PL	.01
#define DR	.005
#define DL	.005
#define BR	.0003
#define BL	.0005

double oldErrorP;

void PID(int leftBaseSpeed, int rightBaseSpeed) {

	double errorP, errorD;
	double totalErrorR;
	double totalErrorL;

	int rightCenterSensor = readRightCenterSensor();
	int leftCenterSensor = readLeftCenterSensor();
	int rightSensor = readRightSensor();
	int leftSensor = readLeftSensor();

	float rightFactor = 1;
	float leftFactor = 1;

	int idealRight = getIdealRightFront();
	int idealLeft = getIdealLeftFront();

	/* Determine if brakes are needed */
	if ((rightSensor > idealRight) && (leftSensor > idealLeft)) {
		rightFactor = (idealRight - rightSensor)*BR;
		leftFactor = (idealLeft - leftSensor)*BL;
		totalErrorL = 0;
		totalErrorR = 0;
		setLED(BLUE);
	}
	else {
		resetLED(BLUE);
		/* Has both right & left walls */
		if( (leftCenterSensor > getLeftWall()) && (rightCenterSensor > getRightWall()) )
		{
			/* Take the difference between the distances of right & left walls minus the ideal offset from calibration */
			errorP = rightCenterSensor - leftCenterSensor - (getIdealRightCenter() - getIdealLeftCenter());
			errorD = errorP - oldErrorP;
		}
		/* Has only left wall */
		else if( (leftCenterSensor > getLeftWall()) )
		{
			errorP = 2 * (getIdealLeftCenter() - leftCenterSensor);
			errorD = errorP - oldErrorP;
		}
		/* Has only right wall */
		else if( (rightCenterSensor > getRightWall()) )
		{
			errorP = 2 * (rightCenterSensor - getIdealRightCenter());
			errorD = errorP - oldErrorP;
		}
		/* Has no walls... good luck */
		else if( (leftCenterSensor < getLeftWall() && rightCenterSensor < getRightWall()) )
		{
			errorP = oldErrorP;//(leftEncoder – rightEncoder*1005/1000)*3;
			errorD = 0;
		}
		totalErrorL = PL * errorP + DL * errorD;
		totalErrorR = PR * errorP + DR * errorD;
		oldErrorP = errorP;
	}
	setSpeed(LEFTMOTOR, (int)((leftBaseSpeed - totalErrorL)*leftFactor));
	setSpeed(RIGHTMOTOR, (int)((rightBaseSpeed + totalErrorR)*rightFactor));
}

void PIDleft(int leftBaseSpeed, int rightBaseSpeed) {

	double errorP, errorD;
	double totalErrorL;

	int leftCenterSensor = readLeftCenterSensor();

	float rightFactor = 1;
	float leftFactor = 1;

	if( (leftCenterSensor > getLeftWall()) )
	{
		errorP = 2 * (leftCenterSensor - getIdealLeftCenter());
		errorD = errorP - oldErrorP;
	}
	/* Has no walls... good luck */
	else
	{
		errorP = oldErrorP;//(leftEncoder – rightEncoder*1005/1000)*3;
		errorD = 0;
	}
	totalErrorL = PL * errorP + DL * errorD;
	oldErrorP = errorP;

	setSpeed(LEFTMOTOR, leftBaseSpeed);
	setSpeed(RIGHTMOTOR, (int)((leftBaseSpeed - totalErrorL)*leftFactor));
}
