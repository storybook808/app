#include "components/coocox-master/STM32F405xx_cmsisboot/source/stm32f4xx.h"
#include "components/coocox-master/STM32F405xx_cmsisboot/source/stm32f405xx.h"
#include "components/coocox-master/STM32F405xx_cmsisboot/source/Hal/stm32f4xx_hal.h"
#include "components/coocox-master/STM32F405xx_cmsisboot/source/Hal/stm32f4xx_hal_conf.h"
#include "components/coocox-master/STM32F405xx_cmsisboot/source/Hal/stm32f4xx_hal_gpio.h"
#include "components/coocox-master/STM32F405xx_cmsisboot/source/Hal/stm32f4xx_hal_rcc.h"

#include "adc.h"
#include "encoder.h"
#include "led.h"
#include "motor.h"
#include "stm32f4xx_it.h"
#include "usart.h"
#include <string.h>
#include "calibration.h"
#include "correction.h"

//Private prototypes
void testChaser(int mode, int period);
void testMenu();
void testRamp(int maxSpeed, int period);
void batteryFault();
void SystemClock_Config(void);

void main(void) {
	HAL_Init();
	SystemClock_Config();
	initLED();
	initUSART();
	initMotor();
	initEncoder();
	initADC();
	calibrateSensors();

	//LED start up sequence
	testChaser(1, 250);

	// Test USART Connection
	printString("Hello world!");
	printNL();
	resetEncoder(RIGHTENCODER);
	resetEncoder(LEFTENCODER);

	setVelocity(50.0);

	while (1) {
		batteryFault();
	}

	return;
}

void testChaser(int mode, int period) {
	switch (mode) {
	case 0:
		setLED(WHITE);
		HAL_Delay(period);
		setLED(BLUE);
		HAL_Delay(period);
		setLED(GREEN);
		HAL_Delay(period);
		setLED(RED);
		HAL_Delay(period);
		break;
	case 1:
		setLED(WHITE);
		HAL_Delay(period);
		setLED(BLUE);
		HAL_Delay(period);
		setLED(GREEN);
		HAL_Delay(period);
		setLED(RED);
		HAL_Delay(period);
		toggleLEDAll();
		HAL_Delay(period);
		toggleLEDAll();
		HAL_Delay(period);
		toggleLEDAll();
		HAL_Delay(period);
		toggleLEDAll();
		HAL_Delay(period);
		toggleLEDAll();
		break;
	case 2:
		setLED(WHITE);
		HAL_Delay(period);
		resetLED(WHITE);
		setLED(BLUE);
		HAL_Delay(period);
		resetLED(BLUE);
		setLED(GREEN);
		HAL_Delay(period);
		resetLED(GREEN);
		setLED(RED);
		HAL_Delay(period);
		break;
	}
}

void testMenu(int channel) {
	int count = readEncoder(channel);
	if (count < 0) {
		count = 0;
	} else if (count >= 0 && count <= 800) {
		setLED(RED);
		resetLED(GREEN);
	} else if (count > 800 && count <= (800*2)) {
		resetLED(RED);
		setLED(GREEN);
		resetLED(BLUE);
	} else if (count > (800*2) && count <= (800*3)) {
		resetLED(GREEN);
		setLED(BLUE);
		resetLED(WHITE);
	} else if (count > (800*3) && count <= (800*4)) {
		resetLED(BLUE);
		setLED(WHITE);
	} else if (count > (800*4)) {
		count = (800*4);
	}
}

void testRamp(int maxSpeed, int period) {
	int x;
	for(x = 0; x < maxSpeed; x++) {
		setSpeed(LEFTMOTOR, x);
		setSpeed(RIGHTMOTOR, x);
		HAL_Delay(period);
	}

	for(x = maxSpeed; x >=0; x--) {
		setSpeed(LEFTMOTOR, x);
		setSpeed(RIGHTMOTOR, x);
		HAL_Delay(period);
	}
}

void batteryFault() {
	//Take a reading from the voltage detector
	uint32_t batteryLevel = readBattery();
	//Check to see if voltage level is above 7V
	//Voltage detector is a voltage divider where 7V is measured as 2.3333V
	//2.3333V translate to roughly 2333 from the 12 bit ADC
	if (batteryLevel < 2333) {
		//Disable all motors
		setSpeed(LEFTMOTOR, 0);
		setSpeed(RIGHTMOTOR, 0);
		//Enable buzzer
		setBuzzer(ON);
		//Disable all LEDs
		resetLEDAll();
		//Flash red LED every half second.
		while (1) {
			//Invert the state of the red LED located closest to the STM
			toggleLED(RED);
			//Half second delay
			HAL_Delay(500);
		}
	}
}

void SystemClock_Config(void)
{

  RCC_OscInitTypeDef RCC_OscInitStruct;
  RCC_ClkInitTypeDef RCC_ClkInitStruct;

  __PWR_CLK_ENABLE();

  __HAL_PWR_VOLTAGESCALING_CONFIG(PWR_REGULATOR_VOLTAGE_SCALE1);

  RCC_OscInitStruct.OscillatorType = RCC_OSCILLATORTYPE_HSI;
  RCC_OscInitStruct.HSIState = RCC_HSI_ON;
  RCC_OscInitStruct.HSICalibrationValue = 6;
  RCC_OscInitStruct.PLL.PLLState = RCC_PLL_ON;
  RCC_OscInitStruct.PLL.PLLSource = RCC_PLLSOURCE_HSI;
  RCC_OscInitStruct.PLL.PLLM = 10;
  RCC_OscInitStruct.PLL.PLLN = 210;
  RCC_OscInitStruct.PLL.PLLP = RCC_PLLP_DIV2;
  RCC_OscInitStruct.PLL.PLLQ = 4;
  HAL_RCC_OscConfig(&RCC_OscInitStruct);

  RCC_ClkInitStruct.ClockType = RCC_CLOCKTYPE_SYSCLK|RCC_CLOCKTYPE_PCLK1
                              |RCC_CLOCKTYPE_PCLK2;
  RCC_ClkInitStruct.SYSCLKSource = RCC_SYSCLKSOURCE_PLLCLK;
  RCC_ClkInitStruct.AHBCLKDivider = RCC_SYSCLK_DIV1;
  RCC_ClkInitStruct.APB1CLKDivider = RCC_HCLK_DIV4;
  RCC_ClkInitStruct.APB2CLKDivider = RCC_HCLK_DIV2;
  HAL_RCC_ClockConfig(&RCC_ClkInitStruct, FLASH_LATENCY_5);

}
