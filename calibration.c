/* File Name: calibration.c
 * Project: EE 396
 * Team: TSM
 * Members: Mark Miw
 *          Steven Chen
 *          Joseph Felix Jr.
 */

#include "calibration.h"

static double farLeftWall, farRightWall;
static double idealLeftFront, idealRightFront;
static double idealLeftCenter, idealRightCenter;
static double centerLeftFront, centerRightFront;

/*=Public Functions===========================================================*/

void testCalibration(void) {
	setLED(WHITE);
	setLED(BLUE);
	while(!getButton());
	HAL_Delay(500);
	calibrateWall(FRONTCENTER);
	resetLEDAll();
	turnRight();
	setLED(WHITE);
	setLED(GREEN);
	frontCorrection();
	resetLEDAll();
	turnRight();
	setLED(WHITE);
	setLED(RED);
	frontCorrection();
	turnRight();
	HAL_Delay(200);
	calibrateWall(CENTER);

	setWall(FARLEFTWALL, getWall(IDEALLEFTCENTER)+140);
	setWall(FARRIGHTWALL, getWall(IDEALRIGHTCENTER)+110);
	setWall(IDEALLEFTFRONT, getWall(IDEALLEFTCENTER)+110);
	setWall(IDEALRIGHTFRONT, getWall(IDEALRIGHTCENTER)+110);
}

void calibrateEncoders() {
	int rightEncoder, leftEncoder;
	int startR, startL;
	resetEncoder(LEFTENCODER);
	resetEncoder(RIGHTENCODER);

	startR = getEncoder(RIGHTENCODER);
	startL = getEncoder(LEFTENCODER);

	double right, left;

	while(1) {
		right = readSensor(RIGHT_DET);
		left = readSensor(LEFT_DET);
		if (hasFrontWall(right,left)) {
			frontCorrection();
			HAL_Delay(300);
			rightEncoder = getEncoder(RIGHTENCODER);
			leftEncoder = getEncoder(LEFTENCODER);
			while(!getButton()) {
				HAL_Delay(200);
				toggleLED(WHITE);
			}
			printInt(startR);
			printComma();
			printInt(rightEncoder);
			printNL();
			printInt(startL);
			printComma();
			printInt(leftEncoder);
			printNL();
			break;
		}
		else {
			correction2(50);
		}
	}
}

void calibrateSensors(void) {
	setLED(BLUE);
//	waitForTop();
	while(!getButton());
	calibrateWall(LEFT);
	resetLED(BLUE);
	resetLED(WHITE);
	HAL_Delay(1000);

	setLED(GREEN);
//	waitForTop();
	while(!getButton());
	calibrateWall(RIGHT);
	resetLED(GREEN);
	resetLED(WHITE);
	HAL_Delay(1000);

	setLED(RED);
//	waitForTop();
	while(!getButton());
	calibrateWall(CENTER);
	resetLED(RED);
	resetLED(WHITE);
	HAL_Delay(1000);

	setLEDAll();
//	waitForTop();
	while(!getButton());
	calibrateWall(FRONT);
	resetLEDAll();

	HAL_Delay(1000);

	setLED(RED);
	setLED(GREEN);
	while(!getButton());
	calibrateWall(FRONTCENTER);
	resetLEDAll();
	HAL_Delay(1000);
}

void calibrateWall(Wall wall) {
    switch (wall) {
        case LEFT:
            setWall(FARLEFTWALL, readSensor(LEFT_CEN_DET));
            break;
        case RIGHT:
            setWall(FARRIGHTWALL, readSensor(RIGHT_CEN_DET));
            break;
        case CENTER:
            setWall(IDEALLEFTCENTER, readSensor(LEFT_CEN_DET));
            setWall(IDEALRIGHTCENTER, readSensor(RIGHT_CEN_DET));
            break;
        case FRONT:
            setWall(IDEALLEFTFRONT, readSensor(LEFT_DET));
            setWall(IDEALRIGHTFRONT, readSensor(RIGHT_DET));
            break;
        case FRONTCENTER:
        	setWall(CENTERRIGHTFRONT, readSensor(RIGHT_DET));
        	setWall(CENTERLEFTFRONT, readSensor(LEFT_DET));
        	break;
            
        default:
            break;
    }
}

double getWall(Wall wall) {
    switch (wall) {
        case FARLEFTWALL:
            return farLeftWall;
            break;
        case FARRIGHTWALL:
            return farRightWall;
            break;
        case IDEALLEFTFRONT:
            return idealLeftFront;
            break;
        case IDEALRIGHTFRONT:
            return idealRightFront;
            break;
        case IDEALLEFTCENTER:
            return idealLeftCenter;
            break;
        case IDEALRIGHTCENTER:
            return idealRightCenter;
            break;
        case CENTERRIGHTFRONT:
        	return centerRightFront;
        	break;
        case CENTERLEFTFRONT:
        	return centerLeftFront;
        	break;
            
        default:
            return 0.0;
    }
}

double toLinear(uint16_t input) {
	return (double)5000/log((double)input);
}

/*=Private Functions==========================================================*/
void setWall(Wall wall, double value) {
    switch (wall) {
        case FARLEFTWALL:
            farLeftWall = value;
            break;
        case FARRIGHTWALL:
            farRightWall = value;
            break;
        case IDEALLEFTFRONT:
            idealLeftFront = value;
            break;
        case IDEALRIGHTFRONT:
            idealRightFront = value;
            break;
        case IDEALLEFTCENTER:
            idealLeftCenter = value;
            break;
        case IDEALRIGHTCENTER:
            idealRightCenter = value;
            break;
        case CENTERRIGHTFRONT:
        	centerRightFront = value;
        	break;
        case CENTERLEFTFRONT:
        	centerLeftFront = value;
        	break;
            
        default:
            break;
    }
}
