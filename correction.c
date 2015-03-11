#include <stdint.h>
#include <adc.h>
#include <motor.h>

#define P	.1
#define D	0

double oldErrorP;

void PID(int leftBaseSpeed, int rightBaseSpeed) {

	double errorP, errorD;
	double totalError;

	double rightCenterSensor = rightCenterSensorDistance(readRightCenterSensor());
	double leftCenterSensor = leftCenterSensorDistance(readLeftCenterSensor());

	/* Has both right & left walls */
	if( (leftCenterSensor < getLeftWall()) && (rightCenterSensor < getRightWall()) )
	{
		/* Take the difference between the distances of right & left walls minus the ideal offset from calibration */
		errorP = rightCenterSensor - leftCenterSensor - (getIdealRightCenter() - getIdealLeftCenter());
		errorD = errorP - oldErrorP;
	}
	/* Has only left wall */
	else if( (leftCenterSensor < getLeftWall()) )
	{
		errorP = 2 * (getIdealLeftCenter() - leftCenterSensor);
		errorD = errorP - oldErrorP;
	}
	/* Has only right wall */
	else if( (rightCenterSensor < getRightWall()) )
	{
		errorP = 2 * (rightCenterSensor - getIdealRightCenter());
		errorD = errorP - oldErrorP;
	}
	/* Has no walls... good luck */
	else if( (leftCenterSensor > getLeftWall() && rightCenterSensor > getRightWall()) )
	{
		errorP = 0;//(leftEncoder – rightEncoder*1005/1000)*3;
		errorD = 0;
	}
	totalError = P * errorP + D * errorD;
	oldErrorP = errorP;
	setSpeed(LEFTMOTOR, leftBaseSpeed - (int)totalError);
	setSpeed(RIGHTMOTOR, rightBaseSpeed + (int)totalError);
}
