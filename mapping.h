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

bool convertWallsToRows();

bool convertRowsToWalls();

void printMap();

#endif
