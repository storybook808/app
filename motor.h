#define PERIOD 1000

typedef enum
{
	LEFTMOTOR,
	RIGHTMOTOR
}Motor;

typedef enum
{
	FORWARD,
	BACKWARD
}Direction;

void setBuzzer(int state);
static void setDirection(Motor channel, Direction state);
int currentSpeed(Motor channel);
void setSpeed(Motor channel, int speed);
void travelDistance(uint32_t distance, uint32_t maxSpeed, uint32_t dt);
void brake();
void brakeCallBack();
void setVelocity(double velocity);
void setRightVelocity(double velocity);
void setLeftVelocity(double velocity);
void velocityCallBack();
double getCurrentVelocity(Motor channel);
double getTargetVelocity(Motor channel);
