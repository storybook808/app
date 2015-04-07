/* File Name: tracking.h
 * Project: EE 396
 * Team: TSM
 * Members: Mark Miw
 *          Steven Chen
 *          Joseph Felix Jr.
 */

#ifndef tracking_H
#define tracking_H

#include "global_libs.h"
#include "HAL_includes.h"
#include "defines.h"
#include "calibration.h"
#include "correction.h"

bool hasRightWall(double value);

bool hasLeftWall(double value);

bool hasFrontWall(double valueRight, double valueLeft);

void moveForward(int cells, Wall_Correction wall, double base_speed);

void moveCells(int num, double base_speed);

void turnLeft();
void turnLeft2();
void turnLeft3();

void turnRight();
void turnRight2();
void turnRight3();

#endif
