#ifndef adc_H
#define adc_H

#include "global_libs.h"
#include "HAL_includes.h"
#include "defines.h"

#include "interface.h"
#include "calibration.h"

uint32_t readADC(int channel);
uint32_t readBattery(void);

double readSensor(ADC_Channel channel);

// Remove these and create readSensor
double readRightSensor();
double readRightCenterSensor();
double readLeftCenterSensor();
double readLeftSensor();

uint16_t ADC_getSampleAvgNDeleteX(uint8_t N, uint8_t X, int channel);
void Sort_tab(uint16_t tab[], uint8_t length);

#endif
