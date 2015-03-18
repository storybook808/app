#ifndef motor_H
#define motor_H

#include "global_libs.h"
#include "HAL_includes.h"
#include "defines.h"

#include "adc.h"
#include "encoder.h"
#include "interface.h"
#include "correction.h"

void brake();
void setVelocity(double velocity);
void setRightVelocity(double velocity);
void setLeftVelocity(double velocity);
double getCurrentVelocity(Motor channel);
double getTargetVelocity(Motor channel);

void MX_TIM2_Init(void);

#endif
