
void initADC();
uint32_t readADC(int channel);
uint32_t readBattery(void);
double rightSensorConversion(uint32_t reading);
double leftSensorConversion(uint32_t reading);
double rightCenterSensorConversion(uint32_t reading);
double leftCenterSensorConversion(uint32_t reading);
void sensorCalibrationPrint(int sensor, int distance);

int readRightSensor();
int readRightCenterSensor();
int readLeftCenterSesnor();
int readLeftSensor();

double rightSensorDistance(int value);
double leftSensorDistance(int value);
double rightCenterSensorDistance(int value);
double leftCenterSensorDistance(int value);

uint16_t ADC_getSampleAvgNDeleteX(uint8_t N, uint8_t X, int channel);
void Sort_tab(uint16_t tab[], uint8_t length);
//void HAL_ADC_ConvCpltCallback(ADC_HandleTypeDef* hadc);
