#ifndef defines_H
#define defines_H

#define PR	.01		// P constant for correction on right motor
#define PL	.01		// P constant for correction on left motor
#define DR	.005	// D constant for correction on right motor
#define DL	.005	// D constant for correction on left motor
#define BR	.0003	// B constant for brake system on right motor
#define BL	.0005	// B constant for brake system on left motor

#define ON  1		// Used for on/off states
#define OFF 0		// Used for on/off states

#define PERIOD 1000	// Motor period

#define TRANS_DELAY 200	// Delay between USART transmissions

/* Enumeration Variable for ADC Channels */
typedef enum
{
	LEFT_DET,
	LEFT_CEN_DET,
	RIGHT_CEN_DET,
	RIGHT_DET,
	VOLT_DET,
	GYRO,
	FLASH_MEM
}ADC_Channel;

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

#endif
