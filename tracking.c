#include "tracking.h"

bool hasRightWall(double value) {
	if (value < getRightWall()) return true;
	else return false;
}

bool hasLeftWall(double value) {
	if (value < getLeftWall()) return true;
	else return false;
}

bool hasFrontWall(double valueRight, double valueLeft) {
	if (valueRight < getIdealRightFront() && valueLeft < getIdealLeftFront()) return true;
	else return false;
}
