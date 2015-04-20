/*
 * This file is part of the Micro Python project, http://micropython.org/
 *
 * The MIT License (MIT)
 *
 * Copyright (c) 2013, 2014 Damien P. George
 *
 * Permission is hereby granted, free of charge, to any person obtaining a copy
 * of this software and associated documentation files (the "Software"), to deal
 * in the Software without restriction, including without limitation the rights
 * to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 * copies of the Software, and to permit persons to whom the Software is
 * furnished to do so, subject to the following conditions:
 *
 * The above copyright notice and this permission notice shall be included in
 * all copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 * AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 * OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
 * THE SOFTWARE.
 */

#include "HAL_includes.h"

#include "flash.h"

static const uint32_t flash_info_table[26] = {
    ADDR_FLASH_SECTOR_0, FLASH_SECTOR_0,
    ADDR_FLASH_SECTOR_1, FLASH_SECTOR_1,
    ADDR_FLASH_SECTOR_2, FLASH_SECTOR_2,
    ADDR_FLASH_SECTOR_3, FLASH_SECTOR_3,
    ADDR_FLASH_SECTOR_4, FLASH_SECTOR_4,
    ADDR_FLASH_SECTOR_5, FLASH_SECTOR_5,
    ADDR_FLASH_SECTOR_6, FLASH_SECTOR_6,
    ADDR_FLASH_SECTOR_7, FLASH_SECTOR_7,
    ADDR_FLASH_SECTOR_8, FLASH_SECTOR_8,
    ADDR_FLASH_SECTOR_9, FLASH_SECTOR_9,
    ADDR_FLASH_SECTOR_10, FLASH_SECTOR_10,
    ADDR_FLASH_SECTOR_11, FLASH_SECTOR_11,
    ADDR_FLASH_SECTOR_11 + 0x20000, 0,
};

uint32_t flash_get_sector_info(uint32_t addr, uint32_t *start_addr, uint32_t *size) {
	int i;
    if (addr >= flash_info_table[0]) {
        for (i = 0; i < 24; i += 2) {
            if (addr < flash_info_table[i + 2]) {
                if (start_addr != NULL) {
                    *start_addr = flash_info_table[i];
                }
                if (size != NULL) {
                    *size = flash_info_table[i + 2] - flash_info_table[i];
                }
                return flash_info_table[i + 1];
            }
        }
    }
    return 0;
}

bool flash_erase(uint32_t flash_dest, uint32_t num_word32) {
    // check there is something to write
    if (num_word32 == 0) {
        return true;
    }

    // unlock
    HAL_FLASH_Unlock();

    // Clear pending flags (if any)
    __HAL_FLASH_CLEAR_FLAG(FLASH_FLAG_EOP | FLASH_FLAG_OPERR | FLASH_FLAG_WRPERR |
                           FLASH_FLAG_PGAERR | FLASH_FLAG_PGPERR|FLASH_FLAG_PGSERR);

    // erase the sector(s)
    FLASH_EraseInitTypeDef EraseInitStruct;
    EraseInitStruct.TypeErase = TYPEERASE_SECTORS;
    EraseInitStruct.VoltageRange = VOLTAGE_RANGE_3; // voltage range needs to be 2.7V to 3.6V
    EraseInitStruct.Sector = flash_get_sector_info(flash_dest, NULL, NULL);
    EraseInitStruct.NbSectors = flash_get_sector_info(flash_dest + 4 * num_word32 - 1, NULL, NULL) - EraseInitStruct.Sector + 1;
    uint32_t SectorError = 0;
    if (HAL_FLASHEx_Erase(&EraseInitStruct, &SectorError) != HAL_OK) {
        // error occurred during sector erase
        HAL_FLASH_Lock(); // lock the flash
        return true;
    }

    return false;
}

bool eraseMap() {
	return flash_erase(WRITE_ADDRESS_ROWS, 32768);
}

bool eraseCalibration() {
	return flash_erase(WRITE_ADDRESS_CALI, 32768);
}

void flash_write(uint32_t flash_dest, const uint32_t *src, uint32_t num_word32) {
    // program the flash word by word
	int i;
    for (i = 0; i < num_word32; i++) {
        if (HAL_FLASH_Program(TYPEPROGRAM_WORD, flash_dest, *src) != HAL_OK) {
            // error occurred during flash write
            HAL_FLASH_Lock(); // lock the flash
            return;
        }
        flash_dest += 4;
        src += 1;
    }

    // lock the flash
    HAL_FLASH_Lock();
}

bool writeRows() {
	int i;
	uint32_t flash_dest = WRITE_ADDRESS_ROWS;
	bool result = true;

	// write the 16 rows of data
	for (i = 0; i < 16; ++i) {
		if (HAL_FLASH_Program(TYPEPROGRAM_WORD, flash_dest, row[i]) != HAL_OK) {
			// error during write process
			HAL_FLASH_Lock();
			return result;
		}
		// 4 is for four bytes in the word, or uint32_t row data
		flash_dest+=4;
	}

	// lock the flash
	HAL_FLASH_Lock();

	return false;
}

void loadRows() {
	int i;
	uint32_t *data = (uint32_t *)WRITE_ADDRESS_ROWS;

	for (i = 0; i < 16; ++i) {
		row[i] = *data;
		data++;
	}
}

bool writeCalibration() {
	int i;
	uint32_t flash_dest = WRITE_ADDRESS_CALI;
	bool result = true;

	calibration[0] = getWall(CENTERRIGHTFRONT);
	calibration[1] = getWall(CENTERLEFTFRONT);
	calibration[2] = getWall(IDEALRIGHTFRONT);
	calibration[3] = getWall(IDEALLEFTFRONT);
	calibration[4] = getWall(IDEALRIGHTCENTER);
	calibration[5] = getWall(IDEALLEFTCENTER);
	calibration[6] = getWall(FARRIGHTWALL);
	calibration[7] = getWall(FARLEFTWALL);

	// write the 2 calibration readings of data
	for (i = 0; i < 8; i++) {
		if (HAL_FLASH_Program(TYPEPROGRAM_WORD, flash_dest, calibration[i]) != HAL_OK) {
			// error during write process
			HAL_FLASH_Lock();
			printInt(i);
			return result;
		}
		// 4 is for four bytes in the float
		flash_dest+=4;
	}

	// lock the flash
	HAL_FLASH_Lock();

	return false;
}

void loadCalibration() {
	int i;
	uint32_t *data = (uint32_t *)WRITE_ADDRESS_CALI;

	for (i = 0; i < 8; ++i) {
		calibration[i] = *data;
		data+=1;
	}

	setWall(CENTERRIGHTFRONT, calibration[0]);
	setWall(CENTERLEFTFRONT, calibration[1]);
	setWall(IDEALRIGHTFRONT, calibration[2]);
	setWall(IDEALLEFTFRONT, calibration[3]);
	setWall(IDEALRIGHTCENTER, calibration[4]);
	setWall(IDEALLEFTCENTER, calibration[5]);
	setWall(FARRIGHTWALL, calibration[6]);
	setWall(FARLEFTWALL, calibration[7]);
}
