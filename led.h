typedef enum
{
	WHITE,
	BLUE,
	GREEN,
	RED
}Led;

#define ON  1
#define OFF 0

void initLED(void);
void setLED(Led color);
void resetLED(Led color);
void setLEDAll(void);
void resetLEDAll(void);
void toggleLED(Led color);
void toggleLEDAll(void);
