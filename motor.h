#define PERIOD 600

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

void initMotor(void);
void setBuzzer(int state);
void setDirection(Motor channel, Direction state);
int currentSpeed(Motor channel);
void setSpeed(Motor channel, int speed);
void toggleDirection(Motor channel);
void travelDistance(uint32_t distance, uint32_t maxSpeed, uint32_t dt);
