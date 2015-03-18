/* File Name: calibration.h
 * Project: EE 396
 * Team: TSM
 * Members: Mark Miw
 *          Steven Chen
 *          Joseph Felix Jr.
 */

#ifndef calibration_H
#define calibration_H

#include "global_libs.h"
#include "HAL_includes.h"
#include "defines.h"

#include "adc.h"
#include "interface.h"

/*=Public Functions===========================================================*/

void calibrateSensors(void);
void calibrateLeftWall(void);
void calibrateRightWall(void);
void calibrateFrontWall(void);
void calibrateCenter(void);

/* Rename getFarWall functions */
double getFarLeftWall(void);
double getFarRightWall(void);
double getIdealLeftFront(void);
double getIdealRightFront(void);
double getIdealLeftCenter(void);
double getIdealRightCenter(void);
double toLinear(uint16_t input);

#endif
