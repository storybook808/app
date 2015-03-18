#ifndef encoder_H
#define encdoer_H

#include "global_libs.h"
#include "HAL_includes.h"
#include "defines.h"

/* Public Functions */
int getEncoder(Encoder channel);
void resetEncoder(Encoder channel);
void HAL_GPIO_EXTI_Callback(uint16_t GPIO_Pin);

/* Private Functions */
static void incrementEncoder(Encoder channel);
static void decrementEncoder(Encoder channel);

#endif
