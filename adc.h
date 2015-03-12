#define LEFT_DET      0
#define LEFT_CEN_DET  1
#define RIGHT_CEN_DET 2
#define RIGHT_DET     3
#define VOLT_DET      4
#define GYRO          5
#define FLASH		  6

#define CALI_DIST 15.00

#define LEFT_MAX	3631
#define LEFT_MIN	1090
//#define LEFT_25
//#define LEFT_20
//#define LEFT_15
#define LEFT_10		1320
#define LEFT_5		1977

#define LEFT_CEN_MAX	3898
#define LEFT_CEN_MIN	1889
//#define LEFT_CEN_25
//#define LEFT_CEN_20
//#define LEFT_CEN_15
//#define LEFT_CEN_10
//#define LEFT_CEN_5

#define RIGHT_CEN_MAX	3886
#define RIGHT_CEN_MIN	1944
//#define RIGHT_CEN_25
//#define RIGHT_CEN_20
//#define RIGHT_CEN_15
//#define RIGHT_CEN_10
//#define RIGHT_CEN_5

#define RIGHT_MAX	3638
#define RIGHT_MIN	1594
//#define RIGHT_25
//#define RIGHT_20
//#define RIGHT_15
//#define RIGHT_10
//#define RIGHT_5

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
