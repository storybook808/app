/* File Name: usart.c
 * Project: EE 396
 * Team: TSM
 * Members: Mark Miw
 *          Steven Chen
 *          Joseph Felix Jr.
 */

#include "usart.h"

#define TRANS_DELAY 200

//Data structure for USART configuration
extern USART_HandleTypeDef USART_HandleStructure;

void printInt(int value) {
    int k;
    int digLen = 1;
    char dig = '0';
    char digit[10];
    int negative = 1;

    if (value < 0) {
    	negative = -1;
    	value*=negative;
    }

    for(k = 10; k <= value; k*=10) {
        digLen++;
    }

    for(k = 0; k < digLen; k++) {
        digit[digLen-1-k] = dig + value%10;
        value/=10;
    }

    if (negative == -1) HAL_USART_Transmit(&USART_HandleStructure, (uint8_t *) "-", 1, 1000);
    HAL_USART_Transmit(&USART_HandleStructure, (uint8_t *)&digit, digLen, 1000);
    HAL_Delay(TRANS_DELAY);
}

void printFloat(float value) {
	int x = value;
	int y = (value-x)*1000;
    int negative = 1;

    if (value < 0) {
    	negative = -1;
    	value*=negative;
    }

    if (negative == -1) HAL_USART_Transmit(&USART_HandleStructure, (uint8_t *) "-", 1, 1000);
	printInt(x);
	printString(".");
	printInt(y);
}

void printString(char *c) {
    int length;
    length = strlen(c);
    
    HAL_USART_Transmit(&USART_HandleStructure, (uint8_t *)c, length, 1000);
    HAL_Delay(TRANS_DELAY);
}

void printComma() {
	char c = ',';

	HAL_USART_Transmit(&USART_HandleStructure, (uint8_t *)&c, 1, 1000);
	HAL_Delay(TRANS_DELAY);
}

void printNL() {
    char c = 0x0d;
    
    HAL_USART_Transmit(&USART_HandleStructure, (uint8_t *)&c, 1, 1000);
    HAL_Delay(TRANS_DELAY);
}
