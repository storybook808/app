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

void moveForward(int cells, Wall_Correction wall) {
	for (x = 0; x < cells; x++) {

		startL = getEncoder(LEFTENCODER);
		startR = getEncoder(RIGHTENCODER);
		resetLEDAll();

		while(1) {
			left_front_sensor = readSensor(LEFT_DET);
			right_front_sensor = readSensor(RIGHT_DET);

			if (right_front_sensor <= getWall(IDEALRIGHTFRONT) && left_front_sensor <= getWall(IDEALLEFTFRONT)) {
				hardBrake();
				frontCorrection();
				break;
			}

			correction(wall, base_speed);

			if (getEncoder(LEFTENCODER) > startL + 4100) {
				brake();
				break;
			}
		}
	}
}
