#include "components/coocox-master/STM32F405xx_cmsisboot/source/Hal/stm32f4xx_hal.h"
#include "components/coocox-master/STM32F405xx_cmsisboot/source/Hal/stm32f4xx_hal_gpio.h"
#include "components/coocox-master/STM32F405xx_cmsisboot/source/Hal/stm32f4xx_hal_rcc.h"
#include "components/coocox-master/STM32F405xx_cmsisboot/source/Hal/stm32f4xx_hal_tim.h"

#include "adc.h"
#include "encoder.h"
#include "led.h"
#include "motor.h"

//Data structure for TIM configuration
TIM_HandleTypeDef buzzerHandler;
TIM_HandleTypeDef countHandler;
TIM_HandleTypeDef rightHandler;
TIM_HandleTypeDef motorHandler;
TIM_OC_InitTypeDef sConfig;

static int leftMotorSpeed;
static int rightMotorSpeed;

static uint32_t targetDistance;
static uint32_t targetSpeed;
static uint32_t targetTimeDifference;

static int leftSpeedBuffer, rightSpeedBuffer;

void initMotor(void) {
	leftMotorSpeed  = 0;
	rightMotorSpeed = 0;

	//Data structure for GPIO configuration
	GPIO_InitTypeDef GPIO_InitStructure;

	//Enable GPIO clock for LED module (B)
	__GPIOB_CLK_ENABLE();

	//Enable TIM clock for PWM (2 & 3 & 4)
	__TIM2_CLK_ENABLE(); //TIM for counter
	__TIM3_CLK_ENABLE(); //TIM for buzzer
	__TIM4_CLK_ENABLE(); //TIM for motors
	__TIM5_CLK_ENABLE(); //TIM for countRight

	//Configure data structure for GPIO output
	GPIO_InitStructure.Mode = GPIO_MODE_OUTPUT_PP;
	GPIO_InitStructure.Pull = GPIO_NOPULL;
	GPIO_InitStructure.Speed = GPIO_SPEED_MEDIUM;

	//Left motor directional bit
	GPIO_InitStructure.Pin = GPIO_PIN_7;
	HAL_GPIO_Init(GPIOB, &GPIO_InitStructure);

	//Right motor directional bit
	GPIO_InitStructure.Pin = GPIO_PIN_9;
	HAL_GPIO_Init(GPIOB, &GPIO_InitStructure);

	//Buzzer
	GPIO_InitStructure.Pin = GPIO_PIN_4;
	HAL_GPIO_Init(GPIOB, &GPIO_InitStructure);

	//Configure data structure for GPIO AF
	GPIO_InitStructure.Mode = GPIO_MODE_AF_PP;
	GPIO_InitStructure.Alternate = GPIO_AF2_TIM4;

	//Left motor PWM pin
	GPIO_InitStructure.Pin = GPIO_PIN_6;
	HAL_GPIO_Init(GPIOB, &GPIO_InitStructure);

	//Right motor PWM pin
	GPIO_InitStructure.Pin = GPIO_PIN_8;
	HAL_GPIO_Init(GPIOB, &GPIO_InitStructure);

	//Configure TIM for motors
	motorHandler.Instance = TIM4;
	motorHandler.Init.Period = PERIOD;
	motorHandler.Init.Prescaler = 0;
	motorHandler.Init.ClockDivision = 0;
	motorHandler.Init.CounterMode = TIM_COUNTERMODE_DOWN;
	HAL_TIM_PWM_Init(&motorHandler);

	sConfig.OCMode = TIM_OCMODE_PWM1;
	sConfig.OCPolarity = TIM_OCPOLARITY_HIGH;
	sConfig.Pulse = 0;

	HAL_TIM_PWM_ConfigChannel(&motorHandler, &sConfig, TIM_CHANNEL_1);
	HAL_TIM_PWM_ConfigChannel(&motorHandler, &sConfig, TIM_CHANNEL_3);
	HAL_TIM_PWM_Start(&motorHandler, TIM_CHANNEL_1);
	HAL_TIM_PWM_Start(&motorHandler, TIM_CHANNEL_3);

	//Configure TIM for buzzer
	buzzerHandler.Instance = TIM3;
	buzzerHandler.Init.Period = 42000;
	buzzerHandler.Init.Prescaler = 0;
	buzzerHandler.Init.ClockDivision = 0;
	buzzerHandler.Init.CounterMode = TIM_COUNTERMODE_DOWN;

	HAL_TIM_Base_Init(&buzzerHandler);
	HAL_TIM_Base_Stop_IT(&buzzerHandler);

	HAL_NVIC_EnableIRQ(TIM3_IRQn);

	//Configure TIM for countLeft
	countHandler.Instance = TIM2;
	countHandler.Init.Period = 42000;
	countHandler.Init.Prescaler = 0;
	countHandler.Init.ClockDivision = 0;
	countHandler.Init.CounterMode = TIM_COUNTERMODE_DOWN;

	HAL_TIM_Base_Init(&countHandler);
	HAL_TIM_Base_Stop_IT(&countHandler);

	HAL_NVIC_EnableIRQ(TIM2_IRQn);

	//Configure TIM for ms
	rightHandler.Instance = TIM5;
	rightHandler.Init.Period = 55999;
	rightHandler.Init.Prescaler = 2;
	rightHandler.Init.ClockDivision = 0;
	rightHandler.Init.CounterMode = TIM_COUNTERMODE_UP;
	HAL_TIM_Base_Init(&rightHandler);
	HAL_TIM_Base_Stop_IT(&rightHandler);
	HAL_NVIC_EnableIRQ(TIM5_IRQn);

	setSpeed(LEFTMOTOR, 0);
	setSpeed(RIGHTMOTOR, 0);
}

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
	if (channel == LEFTMOTOR) return 1000 - leftMotorSpeed;
	else return 1000 - rightMotorSpeed;
}

void setSpeed(Motor channel, int speed) {
	if (speed >= 0) {
		setDirection(channel, FORWARD);
	}
	else {
		speed = ~speed;
		setDirection(channel, BACKWARD);
	}

	if (channel == LEFTMOTOR) {
		if (HAL_GPIO_ReadPin(GPIOB, GPIO_PIN_7) == GPIO_PIN_SET) speed = PERIOD - speed;
		leftMotorSpeed = speed;
		__HAL_TIM_SetCompare(&motorHandler, TIM_CHANNEL_1, speed);
	}
	else {
		if (HAL_GPIO_ReadPin(GPIOB, GPIO_PIN_9) == GPIO_PIN_SET) speed = PERIOD - speed;
		rightMotorSpeed = speed;
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
