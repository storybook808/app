/* File Name: motor.c
 * Project: EE 396
 * Team: TSM
 * Members: Mark Miw
 *          Steven Chen
 *          Joseph Felix Jr.
 */

#include "mapping.h"

void emptyMap() {
	int i,j;
	for (i = 0; i < 16; ++i) {
		for (j = 0; j < 16; ++j) {
			map[i][j].south = false;
			map[i][j].west = false;
			map[i][j].mapped = false;
		}
	}

	for (i = 0; i < 16; ++i) {
		map[0][i].west = true;
		map[i][0].south = true;
	}
	map[0][0].mapped = true;
	map[0][0].south = true;
	map[0][0].west = true;
	map[1][0].west = true;
}

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

void setSideWallsForCell(uint8_t x, uint8_t y, cell here) {
	// If cell is not mapped
//	if(!map[x][y].mapped) {
		// Map Cell
		map[x][y].south = here.south;
		map[x][y].west = here.west;
		if (x != 15) map[x+1][y].west = here.east;
		if (y != 15) map[x][y+1].south = here.north;
		map[x][y].mapped = true;
//	}
//	// Else
//	else {
//		cell test = getWallsForCell(x,y);
//		// Verify cell
//		if (here.south != test.south) {
//			setBuzzerTone(C7);
//			playBuzzer(50,0);
//		}
//		if (here.north != test.north) {
//			setBuzzer(C8);
//			playBuzzer(50,0);
//		}
//		if (here.east != test.east) {
//			setBuzzer(G7);
//			playBuzzer(50,0);
//		}
//		if (here.west != test.west) {
//			setBuzzer(G8);
//			playBuzzer(50,0);
//		}
//	}
}

void mapSideWalls(uint8_t x, uint8_t y, cell here) {
	switch (dir) {
		case NORTH:
			map[x][y].west = here.west;
			if (x !=15) map[x+1][y].west = here.east;
			break;
		case EAST:
			if (y != 15) map[x][y+1].south = here.north;
			map[x][y].south = here.south;
			break;
		case SOUTH:
			map[x][y].west = here.west;
			if (x !=15) map[x+1][y].west = here.east;
			break;
		case WEST:
			if (y != 15) map[x][y+1].south = here.north;
			map[x][y].south = here.south;
			break;
	}
}

void mapFrontWall(uint8_t x, uint8_t y, bool here) {
	switch (dir) {
		case NORTH:
			if (y != 15) map[x][y+1].south = here;
			break;
		case EAST:
			if (x != 15) map[x+1][y].west = here;
			break;
		case SOUTH:
			map[x][y].south = here;
			break;
		case WEST:
			map[x][y].west = here;
			break;
	}
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

void printMap() {
	int i,j;
	for (j = 15; j >= 0; --j) {
		for (i = 0; i < 16; ++i) {
//			printString("Cell:[");
//			printInt(i);
//			printComma();
//			printInt(j);
//			printString("]: ");
			if (map[i][j].west) {
				printString("|");
			}
			else printString(" ");
			if (map[i][j].south) {
				printString("_");
			}
			else printString(" ");
		}
		printNL();
	}
}
