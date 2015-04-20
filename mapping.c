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
			flood[i][j] = 0b11111111;
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
            if (map[i][j].south) {
				row[j] = row[j] + 1;
			}
            // Shift row bits to the left
            row[j]<<=1;
            // Place wall data on row
            if (map[i][j].west) {
            	row[j] = row[j] + 1;
			}
        }
    }

    if (j == 16 && i == 16) {
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
        	if ((row[j] & 1) == 1) {
				map[i][j].west = true;
			}
        	else {
				map[i][j].west = false;
			}
            row[j]>>=1;
            if ((row[j] & 1) == 1) {
				map[i][j].south = true;
			}
            else {
				map[i][j].south = false;
			}
            row[j]>>=1;
        }
    }

    if (j == -1 && i == -1) {
        result = true;
    }

    return result;
}

bool saveMap() {
	if (convertWallsToRows()) {
		playBuzzer(100,100);
		return true;
	}
	if (eraseMap()) {
		playBuzzer(100,100);
		playBuzzer(100,100);
		return true;
	}
	if (writeRows()) {
		playBuzzer(100,100);
		playBuzzer(100,100);
		playBuzzer(100,100);
		return true;
	}
	return false;
}

void loadMap() {
	if (!convertRowsToWalls()) {
		playBuzzer(1000,100);
	}
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
void printFlood() {
	int i,j,k;
	uint8_t value;
	for (j = 15; j >= 0; --j) {
		for (k = 0; k < 3; ++k) {
			for (i = 0; i < 16; ++i) {
				value = getFloodValue(setCoordinate(i,j),CURRENT);
				if(k == 0) {
					if (map[i][j].west) {
						printString("|   ");
					}
					else printString("    ");
				}
				if(k == 1) {
					if (map[i][j].west) {
						printString("|");
					}
					else printString(" ");
					if (value > 99) {
						printInt(value);
					}
					else if (value > 10) {
						printString(" ");
						printInt(value);
					}
					else {
						printString(" ");
						printInt(value);
						printString(" ");
					}
				}
				if(k == 2) {
					if (map[i][j].west) {
						printString("|");
					}
					else printString(" ");
					if (map[i][j].south) {
						printString("___");
					}
					else printString("   ");
				}
			}
			printNL();
		}
	}
}

void flood1() {
	flood[7][7] = 0;
	flood[7][8] = 0;
	flood[8][7] = 0;
	flood[8][8] = 0;

	cell currentCell;
	floodStack stack1;
	floodStack stack2;

	resetStack(&stack1);
	resetStack(&stack2);

	pushStack(&stack1,7,7);
	pushStack(&stack1,7,8);
	pushStack(&stack1,8,7);
	pushStack(&stack1,8,8);

	int size;
	int i,j;
	for (i = 1; i <= 256; ++i) {
		// Determine current stack
		if (i%2 == 1) {
			size = stack1.stackCount;
			for (j = 0; j < size; ++j) {
				currentCell = getWallsForCell(stack1.stack[j].x,stack1.stack[j].y);
				if (!currentCell.north) {
					if (getFloodValue(stack1.stack[j], NORTH) > getFloodValue(stack1.stack[j],CURRENT) + 1) {
						floodCell(stack1.stack[j],NORTH);
						pushStack(&stack2,stack1.stack[j].x,stack1.stack[j].y+1);
					}
				}
				if (!currentCell.east) {
					if (getFloodValue(stack1.stack[j], EAST) > getFloodValue(stack1.stack[j],CURRENT) + 1) {
						floodCell(stack1.stack[j],EAST);
						pushStack(&stack2,stack1.stack[j].x+1,stack1.stack[j].y);
					}
				}
				if (!currentCell.south) {
					if (getFloodValue(stack1.stack[j], SOUTH) > getFloodValue(stack1.stack[j],CURRENT) + 1) {
						floodCell(stack1.stack[j],SOUTH);
						pushStack(&stack2,stack1.stack[j].x,stack1.stack[j].y-1);
					}
				}
				if (!currentCell.west) {
					if (getFloodValue(stack1.stack[j], WEST) > getFloodValue(stack1.stack[j],CURRENT) + 1) {
						floodCell(stack1.stack[j],WEST);
						pushStack(&stack2,stack1.stack[j].x-1,stack1.stack[j].y);
					}
				}
			}
			resetStack(&stack1);
		}
		else {
			size = stack2.stackCount;
			for (j = 0; j < size; ++j) {
				currentCell = getWallsForCell(stack2.stack[j].x,stack2.stack[j].y);
				if (!currentCell.north) {
					if (getFloodValue(stack2.stack[j], NORTH) > getFloodValue(stack2.stack[j],CURRENT) + 1) {
						floodCell(stack2.stack[j],NORTH);
						pushStack(&stack1,stack2.stack[j].x,stack2.stack[j].y+1);
					}
				}
				if (!currentCell.east) {
					if (getFloodValue(stack2.stack[j], EAST) > getFloodValue(stack2.stack[j],CURRENT) + 1) {
						floodCell(stack2.stack[j],EAST);
						pushStack(&stack1,stack2.stack[j].x+1,stack2.stack[j].y);
					}
				}
				if (!currentCell.south) {
					if (getFloodValue(stack2.stack[j], SOUTH) > getFloodValue(stack2.stack[j],CURRENT) + 1) {
						floodCell(stack2.stack[j],SOUTH);
						pushStack(&stack1,stack2.stack[j].x,stack2.stack[j].y-1);
					}
				}
				if (!currentCell.west) {
					if (getFloodValue(stack2.stack[j], WEST) > getFloodValue(stack2.stack[j],CURRENT) + 1) {
						floodCell(stack2.stack[j],WEST);
						pushStack(&stack1,stack2.stack[j].x-1,stack2.stack[j].y);
					}
				}
			}
			resetStack(&stack2);
		}
	}
}

void floodCell(coordinate cell, uint8_t dir) {
	switch (dir) {
		case NORTH:
			flood[cell.x][cell.y+1] = getFloodValue(cell,CURRENT)+1;
		case EAST:
			flood[cell.x+1][cell.y] = getFloodValue(cell,CURRENT)+1;
		case SOUTH:
			flood[cell.x][cell.y-1] = getFloodValue(cell,CURRENT)+1;
		case WEST:
			flood[cell.x-1][cell.y] = getFloodValue(cell,CURRENT)+1;
		case CURRENT:
			flood[cell.x][cell.y] = getFloodValue(cell,CURRENT)+1;
		default:
			break;
	}
}

void resetStack(floodStack *stack) {
	stack->stackCount = 0;
}

void pushStack(floodStack *stack, uint8_t i, uint8_t j) {
	stack->stack[stack->stackCount] = setCoordinate(i,j);
	stack->stackCount++;
}

coordinate setCoordinate(uint8_t i, uint8_t j) {
	coordinate cell;
	cell.x = i;
	cell.y = j;
	return cell;
}

uint8_t getFloodValue(coordinate cell, uint8_t dir) {
	switch (dir) {
		case NORTH:
			return flood[cell.x][cell.y+1];
		case EAST:
			return flood[cell.x+1][cell.y];
		case SOUTH:
			return flood[cell.x][cell.y-1];
		case WEST:
			return flood[cell.x-1][cell.y];
		case CURRENT:
			return flood[cell.x][cell.y];
		default:
			break;
	}
	return flood[cell.x][cell.y];
}
