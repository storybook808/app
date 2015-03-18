#include "correction.h"

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

double lastErrorR, lastErrorL;

void correction() {

	double errorR, errorL;
	double currentRightSide = readRightCenterSensor();
	double currentLeftSide = readLeftCenterSensor();
	double currentFrontRight = readRightSensor();
	double currentFrontLeft = readLeftSensor();

	if (hasFrontWall(currentFrontRight, currentFrontLeft)) {
		brake();
	}
	else {
		/* If there are right & left walls */
		if (hasRightWall(currentRightSide) && hasLeftWall(currentLeftSide)) {
			if (getIdealRightCenter() > currentRightSide) {
				errorR = getIdealRightCenter() - currentRightSide;
				setRightVelocity(getTargetVelocity(RIGHTMOTOR) + errorR);
			}
			else if (getIdealLeftCenter() > currentLeftSide) {
				errorL = getIdealLeftCenter() - currentLeftSide;
				setRightVelocity(getTargetVelocity(RIGHTMOTOR) - errorL);
			}
		}
		/* If there is only Right Wall */
		else if (hasRightWall(currentRightSide) && !hasLeftWall(currentLeftSide)) {
			brake();
			setLED(RED);
		}
		/* If there is only Left Wall */
		else if (!hasRightWall(currentRightSide) && hasLeftWall(currentLeftSide)) {
			brake();
			setLED(GREEN);
		}
		/* If there are no Walls */
		else {
			brake();
			setLED(BLUE);
		}

		lastErrorR = errorR;
		lastErrorL = errorL;
	}
}
