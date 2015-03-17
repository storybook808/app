#include "components/coocox-master/STM32F405xx_cmsisboot/source/Hal/stm32f4xx_hal.h"
#include "components/coocox-master/STM32F405xx_cmsisboot/source/Hal/stm32f4xx_hal_gpio.h"
#include "components/coocox-master/STM32F405xx_cmsisboot/source/Hal/stm32f4xx_hal_rcc.h"
#include "components/coocox-master/STM32F405xx_cmsisboot/source/Hal/stm32f4xx_hal_tim.h"

#include "adc.h"
#include "encoder.h"
#include "led.h"
#include "motor.h"
#include "system.h"

//Data structure for TIM configuration
extern TIM_HandleTypeDef buzzerHandler;
extern TIM_HandleTypeDef countHandler;
extern TIM_HandleTypeDef rightHandler;
extern TIM_HandleTypeDef motorHandler;

static int leftMotorSpeed;
static int rightMotorSpeed;

static uint32_t targetDistance;
static uint32_t targetSpeed;
static uint32_t targetTimeDifference;

static int leftSpeedBuffer, rightSpeedBuffer;

void setBuzzer(int state) {
	if (state) HAL_TIM_Base_Start_IT(&buzzerHandler);
	else HAL_TIM_Base_Stop_IT(&buzzerHandler);
}

void setMilliTimer(int state) {
	if (state) HAL_TIM_Base_Start_IT(&rightHandler);
	else HAL_TIM_Base_Stop_IT(&rightHandler);
}

static void setDirection(Motor channel, Direction state) {
	if (channel == LEFTMOTOR) {
		if (state == FORWARD) HAL_GPIO_WritePin(GPIOB, GPIO_PIN_7, GPIO_PIN_SET);
		else HAL_GPIO_WritePin(GPIOB, GPIO_PIN_7, GPIO_PIN_RESET);
	}
	else {
		if (state == FORWARD) HAL_GPIO_WritePin(GPIOB, GPIO_PIN_9, GPIO_PIN_SET);
		else HAL_GPIO_WritePin(GPIOB, GPIO_PIN_9, GPIO_PIN_RESET);
	}
}

int currentSpeed(Motor channel) {
	if (channel == LEFTMOTOR) return leftMotorSpeed;
	else return rightMotorSpeed;
}

void setSpeed(Motor channel, int speed) {
	int neg = 1;
	if (speed >= 0) {
		setDirection(channel, FORWARD);
	}
	else {
		neg = -1;
		speed = ~speed;
		setDirection(channel, BACKWARD);
	}

	if (channel == LEFTMOTOR) {
		if (HAL_GPIO_ReadPin(GPIOB, GPIO_PIN_7) == GPIO_PIN_SET) {
			speed = PERIOD - speed;
			leftMotorSpeed = (1000 - speed)*neg;
		}
		else {
			leftMotorSpeed = speed*neg;
		}
		__HAL_TIM_SetCompare(&motorHandler, TIM_CHANNEL_1, speed);
	}
	else {
		if (HAL_GPIO_ReadPin(GPIOB, GPIO_PIN_9) == GPIO_PIN_SET) {
			speed = PERIOD - speed;
			rightMotorSpeed = (1000 - speed)*neg;
		}
		else {
			rightMotorSpeed = speed*neg;
		}
		__HAL_TIM_SetCompare(&motorHandler, TIM_CHANNEL_3, speed);
	}
}

/**
  * @brief  Non-blocking traveling function...
  * @param  distance: 	Number of ticks for the robot to travel
  * @param  maxSpeed: 	Target speed limit that the robot will not pass
  * @param	dt:			Rate of time between speed increase (msec)
  * @retval Nothing
  */
void travelDistance(uint32_t distance, uint32_t maxSpeed, uint32_t dt)
{
	targetDistance = distance;
	targetSpeed = maxSpeed;
	targetTimeDifference = dt;
	resetEncoder(LEFTENCODER);
	resetEncoder(RIGHTENCODER);
	setDirection(LEFTMOTOR, FORWARD);
	setDirection(RIGHTMOTOR, FORWARD);
	setSpeed(LEFTMOTOR, 0);
	setSpeed(RIGHTMOTOR, 0);
	leftSpeedBuffer = 0;
	rightSpeedBuffer = 0;
	HAL_TIM_Base_Start_IT(&countHandler);
}

void HAL_TIM_PeriodElapsedCallback(TIM_HandleTypeDef *htim)
{
	//Buzzer interrupt
	if (htim->Instance == TIM3) HAL_GPIO_TogglePin(GPIOB, GPIO_PIN_4);

	else if (htim->Instance == TIM2) //Counter interrupt
	{

	}
	else if (htim->Instance == TIM5) //Millisecond Timer
	{

	}
}
