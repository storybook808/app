/* File Name: motor.c
 * Project: EE 396
 * Team: TSM
 * Members: Mark Miw
 *          Steven Chen
 *          Joseph Felix Jr.
 */

#include "mapping.h"

cell getWallsForCell(uint8_t x, uint8_t y) {
    cell request;

    // Grab South wall
    request.south = map[x][y].south;

    // Grab West wall
    request.west = map[x][y].west;

    // Check if cell is on boundaries for north and east walls
    // If cell is far East cell
    if (x == 15) {
        request.east = true;
    }
    else {
        request.east = map[x+1][y].west;
    }
    // If cell is far North cell
    if (y == 15) {
        request.north = true;
    }
    else {
        request.north = map[x][y+1].south;
    }

    return request;
}

bool convertWallsToRows() {
    int i,j;
    bool result = true;

    // Clear all rows to blank
    for (i = 0; i <=15 ; i++) {
        row[i] = 0;
    }

    // Traverse through all 16 by 16 cells
    for (j = 0; j <= 15; j++) {
        for (i = 0; i <= 15; i++) {
            // Shift row bits to the left
            row[j]<<=1;
            // Place wall data on row
            row[j] = row[j] | map[i][j].south;
            // Shift row bits to the left
            row[j]<<=1;
            // Place wall data on row
            row[j] = row[j] | map[i][j].west;
        }
    }

    if (j == 15 && i == 15) {
        result = false;
    }

    return result;
}

bool convertRowsToWalls() {
    int i,j;
    bool result = false;

    loadRows();

    for (j = 15; j >= 0; j--) {
        for (i = 15; i >= 0; i--) {
            map[i][j].west = row[j] & 1;
            row[j]>>=1;
            map[i][j].south = row[j]&1;
            row[j]>>=1;
        }
    }

    if (j == 0 && i == 0) {
        result = true;
    }

    return result;
}

bool saveMap() {
	if (convertWallsToRows()) {
		return false;
	}

	if (eraseMap()) {
		return false;
	}

	if (writeRows()) {
		return false;
	}
	return true;
}
