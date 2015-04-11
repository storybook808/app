/* File Name: system.h
 * Project: EE 396
 * Team: TSM
 * Members: Mark Miw
 *          Steven Chen
 *          Joseph Felix Jr.
 */

#ifndef flash_H
#define flash_H

#include "global_libs.h"
#include "HAL_includes.h"
#include "defines.h"

/* Base address of the Flash sectors */
#define ADDR_FLASH_SECTOR_0     ((uint32_t)0x08000000) /* Base @ of Sector 0, 16 Kbytes */
#define ADDR_FLASH_SECTOR_1     ((uint32_t)0x08004000) /* Base @ of Sector 1, 16 Kbytes */
#define ADDR_FLASH_SECTOR_2     ((uint32_t)0x08008000) /* Base @ of Sector 2, 16 Kbytes */
#define ADDR_FLASH_SECTOR_3     ((uint32_t)0x0800C000) /* Base @ of Sector 3, 16 Kbytes */
#define ADDR_FLASH_SECTOR_4     ((uint32_t)0x08010000) /* Base @ of Sector 4, 64 Kbytes */
#define ADDR_FLASH_SECTOR_5     ((uint32_t)0x08020000) /* Base @ of Sector 5, 128 Kbytes */
#define ADDR_FLASH_SECTOR_6     ((uint32_t)0x08040000) /* Base @ of Sector 6, 128 Kbytes */
#define ADDR_FLASH_SECTOR_7     ((uint32_t)0x08060000) /* Base @ of Sector 7, 128 Kbytes */
#define ADDR_FLASH_SECTOR_8     ((uint32_t)0x08080000) /* Base @ of Sector 8, 128 Kbytes */
#define ADDR_FLASH_SECTOR_9     ((uint32_t)0x080A0000) /* Base @ of Sector 9, 128 Kbytes */
#define ADDR_FLASH_SECTOR_10    ((uint32_t)0x080C0000) /* Base @ of Sector 10, 128 Kbytes */
#define ADDR_FLASH_SECTOR_11    ((uint32_t)0x080E0000) /* Base @ of Sector 11, 128 Kbytes */

#define SECTOR_SIZE	32768	/* Amount of words in Sectors */

/* Get Sector Function
 * 		Takes in a user specified address
 *  	Return is the Flash Sector
 *  	start_addr is the sector address it is located in
 *		size returns the size of the sector
 */
uint32_t flash_get_sector_info(uint32_t addr, uint32_t *start_addr, uint32_t *size);

/* Erase Sector based on flash_dest
 * 		Number of sectors erased is based upon num_word32
 */
void flash_erase(uint32_t flash_dest, uint32_t num_word32);

/* Takes in the address location and writes the array's data to flash
 *		flash_dest is the starting address
 *		*src is an array of uint32_t data
 *		num_word32 is the size of the array
 */
void flash_write(uint32_t flash_dest, const uint32_t *src, uint32_t num_word32);

#endif
