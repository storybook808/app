/* File Name: system.h
 * Project: EE 396
 * Team: TSM
 * Members: Mark Miw
 *          Steven Chen
 *          Joseph Felix Jr.
 */

#ifndef mapping_H
#define mapping_H

#include "global_libs.h"
#include "HAL_includes.h"
#include "defines.h"
#include "flash.h"
#include "usart.h"

void emptyMap();

cell getWallsForCell(uint8_t x, uint8_t y);

void setWallsForCell(uint8_t x, uint8_t y, cell here);

void mapSideWalls(uint8_t x, uint8_t y, cell here);

void mapFrontWall(uint8_t x, uint8_t y, bool here);

bool convertWallsToRows();

bool convertRowsToWalls();

void printMap();

bool saveMap();
void loadMap();

void printFlood();

void flood1();
void flood2();

void floodCell(coordinate cell, uint8_t dir);
void floodCellTurn(coordinate cell, uint8_t dir);

void resetStack(floodStack *stack);

void pushStack(floodStack *stack, uint8_t i, uint8_t j);

coordinate setCoordinate(uint8_t i, uint8_t j);

float getFloodValue(coordinate cell, uint8_t dir);

#endif
