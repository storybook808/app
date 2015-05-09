/* File Name: encoder.c
 * Project: EE 396
 * Team: TSM
 * Members: Mark Miw
 *          Steven Chen
 *          Joseph Felix Jr.
 */

#include "encoder.h"

/* Private Functions */
static void incrementEncoder(Encoder channel);
static void decrementEncoder(Encoder channel);

static int leftCount, rightCount;
static int positionLeft, positionRight;

int getPosition(Encoder channel)
{
	if (channel == LEFTENCODER) {
		return positionLeft;
	}
	else return positionRight;
}

void resetPosition(Encoder channel)
{
	if (channel == LEFTENCODER) {
		positionLeft = 0;
	}
	else positionRight = 0;
}

void setPosition(Encoder channel)
{
	if (channel == LEFTENCODER) {
		positionLeft = (CELL_L/2-CELL_L/9)-10;
	}
	else positionRight = (CELL_R/2-CELL_R/9)-10;
}

int getEncoder(Encoder channel)
{
	if (channel == LEFTENCODER) return leftCount;
	else return rightCount;
}

void resetEncoder(Encoder channel)
{
	if (channel == LEFTENCODER) leftCount = 0;
	else rightCount = 0;
}

static void incrementEncoder(Encoder channel)
{
	if (channel == LEFTENCODER) {
		leftCount++;
		positionLeft++;
	}
	else {
		rightCount++;
		positionRight++;
	}
}

static void decrementEncoder(Encoder channel)
{
	if (channel == LEFTENCODER) {
		leftCount--;
		positionLeft--;
	}
	else {
		rightCount--;
		positionRight--;
	}
}

void HAL_GPIO_EXTI_Callback(uint16_t GPIO_Pin)
{
	if (GPIO_Pin == GPIO_PIN_0)
	{
		if (HAL_GPIO_ReadPin(GPIOA, GPIO_PIN_1) == GPIO_PIN_SET) incrementEncoder(RIGHTENCODER);
		else decrementEncoder(RIGHTENCODER);
	}
	else if (GPIO_Pin == GPIO_PIN_15)
	{
		if (HAL_GPIO_ReadPin(GPIOB, GPIO_PIN_3) == GPIO_PIN_SET) incrementEncoder(LEFTENCODER);
		else decrementEncoder(LEFTENCODER);
	}
}
