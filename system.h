#ifndef system_H
#define system_H

#include "global_libs.h"
#include "HAL_includes.h"
#include "defines.h"

void initSystem(void);
void initGPIO(void);
void initTIM(void);
void initADC(void);
void initUSART(void);

#endif
