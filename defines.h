/* File Name: defines.h
 * Project: EE 396
 * Team: TSM
 * Members: Mark Miw
 *          Steven Chen
 *          Joseph Felix Jr.
 */

#ifndef defines_H
#define defines_H

#define PR	.01		// P constant for correction on right motor
#define PL	.01		// P constant for correction on left motor
#define DR	.005	// D constant for correction on right motor
#define DL	.005	// D constant for correction on left motor
#define BR	.0003	// B constant for brake system on right motor
#define BL	.0005	// B constant for brake system on left motor

#define TIMEOUT 100

#define ON  1		// Used for on/off states
#define OFF 0		// Used for on/off states

#define PERIOD 1000	// Motor period

#define TRANS_DELAY 200	// Delay between USART transmissions

#define BREAK_k	50					// Brake slow-down/speedup rate
#define brakeThresh	100				// Brake time wait in milliseconds
#define CORRECTION_FRONT_THRESH	10	// Threshold for stopping infront of walls
#define TURN_THRESH		5
#define R_ENCODER_DIST	0.00200001	// Right encoder tick per cm 0.0020000 is left bias
#define L_ENCODER_DIST	0.00205		// Left encoder tick per cm
#define VELOCITY_k_R	25			// Velocity slow-down/speedup rate right motor
#define VELOCITY_k_L	25			// Velocity slow-down/speedup rate left motor
#define BRAKE_k_R		50
#define BRAKE_k_L		50
#define CELL_R			4395
#define CELL_L			4375
#define TURN_R			1470
#define TURN_L			1515
#define SLOW			15
#define	UPPER_BOUND		0.2
#define ACCEL_RATE		250			// Acceleration time in milliseconds

/* Calibration readings
 *
 * CenterRightFront = 634.849
 * CenterLeftFront = 618.250
 *
 * IdealRightFront = 781.218
 * IdealLeftFront = 750.397
 *
 * IdealRightCenter = 805.597
 * IdealLeft Center = 831.432
 *
 * FarRightWall = 908.882
 * FarLeftWall = 978.93
 */

/* Global Variables */
uint32_t row[16];

typedef struct walls {
	bool south;
	bool west;
} walls;

typedef struct cell {
	bool north;
	bool east;
	bool south;
	bool west;
} cell;

walls map[16][16];

double calibration[2];

/* Enumeration Variable for ADC Channels */
typedef enum
{
	LEFT_DET,
	LEFT_CEN_DET,
	RIGHT_CEN_DET,
	RIGHT_DET,
	VOLT_DET,
	GYRO1,
	GYRO2,
	FLASH_MEM
}ADC_Channel;

typedef enum
{
	RIGHTWALL,
	LEFTWALL,
	NOWALL
}Wall_Correction;

typedef enum {
    LEFT,
    RIGHT,
    CENTER,
    FRONT,
    FRONTCENTER,
    FARLEFTWALL,
    FARRIGHTWALL,
    IDEALLEFTFRONT,
    IDEALRIGHTFRONT,
    IDEALLEFTCENTER,
    IDEALRIGHTCENTER,
    CENTERRIGHTFRONT,
    CENTERLEFTFRONT
}Wall;

/* Enumeration Variable for Encoder Channels */
typedef enum
{
	LEFTENCODER,
	RIGHTENCODER
}Encoder;

/* Enumeration for Motor Channels */
typedef enum
{
	LEFTMOTOR,
	RIGHTMOTOR
}Motor;

/* Enumeration for LEDs */
typedef enum
{
	WHITE,
	BLUE,
	GREEN,
	RED
}Led;

/* Enumeration for Motor Direction */
typedef enum
{
	FORWARD,
	BACKWARD
}Direction;

/* Enumeration for Modes */
typedef enum
{
	MODE1,
	MODE2,
	MODE3,
	MODE4
}Mode;

/* Enumeration for Buzzer */
typedef enum
{
	C7,
	D7,
	E7,
	F7,
	G7,
	A7,
	B7,
	C8,
	D8,
	E8,
	F8,
	G8,
	A8,
	B8,
	C9
}Note;

#endif
