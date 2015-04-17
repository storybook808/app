/* File Name: correction.h
 * Project: EE 396
 * Team: TSM
 * Members: Mark Miw
 *          Steven Chen
 *          Joseph Felix Jr.
 */

#ifndef correction_H
#define correction_H

#include "global_libs.h"
#include "HAL_includes.h"
#include "defines.h"

#include "tracking.h"
#include "motor.h"
#include "usart.h"
#include "interface.h"

void frontCorrection();
void brakeCorrection(int startR, int startL);
void correction(uint8_t wall, float base_speed);
void correction2(float base_speed);

float last_errorP;
float last_leftErrorP;
float last_rightErrorP;

#endif
