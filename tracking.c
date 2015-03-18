#include "adc.h"
#include "calibration.h"
#include "tracking.h"
#include <stdint.h>

uint8_t hasRightWall(double value) {
	if (value < getRightWall()) return TRUE;
	else return FALSE;
}

uint8_t hasLeftWall(double value) {
	if (value < getLeftWall()) return TRUE;
	else return FALSE;
}

uint8_t hasFrontWall(double valueRight, double valueLeft) {
	if (valueRight < getIdealRightFront() && valueLeft < getIdealLeftFront()) return TRUE;
	else return FALSE;
}
