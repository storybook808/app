#include "system.h"

// Data structure for TIM configuration
TIM_HandleTypeDef buzzerHandler;
TIM_HandleTypeDef countHandler;
TIM_HandleTypeDef rightHandler;
TIM_HandleTypeDef motorHandler;

// Data structure for ADC configuration
ADC_HandleTypeDef ADCHandle;

//Data structure for USART configuration
USART_HandleTypeDef USART_HandleStructure;

void initSystem(void) {
	initGPIO();
	initTIM();
	initADC();
	initUSART();
}

void initGPIO(void) {
	GPIO_InitTypeDef initGPIOStructure;

	__GPIOA_CLK_ENABLE();
	__GPIOB_CLK_ENABLE();
	__GPIOC_CLK_ENABLE();
    __GPIOH_CLK_ENABLE();

	// White LED
	initGPIOStructure.Mode  = GPIO_MODE_OUTPUT_PP;
	initGPIOStructure.Pull  = GPIO_NOPULL;
	initGPIOStructure.Speed = GPIO_SPEED_MEDIUM;
	initGPIOStructure.Pin = GPIO_PIN_11;
	HAL_GPIO_Init(GPIOB, &initGPIOStructure);

	// Blue LED
	initGPIOStructure.Pin = GPIO_PIN_10;
	HAL_GPIO_Init(GPIOB, &initGPIOStructure);

	// Green LED
	initGPIOStructure.Pin = GPIO_PIN_5;
	HAL_GPIO_Init(GPIOA, &initGPIOStructure);

	// Red LED
	initGPIOStructure.Pin = GPIO_PIN_6;
	HAL_GPIO_Init(GPIOA, &initGPIOStructure);

	// Buzzer
	initGPIOStructure.Pin = GPIO_PIN_4;
	HAL_GPIO_Init(GPIOB, &initGPIOStructure);

	// Left emitter
    initGPIOStructure.Pin = GPIO_PIN_1;
    HAL_GPIO_Init(GPIOH, &initGPIOStructure);
    
    // Left center emitter
    initGPIOStructure.Pin = GPIO_PIN_15;
    HAL_GPIO_Init(GPIOC, &initGPIOStructure);
    
    // Right center emitter
    initGPIOStructure.Pin = GPIO_PIN_0;
    HAL_GPIO_Init(GPIOH, &initGPIOStructure);
    
    // Right emitter
    initGPIOStructure.Pin = GPIO_PIN_14;
    HAL_GPIO_Init(GPIOC, &initGPIOStructure);

	// Left motor directional bit
	initGPIOStructure.Pin = GPIO_PIN_7;
	HAL_GPIO_Init(GPIOB, &initGPIOStructure);

	// Right motor directional bit
	initGPIOStructure.Pin = GPIO_PIN_9;
	HAL_GPIO_Init(GPIOB, &initGPIOStructure);

	// Left motor PWM pin
	initGPIOStructure.Mode = GPIO_MODE_AF_PP;
	initGPIOStructure.Alternate = GPIO_AF2_TIM4;
	initGPIOStructure.Pin = GPIO_PIN_6;
	HAL_GPIO_Init(GPIOB, &initGPIOStructure);

	// Right motor PWM pin
	initGPIOStructure.Pin = GPIO_PIN_8;
	HAL_GPIO_Init(GPIOB, &initGPIOStructure);

	// Left detector
	initGPIOStructure.Mode = GPIO_MODE_ANALOG;
    initGPIOStructure.Speed = GPIO_SPEED_HIGH;
    initGPIOStructure.Pin = GPIO_PIN_0;
    HAL_GPIO_Init(GPIOB, &initGPIOStructure);

    // Left center detector
    initGPIOStructure.Pin = GPIO_PIN_7;
    HAL_GPIO_Init(GPIOA, &initGPIOStructure);
    
    // Right center detector
    initGPIOStructure.Pin = GPIO_PIN_4;
    HAL_GPIO_Init(GPIOA, &initGPIOStructure);
    
    // Right detector
    initGPIOStructure.Pin = GPIO_PIN_5;
    HAL_GPIO_Init(GPIOC, &initGPIOStructure);
    
    // Voltage detector
    initGPIOStructure.Pin = GPIO_PIN_1;
    HAL_GPIO_Init(GPIOB, &initGPIOStructure);

    // Flash detector
    initGPIOStructure.Pin = GPIO_PIN_4;
    HAL_GPIO_Init(GPIOC, &initGPIOStructure);

	// Left encoder channel 2
	initGPIOStructure.Mode = GPIO_MODE_INPUT;
	initGPIOStructure.Pull = GPIO_PULLUP;
	initGPIOStructure.Speed = GPIO_SPEED_HIGH;
	initGPIOStructure.Pin = GPIO_PIN_3;
	HAL_GPIO_Init(GPIOB, &initGPIOStructure);

	// Right encoder channel 2
	initGPIOStructure.Pin = GPIO_PIN_1;
	HAL_GPIO_Init(GPIOA, &initGPIOStructure);

	// Left encoder channel 1
	initGPIOStructure.Mode = GPIO_MODE_IT_RISING;
	initGPIOStructure.Pull = GPIO_NOPULL;
	initGPIOStructure.Speed = GPIO_SPEED_HIGH;
	initGPIOStructure.Pin = GPIO_PIN_15;
	HAL_GPIO_Init(GPIOA, &initGPIOStructure);

	// Right encoder channel 1
	initGPIOStructure.Pin = GPIO_PIN_0;
	HAL_GPIO_Init(GPIOA, &initGPIOStructure);

	HAL_NVIC_EnableIRQ(EXTI0_IRQn);
	HAL_NVIC_EnableIRQ(EXTI15_10_IRQn);

	//TX
	initGPIOStructure.Mode = GPIO_MODE_AF_PP;
	initGPIOStructure.Pull = GPIO_NOPULL;
	initGPIOStructure.Speed = GPIO_SPEED_FAST;
	initGPIOStructure.Alternate = GPIO_AF7_USART1;
	initGPIOStructure.Pin = GPIO_PIN_9;
	HAL_GPIO_Init(GPIOA, &initGPIOStructure);

	//RX
	initGPIOStructure.Pin = GPIO_PIN_10;
	HAL_GPIO_Init(GPIOA, &initGPIOStructure);
}

void initTIM(void) {
	TIM_OC_InitTypeDef sConfig;

	__TIM2_CLK_ENABLE(); //TIM for counter
	__TIM3_CLK_ENABLE(); //TIM for buzzer
	__TIM4_CLK_ENABLE(); //TIM for motors
	__TIM5_CLK_ENABLE(); //TIM for countRight


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

	// Configure TIM for buzzer
	buzzerHandler.Instance = TIM3;
	buzzerHandler.Init.Period = 42000;
	buzzerHandler.Init.Prescaler = 0;
	buzzerHandler.Init.ClockDivision = 0;
	buzzerHandler.Init.CounterMode = TIM_COUNTERMODE_DOWN;

	HAL_TIM_Base_Init(&buzzerHandler);
	HAL_TIM_Base_Stop_IT(&buzzerHandler);

	HAL_NVIC_EnableIRQ(TIM3_IRQn);

	// Configure TIM for countLeft
	countHandler.Instance = TIM2;
	countHandler.Init.Period = 42000;
	countHandler.Init.Prescaler = 0;
	countHandler.Init.ClockDivision = 0;
	countHandler.Init.CounterMode = TIM_COUNTERMODE_DOWN;

	HAL_TIM_Base_Init(&countHandler);
	HAL_TIM_Base_Stop_IT(&countHandler);

	HAL_NVIC_EnableIRQ(TIM2_IRQn);

	// Configure TIM for ms
	rightHandler.Instance = TIM5;
	rightHandler.Init.Period = 55999;
	rightHandler.Init.Prescaler = 2;
	rightHandler.Init.ClockDivision = 0;
	rightHandler.Init.CounterMode = TIM_COUNTERMODE_UP;
	HAL_TIM_Base_Init(&rightHandler);
	HAL_TIM_Base_Stop_IT(&rightHandler);
	HAL_NVIC_EnableIRQ(TIM5_IRQn);
}

void initADC(void) {
	ADC_ChannelConfTypeDef sConfig;

	__ADC1_CLK_ENABLE();	

	ADCHandle.Instance = ADC1;
    
    ADCHandle.Init.ClockPrescaler = ADC_CLOCKPRESCALER_PCLK_DIV2;
    ADCHandle.Init.Resolution = ADC_RESOLUTION12b;
    ADCHandle.Init.ScanConvMode = ENABLE;
    ADCHandle.Init.ContinuousConvMode = DISABLE;
    ADCHandle.Init.NbrOfConversion = 1;
    ADCHandle.Init.DiscontinuousConvMode = DISABLE;
    ADCHandle.Init.NbrOfDiscConversion = 0;
    ADCHandle.Init.ExternalTrigConvEdge = ADC_EXTERNALTRIGCONVEDGE_NONE;
    ADCHandle.Init.ExternalTrigConv = ADC_EXTERNALTRIGCONV_T1_CC1;
    ADCHandle.Init.DMAContinuousRequests = DISABLE;
    ADCHandle.Init.EOCSelection = DISABLE;
    
    HAL_ADC_Init(&ADCHandle);
    
    //Left detector
    sConfig.Channel = ADC_CHANNEL_8;
    sConfig.Rank = 1;
    sConfig.SamplingTime = ADC_SAMPLETIME_28CYCLES;
    sConfig.Offset = 0;
    HAL_ADC_ConfigChannel(&ADCHandle, &sConfig);
}

void initUSART(void) {
	USART_HandleStructure.Instance = USART1;

	USART_HandleStructure.Init.BaudRate = 9600;
	USART_HandleStructure.Init.WordLength = USART_WORDLENGTH_8B;
	USART_HandleStructure.Init.StopBits = USART_STOPBITS_1;
	USART_HandleStructure.Init.Parity = USART_PARITY_NONE;
	USART_HandleStructure.Init.Mode = USART_MODE_TX;
	USART_HandleStructure.Init.CLKPolarity = USART_POLARITY_HIGH;
	USART_HandleStructure.Init.CLKPhase = USART_PHASE_1EDGE;
	USART_HandleStructure.Init.CLKLastBit = USART_LASTBIT_ENABLE;

	HAL_USART_Init(&USART_HandleStructure);	
}