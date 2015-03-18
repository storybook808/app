#ifndef interface_H
#define interface_H

#include "global_libs.h"
#include "HAL_includes.h"
#include "defines.h"

void waitForTop(void);

void setLED(Led color);
void resetLED(Led color);
void setLEDAll(void);
void resetLEDAll(void);
void toggleLED(Led color);
void toggleLEDAll(void);

void setBuzzer(int state);

#endif
