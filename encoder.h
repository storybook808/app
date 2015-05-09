/* File Name: encoder.h
 * Project: EE 396
 * Team: TSM
 * Members: Mark Miw
 *          Steven Chen
 *          Joseph Felix Jr.
 */

#ifndef encoder_H
#define encdoer_H

#include "global_libs.h"
#include "HAL_includes.h"
#include "defines.h"

/* Public Functions */
int getEncoder(Encoder channel);
void resetEncoder(Encoder channel);
int getPosition(Encoder channel);
void resetPosition(Encoder channel);
void setPosition(Encoder channel);
void HAL_GPIO_EXTI_Callback(uint16_t GPIO_Pin);

#endif
