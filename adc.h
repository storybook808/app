#include <stdint.h>
#define LEFT_DET      0
#define LEFT_CEN_DET  1
#define RIGHT_CEN_DET 2
#define RIGHT_DET     3
#define VOLT_DET      4
#define GYRO          5
#define FLASH_MEM	  6

void initADC();
uint32_t readADC(int channel);
uint32_t readBattery(void);
void sensorCalibrationPrint(int sensor, int distance);

double readRightSensor();
double readRightCenterSensor();
double readLeftCenterSensor();
double readLeftSensor();

double rightSensorDistance(int value);
double leftSensorDistance(int value);
double rightCenterSensorDistance(int value);
double leftCenterSensorDistance(int value);

uint16_t ADC_getSampleAvgNDeleteX(uint8_t N, uint8_t X, int channel);
void Sort_tab(uint16_t tab[], uint8_t length);
//void HAL_ADC_ConvCpltCallback(ADC_HandleTypeDef* hadc);
