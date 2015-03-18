/* File Name: system.h
 * Project: EE 396
 * Team: TSM
 * Members: Mark Miw
 *          Steven Chen
 *          Joseph Felix Jr.
 */

#ifndef system_H
#define system_H

#include "global_libs.h"
#include "HAL_includes.h"
#include "defines.h"

#include "encoder.h"

void initSystem(void);
void initGPIO(void);
void initTIM(void);
void initADC(void);
void initUSART(void);
void SystemClock_Config(void);

#endif
