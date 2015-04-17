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
void calibrateWall(Wall wall);
void testCalibration();
void setWall(Wall wall, float value);

/* Rename getFarWall functions */
float getWall(Wall wall);
float toLinear(uint16_t input);

#endif
