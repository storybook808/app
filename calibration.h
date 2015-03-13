/*=Public Functions===========================================================*/
void calibrateSensors(void);
void calibrateLeftWall(void);
void calibrateRightWall(void);
void calibrateFrontWall(void);
void calibrateCenter(void);
int getLeftWall(void);
int getRightWall(void);
int getIdealLeftFront(void);
int getIdealRightFront(void);
int getIdealLeftCenter(void);
int getIdealRightCenter(void);
double toLinear(int input);
void waitForTop(void);

/*=Private Functions==========================================================*/
static void setLeftWall(int value);
static void setRightWall(int value);
static void setIdealLeftFront(int value);
static void setIdealRightFront(int value);
static void setIdealLeftCenter(int value);
static void setIdealRightCenter(int value);
