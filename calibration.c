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

/*=Private Functions==========================================================*/
static void setWall(Wall wall, double value);

/*=Public Functions===========================================================*/

void calibrateSensors(void) {
	printString("Entering calibrate sensors");
	printNL();
	setLED(BLUE);
//	waitForTop();
	while(!getButton()){
		toggleLED(BLUE);
		HAL_Delay(100);
	}
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
	printString("Entering calibrate Wall");
	printNL();
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
	printString("Entering toLinear");
	printNL();
	return (double)5000/log((double)input);
}

/*=Private Functions==========================================================*/
static void setWall(Wall wall, double value) {
	printString("Entering setWall");
	printNL();
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
