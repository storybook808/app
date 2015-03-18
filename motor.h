#ifndef interface_H
#define interface_H

#include "global_libs.h"
#include "HAL_includes.h"
#include "defines.h"

static void setDirection(Motor channel, Direction state);
int currentSpeed(Motor channel);
void setSpeed(Motor channel, int speed);

void brake();
void brakeCallBack();
void setVelocity(double velocity);
void setRightVelocity(double velocity);
void setLeftVelocity(double velocity);
void velocityCallBack();
double getCurrentVelocity(Motor channel);
double getTargetVelocity(Motor channel);

#endif
