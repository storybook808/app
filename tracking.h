#ifndef tracking_H
#define tracking_H

#include "global_libs.h"
#include "HAL_includes.h"
#include "defines.h"
#include "calibration.h"

bool hasRightWall(double value);

bool hasLeftWall(double value);

bool hasFrontWall(double valueRight, double valueLeft);

#endif
