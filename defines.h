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

#define LEFT_DET      0	// ADC Left Sensor
#define LEFT_CEN_DET  1	// ADC Center Left Sensor
#define RIGHT_CEN_DET 2	// ADC Center Right Sensor
#define RIGHT_DET     3	// ADC Right Sensor
#define VOLT_DET      4	// ADC Volt-meter of battery
#define GYRO          5	// ADC Gyroscope
#define FLASH		  6	// ADC Flash Memory
