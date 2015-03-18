/* File Name: motor.c
 * Project: EE 396
 * Team: TSM
 * Members: Mark Miw
 *          Steven Chen
 *          Joseph Felix Jr.
 */

#include "motor.h"

//Data structure for TIM configuration
TIM_HandleTypeDef buzzerHandler;
TIM_HandleTypeDef velocityHandler;
TIM_HandleTypeDef brakeHandler;
TIM_HandleTypeDef motorHandler;
TIM_OC_InitTypeDef sConfig;
TIM_HandleTypeDef htim2;

static int leftMotorSpeed;
static int rightMotorSpeed;
static uint16_t counter;

static int oldEncoderR;
static int oldEncoderL;
static uint8_t thresh;

static double targetRightVelocity;
static double targetLeftVelocity;
static double currentRightVelocity;
static double currentLeftVelocity;

/* Private Functions */
static void setDirection(Motor channel, Direction state);
static void velocityCallBack();
static void brakeCallBack();

void brake() {
	HAL_TIM_Base_Start_IT(&brakeHandler);
}

void setVelocity(double velocity) {
	targetRightVelocity = velocity;
	targetLeftVelocity = velocity;
	HAL_TIM_Base_Start_IT(&htim2);
}

void setRightVelocity(double velocity) {
	targetRightVelocity = velocity;
}

void setLeftVelocity(double velocity) {
	targetLeftVelocity = velocity;
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

static int currentSpeed(Motor channel) {
	if (channel == LEFTMOTOR) return leftMotorSpeed;
	else return rightMotorSpeed;
}

static void setSpeed(Motor channel, int speed) {
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

double getCurrentVelocity(Motor channel) {
	if (channel == RIGHTMOTOR) return currentRightVelocity;
	else return currentLeftVelocity;
}

double getTargetVelocity(Motor channel) {
	if (channel == RIGHTMOTOR) return targetRightVelocity;
	else return targetLeftVelocity;
}

static void velocityCallBack() {
	int currentEncoderR = getEncoder(RIGHTENCODER);
	int currentEncoderL = getEncoder(LEFTENCODER);

	int currRspeed = currentSpeed(RIGHTMOTOR);
	int currLspeed = currentSpeed(LEFTMOTOR);

	currentRightVelocity = (double)(currentEncoderR - oldEncoderR)*R_ENCODER_DIST/0.001;
	currentLeftVelocity = (double)(currentEncoderL - oldEncoderL)*L_ENCODER_DIST/0.001;

	if ( currentRightVelocity > targetRightVelocity ) setSpeed(RIGHTMOTOR, currRspeed-VELOCITY_k);
	else if ( currentRightVelocity < targetRightVelocity ) setSpeed(RIGHTMOTOR, currRspeed+VELOCITY_k);
	else setSpeed(RIGHTMOTOR,currRspeed);

	if ( currentLeftVelocity > targetLeftVelocity ) setSpeed(LEFTMOTOR, currLspeed-VELOCITY_k);
	else if ( currentLeftVelocity < targetLeftVelocity ) setSpeed(LEFTMOTOR, currLspeed+VELOCITY_k);
	else setSpeed(LEFTMOTOR,currLspeed);

	oldEncoderR = currentEncoderR;
	oldEncoderL = currentEncoderL;
}

static void brakeCallBack() {
	targetLeftVelocity = 0;
	targetRightVelocity = 0;
	int currentEncoderR = getEncoder(RIGHTENCODER);
	int currentEncoderL = getEncoder(LEFTENCODER);

	int currRspeed = currentSpeed(RIGHTMOTOR);
	int currLspeed = currentSpeed(LEFTMOTOR);

	int diffR = currentEncoderR - oldEncoderR;
	int diffL = currentEncoderL - oldEncoderL;

	if (diffR > 0) setSpeed(RIGHTMOTOR,currRspeed-BREAK_k);
	else if (diffR < 0) setSpeed(RIGHTMOTOR,currRspeed+BREAK_k);
	else setSpeed(RIGHTMOTOR,currRspeed);

	if (diffL > 0) setSpeed(LEFTMOTOR,currLspeed-BREAK_k);
	else if (diffL < 0) setSpeed(LEFTMOTOR,currLspeed+BREAK_k);
	else setSpeed(LEFTMOTOR,currLspeed);

	oldEncoderR = currentEncoderR;
	oldEncoderL = currentEncoderL;

	if (diffR >= thresh && diffL >= thresh) {
		counter++;
		if(counter >= brakeThresh) {
			HAL_TIM_Base_Stop_IT(&brakeHandler);
			counter = 0;
			setSpeed(LEFTMOTOR,0);
			setSpeed(RIGHTMOTOR,0);
			currentRightVelocity = 0;
			currentLeftVelocity = 0;
		}
	}
}

void HAL_TIM_PeriodElapsedCallback(TIM_HandleTypeDef *htim)
{
	//Buzzer interrupt
	if (htim->Instance == TIM3) HAL_GPIO_TogglePin(GPIOB, GPIO_PIN_4);

	else if (htim->Instance == TIM2) //Velocity Timer
	{
		velocityCallBack();
		correction();
	}
	else if (htim->Instance == TIM5) //Millisecond Timer
	{
		HAL_TIM_Base_Stop_IT(&velocityHandler);
		brakeCallBack();
	}
}
