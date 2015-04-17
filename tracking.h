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

bool brb;
int x, y, dir;

bool hasRightWall(float value);

bool hasLeftWall(float value);

bool hasFrontWall(float valueRight, float valueLeft);

void moveForward(int cells, Wall_Correction wall, float base_speed);

void moveCells(int num, float base_speed);

void move(int cells, float base_speed);

void turnLeft();
void turnLeft2();
void turnLeft3();

void turnRight();
void turnRight2();
void turnRight3();

void brakeInCell(float base_speed);

void startCellStop();

void searchSlow();

#endif
