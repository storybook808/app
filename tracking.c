/* File Name: tracking.c
 * Project: EE 396
 * Team: TSM
 * Members: Mark Miw
 *          Steven Chen
 *          Joseph Felix Jr.
 */

#include "tracking.h"


bool hasRightWall(double value) {
	if (value < getFarRightWall()) return true;
	else return false;
}

bool hasLeftWall(double value) {
	if (value < getFarLeftWall()) return true;
	else return false;
}

bool hasFrontWall(double valueRight, double valueLeft) {
	if (valueRight < getIdealRightFront() && valueLeft < getIdealLeftFront()) return true;
	else return false;
}
