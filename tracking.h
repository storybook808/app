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
#include "mapping.h"

bool brb;
int x, y, dir;

bool hasRightWall(double value);

bool hasLeftWall(double value);

bool hasFrontWall(double valueRight, double valueLeft);

bool hasFarFrontWall(double valueRight, double valueLeft);

void moveForward(int cells, Wall_Correction wall, double base_speed);

void moveCells(int num, double base_speed);

void move(int cells, double base_speed);

void turnLeft();
void turnLeft2();
void turnLeft3();

void turnRight();
void turnRight2();
void turnRight3();

void curveTurn(Wall turn, double base_speed);

void brakeInCell(double base_speed);

bool startCellStop();

void searchSlow();

void mapSlow();

void floodSlow(double base_speed);
void floodSlow2();

void floodSearch();

void resetPath(Path *path);
void addPathMove(Path *path, Move move);
uint8_t getDirFromFloodValues(Coordinate here);
void updateCoordinate(Coordinate *here, uint8_t direction);
void updateDirection(uint8_t *direction, Move turn);
Path findShortestPath();
void printPath(Path *moves);

void speed1(double base_speed);

#endif
