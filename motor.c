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
TIM_HandleTypeDef buzzerHandler;
TIM_HandleTypeDef velocityHandler;
TIM_HandleTypeDef brakeHandler;
TIM_HandleTypeDef motorHandler;
TIM_OC_InitTypeDef sConfig;
TIM_HandleTypeDef htim2;

#define BREAK_k	50				// Brake slow-down/speedup rate
#define brakeThresh	100			// Brake time wait in milliseconds
#define R_ENCODER_DIST	0.00411	// Right encoder tick per cm
#define L_ENCODER_DIST	0.00410	// Left encoder tick per cm
#define VELOCITY_k	10			// Velocity slow-down/speedup rate

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
static void MX_TIM2_Init(void);

static uint32_t targetDistance;
static uint32_t targetSpeed;
static uint32_t targetTimeDifference;

static int leftSpeedBuffer, rightSpeedBuffer;

<<<<<<< HEAD
=======
void initMotor(void) {
	leftMotorSpeed  = 0;
	rightMotorSpeed = 0;
	oldEncoderR = 0;
	oldEncoderL = 0;
	counter = 0;
	thresh = 0;
	targetRightVelocity = 0;
	targetLeftVelocity = 0;
	currentRightVelocity = 0;
	currentLeftVelocity = 0;

	//Data structure for GPIO configuration
	GPIO_InitTypeDef GPIO_InitStructure;

	//Enable GPIO clock for LED module (B)
	__GPIOB_CLK_ENABLE();

	//Enable TIM clock for PWM (2 & 3 & 4 & 5)
	__TIM2_CLK_ENABLE();
	__TIM3_CLK_ENABLE(); //TIM for buzzer
	__TIM4_CLK_ENABLE(); //TIM for motors
	__TIM5_CLK_ENABLE(); //TIM for brake system

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

	//Configure TIM for Millisecond Brake System
	brakeHandler.Instance = TIM5;
	brakeHandler.Init.Period = 55999;
	brakeHandler.Init.Prescaler = 2;
	brakeHandler.Init.ClockDivision = 0;
	brakeHandler.Init.CounterMode = TIM_COUNTERMODE_UP;
	HAL_TIM_Base_Init(&brakeHandler);
	HAL_TIM_Base_Stop_IT(&brakeHandler);
	HAL_NVIC_EnableIRQ(TIM5_IRQn);

	MX_TIM2_Init();

	setSpeed(LEFTMOTOR, 0);
	setSpeed(RIGHTMOTOR, 0);
}

/* TIM2 init function */
void MX_TIM2_Init(void)
{

  TIM_ClockConfigTypeDef sClockSourceConfig;
  TIM_MasterConfigTypeDef sMasterConfig;

  htim2.Instance = TIM2;
  htim2.Init.Prescaler = 2;
  htim2.Init.CounterMode = TIM_COUNTERMODE_UP;
  htim2.Init.Period = 55999;
  htim2.Init.ClockDivision = TIM_CLOCKDIVISION_DIV1;
  HAL_TIM_Base_Init(&htim2);

  HAL_NVIC_EnableIRQ(TIM2_IRQn);
  HAL_TIM_Base_Stop(&htim2);

  sClockSourceConfig.ClockSource = TIM_CLOCKSOURCE_INTERNAL;
  HAL_TIM_ConfigClockSource(&htim2, &sClockSourceConfig);

  sMasterConfig.MasterOutputTrigger = TIM_TRGO_RESET;
  sMasterConfig.MasterSlaveMode = TIM_MASTERSLAVEMODE_DISABLE;
  HAL_TIMEx_MasterConfigSynchronization(&htim2, &sMasterConfig);

}

>>>>>>> origin/encoder-motor_upgrade
void setBuzzer(int state) {
	if (state) HAL_TIM_Base_Start_IT(&buzzerHandler);
	else HAL_TIM_Base_Stop_IT(&buzzerHandler);
}

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
	HAL_TIM_Base_Start_IT(&velocityHandler);
}

double getCurrentVelocity(Motor channel) {
	if (channel == RIGHTMOTOR) return currentRightVelocity;
	else return currentLeftVelocity;
}

double getTargetVelocity(Motor channel) {
	if (channel == RIGHTMOTOR) return targetRightVelocity;
	else return targetLeftVelocity;
}


void velocityCallBack() {
	int currentEncoderR = readEncoder(RIGHTENCODER);
	int currentEncoderL = readEncoder(LEFTENCODER);

	int currRspeed = currentSpeed(RIGHTMOTOR);
	int currLspeed = currentSpeed(LEFTMOTOR);

	currentRightVelocity = (double)(currentEncoderR - oldEncoderR)*R_ENCODER_DIST/0.001;
	currentLeftVelocity = (double)(currentEncoderL - oldEncoderL)*L_ENCODER_DIST/0.001;

	if(currentRightVelocity == targetRightVelocity) setLED(WHITE);
	else resetLED(WHITE);
	if(currentLeftVelocity == targetLeftVelocity) setLED(BLUE);
	else resetLED(BLUE);

	if ( currentRightVelocity > targetRightVelocity ) setSpeed(RIGHTMOTOR, currRspeed-VELOCITY_k);
	else if ( currentRightVelocity < targetRightVelocity ) setSpeed(RIGHTMOTOR, currRspeed+VELOCITY_k);
	else setSpeed(RIGHTMOTOR,currRspeed);

	if ( currentLeftVelocity > targetLeftVelocity ) setSpeed(LEFTMOTOR, currLspeed-VELOCITY_k);
	else if ( currentLeftVelocity < targetLeftVelocity ) setSpeed(LEFTMOTOR, currLspeed+VELOCITY_k);
	else setSpeed(LEFTMOTOR,currLspeed);

	oldEncoderR = currentEncoderR;
	oldEncoderL = currentEncoderL;
}

void brakeCallBack() {
	targetLeftVelocity = 0;
	targetRightVelocity = 0;
	int currentEncoderR = readEncoder(RIGHTENCODER);
	int currentEncoderL = readEncoder(LEFTENCODER);

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
	}
	else if (htim->Instance == TIM5) //Millisecond Timer
	{
		HAL_TIM_Base_Stop_IT(&velocityHandler);
		brakeCallBack();
	}
}
