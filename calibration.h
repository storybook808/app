#ifndef calibration_H
#define calibration_H

#include "global_libs.h"
#include "HAL_includes.h"
#include "defines.h"

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

/*=Private Functions==========================================================*/
static void setFarLeftWall(double value);
static void setFarRightWall(double value);
static void setIdealLeftFront(double value);
static void setIdealRightFront(double value);
static void setIdealLeftCenter(double value);
static void setIdealRightCenter(double value);

#endif
