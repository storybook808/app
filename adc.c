#include "components/coocox-master/STM32F405xx_cmsisboot/source/Hal/stm32f4xx_hal.h"
#include "components/coocox-master/STM32F405xx_cmsisboot/source/Hal/stm32f4xx_hal_adc.h"
#include "components/coocox-master/STM32F405xx_cmsisboot/source/Hal/stm32f4xx_hal_cortex.h"
#include "components/coocox-master/STM32F405xx_cmsisboot/source/Hal/stm32f4xx_hal_def.h"
#include "components/coocox-master/STM32F405xx_cmsisboot/source/Hal/stm32f4xx_hal_gpio.h"
#include "components/coocox-master/STM32F405xx_cmsisboot/source/Hal/stm32f4xx_hal_rcc.h"

#include "adc.h"
#include "led.h"

ADC_HandleTypeDef ADCHandle;

uint16_t uhADCxConvertedValue = 0;
int convertedValue[6];
int currentTarget;

int rightSensorreading[20] = {3648,3482,3204,2781,2468,2255,2098,1969,1877,1806,1742,1695,1652,1617,1588,1565,1546,130,1517,1506};
float rightSensorm[20] = {-0.00602,-0.00360,-0.00236,-0.00319,-0.00469,-0.00637,-0.00775,-0.01087,-0.01408,-0.01536,-0.02128,-0.02326,-0.02857,-0.03448,-0.04348,-0.05263,-0.06250,-0.07692,-0.09091,-0.1};
float rightSensorb[20] = {21.9759,13.52518,9.57447,11.88498,15.58685,19.36306,22.23357,28.40217,34.43662,37.21875,47.06383,50.41860,59.2,68.75862,83.04348,97.36842,112.625,134.69231,155.90909,169.6};
    
int leftSensorreading[20] = {3614,3438,2984,2482,2132,1877,1697,1548,1441,1365,1298,1247,1199,1164,1131,1104,1082,1060,1044,1029};
float leftSensorm[20] = {-0.00568,-0.0022,-0.00199,-0.00286,-0.00392,-0.00538,-0.00699,-0.00935,-0.01316,-0.01493,-0.01961,-0.02083,-0.02857,-0.0303,-0.03704,-0.04545,-0.04545,-0.06250,-0.06667,+0.06667};
float leftSensorb[20] = {20.53409,8.57269,7.94422,10.09143,12.36078,15.09140,17.82517,21.46729,26.96053,29.37313,35.45098,36.97917,46.25714,48.27273,55.88889,65.18182,65.18182,83.25,84.6,87.6,87.6};
    
int rightCenterSensorreading[20] = {4000,3868,3846,3791,3658,3338,2657,2208,1881,1615,1431,1295,1182,1095,1024,963,914,872,932,907,780,760};
float rightCenterSensorm[20] = {-.1,-.1-0.01818,-0.00752,-0.00313,-0.00147,-0.00223,-0.00306,-0.00376,-0.00543,-0.00735,-0.00885,-0.01149,-0.01408,-0.01639,-0.02041,-0.02381,-0.025,-0.04,-0.03704,-0.05,-0.00132};
float rightCenterSensorb[20] = {80,80,71.92727,31.50376,15.43125,9.90162,11.91759,13.75229,15.07143,17.77717,20.52206,22.46018,25.58621,28.42254,30.78689,34.65306,37.76190,38.8,51.28,48.88889,59,22};
    
int leftCenterSensorreading[20] = {3911,3884,3837,3767,3632,3466,3199,2853,2606,2401,2260,2155,2065,1998,1940,1896,1859,1828,1801,1778,1757,1742};
float leftCenterSensorm[20] = {-0.03704,-0.02128,-0.01429,-0.00741,-0.00602,-0.00375,-0.00289,-0.00405,-0.00488,-0.00709,-0.00952,-0.01111,-0.01493,-0.01724,-0.02273,-0.02703,-0.03226,-0.03704,-0.04348,-0.04762};
float leftCenterSensorb[20] = {144.85185,83.63830,56.81426,30.9037,25.87952,17.98127,15.24566,18.55061,20.7122,26.02837,31.52381,34.94444,42.8209,47.44828,58.09091,66.24324,75.96774,84.7037,96.30435,103.66667};

double rightSensorDistance(int value) {
    if (value > rightSensorreading[0]) {
        return 0;
    }
    double value2 = (float)value;
    int i;
    for (i = 0; i < 19; i++) {
        if (value < rightSensorreading[i] && value > rightSensorreading[i+1]) {
            return rightSensorm[i]*value2+rightSensorb[i];
        }
    }
    return 20.0;
}

double leftSensorDistance(int value) {
    if (value > leftSensorreading[0]) {
        return 0;
    }
    double value2 = (float)value;
    int i;
    for (i = 0; i < 19; i++) {
        if (value < leftSensorreading[i] && value > leftSensorreading[i+1]) {
            return leftSensorm[i]*value2+leftSensorb[i];
        }
    }
    return 20.0;
}

double rightCenterSensorDistance(int value) {
    if (value > rightCenterSensorreading[0]) {
        return 0;
    }
    double value2 = (float)value;
    int i;
    for (i = 0; i < 19; i++) {
        if (value < rightCenterSensorreading[i] && value > rightCenterSensorreading[i+1]) {
            return rightCenterSensorm[i]*value2+rightCenterSensorb[i];
        }
    }
    return 20.0;
}

double leftCenterSensorDistance(int value) {
    if (value > leftCenterSensorreading[0]) {
        return 0;
    }
    double value2 = (float)value;
    int i;
    for (i = 0; i < 19; i++) {
        if (value < leftCenterSensorreading[i] && value > leftCenterSensorreading[i+1]) {
            return leftCenterSensorm[i]*value2+leftCenterSensorb[i];
        }
    }
    return 20.0;
}

void initADC() {
    currentTarget = 0;
    convertedValue[0] = 0;
    convertedValue[1] = 0;
    convertedValue[2] = 0;
    convertedValue[3] = 0;
    convertedValue[4] = 0;
    convertedValue[5] = 0;
    
    //Data structure for GPIO configuration
    GPIO_InitTypeDef GPIO_InitStructure;
    
    ADC_ChannelConfTypeDef sConfig;
    
    //Enable GPIO clock for LED module (C & H)
    __GPIOA_CLK_ENABLE();
    __GPIOB_CLK_ENABLE();
    __GPIOC_CLK_ENABLE();
    __GPIOH_CLK_ENABLE();
    
    //Enable ADC clock
    __ADC1_CLK_ENABLE();
    
    //Configure data structure for GPIO output
    GPIO_InitStructure.Mode = GPIO_MODE_OUTPUT_PP;
    GPIO_InitStructure.Pull = GPIO_NOPULL;
    GPIO_InitStructure.Speed = GPIO_SPEED_MEDIUM;
    
    //Left emitter
    GPIO_InitStructure.Pin = GPIO_PIN_1;
    HAL_GPIO_Init(GPIOH, &GPIO_InitStructure);
    
    //Left center emitter
    GPIO_InitStructure.Pin = GPIO_PIN_15;
    HAL_GPIO_Init(GPIOC, &GPIO_InitStructure);
    
    //Right center emitter
    GPIO_InitStructure.Pin = GPIO_PIN_0;
    HAL_GPIO_Init(GPIOH, &GPIO_InitStructure);
    
    //Right emitter
    GPIO_InitStructure.Pin = GPIO_PIN_14;
    HAL_GPIO_Init(GPIOC, &GPIO_InitStructure);
    
    GPIO_InitStructure.Mode = GPIO_MODE_ANALOG;
    GPIO_InitStructure.Speed = GPIO_SPEED_HIGH;
    
    //Left detector
    GPIO_InitStructure.Pin = GPIO_PIN_0;
    HAL_GPIO_Init(GPIOB, &GPIO_InitStructure);
    
    //Left center detector
    GPIO_InitStructure.Pin = GPIO_PIN_7;
    HAL_GPIO_Init(GPIOA, &GPIO_InitStructure);
    
    //Right center detector
    GPIO_InitStructure.Pin = GPIO_PIN_4;
    HAL_GPIO_Init(GPIOA, &GPIO_InitStructure);
    
    //Right detector
    GPIO_InitStructure.Pin = GPIO_PIN_5;
    HAL_GPIO_Init(GPIOC, &GPIO_InitStructure);
    
    //Voltage detector
    GPIO_InitStructure.Pin = GPIO_PIN_1;
    HAL_GPIO_Init(GPIOB, &GPIO_InitStructure);

    //Flash detector
    GPIO_InitStructure.Pin = GPIO_PIN_4;
    HAL_GPIO_Init(GPIOC, &GPIO_InitStructure);
    
    //Gyroscope
    
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
    
    //	HAL_NVIC_EnableIRQ(ADC_IRQn);
    
    return;
}

uint32_t readBattery() {
    ADC_ChannelConfTypeDef sConfig;
    
    //Voltage detector
    sConfig.Channel = ADC_CHANNEL_9;
    sConfig.Rank = 1;
    sConfig.SamplingTime = ADC_SAMPLETIME_28CYCLES;
    sConfig.Offset = 0;
    HAL_ADC_ConfigChannel(&ADCHandle, &sConfig);
    
    HAL_ADC_Start(&ADCHandle);
    //Wait for conversion
    while(HAL_ADC_PollForConversion(&ADCHandle, HAL_MAX_DELAY) != HAL_OK);
    
    return HAL_ADC_GetValue(&ADCHandle);
}

uint32_t readADC(int channel) {
    ADC_ChannelConfTypeDef sConfig;
    int x = 14444;
    int i = x;
    
    switch (channel) {
        case LEFT_DET:
            //Left detector
            sConfig.Channel = ADC_CHANNEL_8;
            sConfig.Rank = 1;
            sConfig.SamplingTime = ADC_SAMPLETIME_28CYCLES;
            sConfig.Offset = 0;
            HAL_ADC_ConfigChannel(&ADCHandle, &sConfig);
            HAL_GPIO_WritePin(GPIOH, GPIO_PIN_1, GPIO_PIN_SET);
            break;
        case LEFT_CEN_DET:
            //Left center detector
            sConfig.Channel = ADC_CHANNEL_7;
            sConfig.Rank = 1;
            sConfig.SamplingTime = ADC_SAMPLETIME_28CYCLES;
            sConfig.Offset = 0;
            HAL_ADC_ConfigChannel(&ADCHandle, &sConfig);
            HAL_GPIO_WritePin(GPIOC, GPIO_PIN_15, GPIO_PIN_SET);
            break;
        case RIGHT_CEN_DET:
            //Right center detector
            sConfig.Channel = ADC_CHANNEL_4;
            sConfig.Rank = 1;
            sConfig.SamplingTime = ADC_SAMPLETIME_28CYCLES;
            sConfig.Offset = 0;
            HAL_ADC_ConfigChannel(&ADCHandle, &sConfig);
            HAL_GPIO_WritePin(GPIOH, GPIO_PIN_0, GPIO_PIN_SET);
            break;
        case RIGHT_DET:
            //Right detector
            sConfig.Channel = ADC_CHANNEL_15;
            sConfig.Rank = 1;
            sConfig.SamplingTime = ADC_SAMPLETIME_28CYCLES;
            sConfig.Offset = 0;
            HAL_ADC_ConfigChannel(&ADCHandle, &sConfig);
            HAL_GPIO_WritePin(GPIOC, GPIO_PIN_14, GPIO_PIN_SET);
            break;
        case FLASH:
            sConfig.Channel = ADC_CHANNEL_14;
            sConfig.Rank = 1;
            sConfig.SamplingTime = ADC_SAMPLETIME_28CYCLES;
            sConfig.Offset = 0;
            HAL_ADC_ConfigChannel(&ADCHandle, &sConfig);
            break;
    }
    // Wait 8us
    while(i--);
    i = x;
    
    HAL_ADC_Start(&ADCHandle);
    
    //Wait for conversion
    while(HAL_ADC_PollForConversion(&ADCHandle, HAL_MAX_DELAY) != HAL_OK);
    HAL_GPIO_WritePin(GPIOH, GPIO_PIN_1, GPIO_PIN_RESET);
    HAL_GPIO_WritePin(GPIOC, GPIO_PIN_15, GPIO_PIN_RESET);
    HAL_GPIO_WritePin(GPIOH, GPIO_PIN_0, GPIO_PIN_RESET);
    HAL_GPIO_WritePin(GPIOC, GPIO_PIN_14, GPIO_PIN_RESET);
    
    while(i--);
    
    return HAL_ADC_GetValue(&ADCHandle);
}

double rightSensorConversion(uint32_t x) {
    return 0;
}

double leftSensorConversion(uint32_t x) {
    return 0;
}

double rightCenterSensorConversion(uint32_t x) {
    return 0;
}

double leftCenterSensorConversion(uint32_t x) {
	return 0;
}

int readLeftSensor() {
	return ADC_getSampleAvgNDeleteX(30,10,LEFT_DET);
}

int readRightSensor() {
	return ADC_getSampleAvgNDeleteX(30,10,RIGHT_DET);
}

int readLeftCenterSensor() {
	return ADC_getSampleAvgNDeleteX(30,10,LEFT_CEN_DET);
}

int readRightCenterSensor() {
	return ADC_getSampleAvgNDeleteX(30,10,RIGHT_CEN_DET);
}

uint16_t ADC_getSampleAvgNDeleteX(uint8_t N, uint8_t X, int channel) {
    uint32_t avg_sample = 0x00;
    uint16_t adc_sample[N];
    uint8_t index = 0x00;
    
    for (index = 0x00; index<N; index++) {
        adc_sample[index] = 0x00;
    }
    
    for (index = 0x00; index<N; index++) {
        adc_sample[index] = readADC(channel);
    }
    
    Sort_tab(adc_sample, N);
    
    for (index=X/2; index<N-X/2; index++) {
        avg_sample += adc_sample[index];
    }
    
    avg_sample /= N-X;
    
    return avg_sample;
}

void Sort_tab(uint16_t tab[], uint8_t length) {
    uint8_t l = 0x00;
    uint8_t exchange = 0x01;
    uint16_t tmp=0x00;
    
    while (exchange==1) {
        exchange = 0;
        for (l = 0; l < length-1; l++) {
            if (tab[l] > tab[l+1]) {
                tmp = tab[1];
                tab[l] = tab[l+1];
                tab[l+1] = tmp;
                exchange = 1;
            }
        }
    }
}

//void HAL_ADC_ConvCpltCallback(ADC_HandleTypeDef* hadc) {
//
//	int ADCValue = HAL_ADC_GetValue(&ADCHandle);
//	HAL_ADC_Stop_IT(&ADCHandle);
//	return ADCValue;
//
//}
