/*=Public Functions===========================================================*/
#include <stdint.h>

void calibrateSensors(void);
void calibrateLeftWall(void);
void calibrateRightWall(void);
void calibrateFrontWall(void);
void calibrateCenter(void);
double getLeftWall(void);
double getRightWall(void);
double getIdealLeftFront(void);
double getIdealRightFront(void);
double getIdealLeftCenter(void);
double getIdealRightCenter(void);
double toLinear(uint16_t input);
void waitForTop(void);

/*=Private Functions==========================================================*/
static void setLeftWall(double value);
static void setRightWall(double value);
static void setIdealLeftFront(double value);
static void setIdealRightFront(double value);
static void setIdealLeftCenter(double value);
static void setIdealRightCenter(double value);
