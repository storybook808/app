#include <stdint.h>
#include "adc.h"
#include "motor.h"
#include "led.h"
#include <math.h>
#include "calibration.h"

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

void correction() {

	double errorP, errorD;
	double totalErrorR;
	double totalErrorL;

	double rightCenterSensor = toLinear(readRightCenterSensor());
	double leftCenterSensor = toLinear(readLeftCenterSensor());
	double rightSensor = toLinear(readRightSensor());
	double leftSensor = toLinear(readLeftSensor());

	int currRvel = getCurrentVelocity(RIGHTMOTOR);
	int currLvel = getCurrentVelocity(LEFTMOTOR);

	double rightFactor = 1;
	double leftFactor = 1;

	double idealRight = toLinear(getIdealRightFront());
	double idealLeft = toLinear(getIdealLeftFront());

	/* Determine if brakes are needed */
	if ((rightSensor < idealRight) && (leftSensor < idealLeft)) {
		rightFactor = (idealRight - rightSensor)*BR;
		leftFactor = (idealLeft - leftSensor)*BL;
		totalErrorL = 0;
		totalErrorR = 0;
		setLED(BLUE);
	}
	else {
		resetLED(BLUE);
		/* Has both right & left walls */
		if( (leftCenterSensor < toLinear(getLeftWall()) && (rightCenterSensor < toLinear(getRightWall())) ))
		{
			/* Take the difference between the distances of right & left walls minus the ideal offset from calibration */
			errorP = rightCenterSensor - leftCenterSensor - (toLinear(getIdealRightCenter()) - toLinear(getIdealLeftCenter()));
			errorD = errorP - oldErrorP;
		}
		/* Has only left wall */
		else if( (leftCenterSensor < toLinear(getLeftWall())) )
		{
			errorP = 2 * (leftCenterSensor - toLinear(getIdealLeftCenter()));
			errorD = errorP - oldErrorP;
		}
		/* Has only right wall */
		else if( (rightCenterSensor < toLinear(getRightWall())) )
		{
			errorP = 2 * (toLinear(getIdealRightCenter()) - rightCenterSensor);
			errorD = errorP - oldErrorP;
		}
		/* Has no walls... good luck */
		else if( (leftCenterSensor > toLinear(getLeftWall()) && rightCenterSensor > toLinear(getRightWall())) )
		{
			errorP = oldErrorP;//(leftEncoder – rightEncoder*1005/1000)*3;
			errorD = 0;
		}
		totalErrorL = PL * errorP + DL * errorD;
		totalErrorR = PR * errorP + DR * errorD;
		oldErrorP = errorP;
	}

	setRightVelocity(currRvel-totalErrorR);
	setLeftVelocity(currLvel-totalErrorL);
}
