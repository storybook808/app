/* File Name: tracking.c
 * Project: EE 396
 * Team: TSM
 * Members: Mark Miw
 *          Steven Chen
 *          Joseph Felix Jr.
 */

#include "tracking.h"


bool hasRightWall(double value) {
	if (value < getWall(FARRIGHTWALL)+20) return true;
	else return false;
}

bool hasLeftWall(double value) {
	if (value < getWall(FARLEFTWALL)+20) return true;
	else return false;
}

bool hasFrontWall(double valueRight, double valueLeft) {
	if ((valueRight < getWall(IDEALRIGHTFRONT)+20) && (valueLeft < getWall(IDEALLEFTCENTER)+20)) return true;
	else return false;
}
