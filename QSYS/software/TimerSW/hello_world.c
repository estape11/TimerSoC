/*
 *******************************************************************

 Instituto Tecnológico de Costa Rica
 Computer Engineering Academic Area

 Programmer: Esteban Agüero Pérez (estape11)
 Language: C
 Version: 1.0
 Last Update: 02/05/2019

 Embedded Systems Introduction
 Professor. Jeferson González Gómez

 *******************************************************************
 */

#include <unistd.h>
#include <stdbool.h>
#include <sys/alt_irq.h>

#define ZERO 0x40
#define ONE 0xF9
#define TWO 0x24
#define THREE 0x30
#define FOUR 0x19
#define FIVE 0x12
#define SIX 0x02
#define SEVEN 0x78
#define EIGHT 0x00
#define NINE 0x18
#define F 0x0E
#define N 0x48
#define OFF 0xFF

// Types definitions
struct timeStruct {
	int hourTens;
	int hourUnits;
	int minTens;
	int minUnits;
	int seconds;
};

typedef unsigned char uchar;
typedef struct timeStruct time;

// HW pointers
volatile int *displays = (int *) 0x29060; // displays address
volatile int *switches = (int *) 0x29050; // switches address
volatile int *buttons = (int *) 0x29040; // buttons address
volatile int *gpioOut = (int *) 0x29090; // gpio out address
volatile int *leds = (int *) 0x29120; // gpio out address
volatile uchar *timerCoreStatus = (uchar *) (TIMERCORE_BASE); // timer core status address

// Clock variables
time actualTime;
time alarmTime;

// Control variables
bool switchingAlarm = false;
bool settingAlarm = false;
bool alarmStatus = false;

// Alarm visualization
int ledsValue = 0x1;
int counter = 0;
bool left = true;

/**
 * Converts a integer value to 7 segment display format
 */
int ToHexa(int value) {
	switch (value) {
	case 0:
		return ZERO;
	case 1:
		return ONE;
	case 2:
		return TWO;
	case 3:
		return THREE;
	case 4:
		return FOUR;
	case 5:
		return FIVE;
	case 6:
		return SIX;
	case 7:
		return SEVEN;
	case 8:
		return EIGHT;
	case 9:
		return NINE;
	case 10:
		return F;
	case 11:
		return N;
	case 12:
		return OFF;
	default:
		return ZERO;
	}
}

/**
 * Takes 4 values and encoded them to be set it in the 7 segments displays
 */
int EncodeHexa(int hex3, int hex2, int hex1, int hex0) {
	return (ToHexa(hex3) << 0x18) | (ToHexa(hex2) << 0x10) | (ToHexa(hex1) << 8)
			| (ToHexa(hex0));
}

/**
 * Update the hour
 */
void UpdateTime(void) {
	if (actualTime.seconds >= 60) { //45
		actualTime.minUnits++;
		actualTime.seconds = 0x0;

	}

	if (actualTime.minUnits >= 10) {
		actualTime.minTens++;
		actualTime.minUnits = 0x0;

	}

	if (actualTime.minTens >= 6) {
		actualTime.hourUnits++;
		actualTime.minTens = 0x0;

	}

	if (actualTime.hourUnits >= 10) {
		actualTime.hourTens++;
		actualTime.hourUnits = 0x0;

	}

	if (actualTime.hourTens >= 2 && actualTime.hourUnits >= 4) {
		actualTime.hourTens = 0x0;
		actualTime.hourUnits = 0x0;

	}

}

/*
 * Refresh the time on the displays
 */
void UpdateTimeDisplays(void) {
	if (settingAlarm) {
		*displays = EncodeHexa(alarmTime.hourTens, alarmTime.hourUnits,
				alarmTime.minTens, alarmTime.minUnits);

	} else if (switchingAlarm) {
		if (alarmStatus) {
			// Put ON on the displays
			*displays = EncodeHexa(12, 12, 0, 11);

		} else {
			// Put OFF on the displays
			*displays = EncodeHexa(12, 0, 10, 10);

		}

	} else {
		*displays = EncodeHexa(actualTime.hourTens, actualTime.hourUnits,
				actualTime.minTens, actualTime.minUnits);

	}
}

/**
 * Handler for the Timer Core interruptions
 */
static void HandlerTimerCoreInterrupt(void* context) {
	*timerCoreStatus = 0; // Resets the status
	actualTime.seconds++;
	UpdateTime();

}

/**
 * Setup the Timer Core
 */
static void SetupTimerCore(void) {
	// Getting the address
	uchar *timerCoreControl = (uchar *) (TIMERCORE_BASE + 0x4);
	uchar *timerCoreMask = (uchar *) (TIMERCORE_BASE + 0x8);
	uchar *timerCoreEdgeCap = (uchar *) (TIMERCORE_BASE + 0xC);

	// Sets the interrupt handler
	alt_ic_isr_register(TIMERCORE_IRQ_INTERRUPT_CONTROLLER_ID,
	TIMERCORE_IRQ, HandlerTimerCoreInterrupt, NULL, 0x0);

	// Starts the timer
	*timerCoreMask = 0x0;
	*timerCoreEdgeCap = 0x4;
	*timerCoreControl = 0x0007;
	*timerCoreStatus = 0;

}

/**
 * Animates the alarm notification
 */
void LedsUpdate(void) {
	*leds = ledsValue;
	if (left) {
		ledsValue <<= 0x1;
		counter++;

	} else {
		ledsValue >>= 0x1;
		counter--;

	}

	if (counter > 8) {
		left = false;

	} else if (counter < 1) {
		left = true;

	}

}

/**
 * Main function of the clock
 */
int main() {
	*gpioOut = 0x00;
	SetupTimerCore();
	*leds = 0x0;

	while (1) {
		if (*switches == 0x1) {
			*leds = 0x1;
			if (*buttons == 0x1) {
				actualTime.hourUnits++;

			} else if (*buttons == 0x2) {
				actualTime.minUnits++;
			}

			if (actualTime.minUnits >= 10) {
				actualTime.minTens++;
				actualTime.minUnits = 0x0;
			}

			if (actualTime.minTens >= 6) {
				actualTime.minTens = 0x0;
			}

			if (actualTime.hourUnits >= 10) {
				actualTime.hourTens++;
				actualTime.hourUnits = 0x0;
			}

			if (actualTime.hourTens >= 2 && actualTime.hourUnits >= 4) {
				actualTime.hourTens = 0x0;
				actualTime.hourUnits = 0x0;
			}
			usleep(150000); // To avoid multiples increments

		} else if (*switches == 0x2) {
			*leds = 0x2;
			settingAlarm = true;
			if (*buttons == 0x1) {
				alarmTime.hourUnits++;

			} else if (*buttons == 0x2) {
				alarmTime.minUnits++;

			}

			if (alarmTime.minUnits >= 10) {
				alarmTime.minTens++;
				alarmTime.minUnits = 0x0;

			}

			if (alarmTime.minTens >= 6) {
				alarmTime.minTens = 0x0;

			}

			if (alarmTime.hourUnits >= 10) {
				alarmTime.hourTens++;
				alarmTime.hourUnits = 0x0;

			}

			if (alarmTime.hourTens >= 2 && alarmTime.hourUnits >= 4) {
				alarmTime.hourTens = 0x0;
				alarmTime.hourUnits = 0x0;

			}
			usleep(150000); // To avoid multiples increments

		} else if (*switches == 0x3) {
			*leds = 0x3;
			switchingAlarm = true;
			if (*buttons == 0x2) {
				if (alarmStatus) {
					alarmStatus = false;

				} else {
					alarmStatus = true;

				}
			}
			usleep(150000); // To avoid multiples changes

		} else {
			if (alarmStatus) {
				if (alarmTime.hourTens == actualTime.hourTens
						&& alarmTime.hourUnits == actualTime.hourUnits
						&& alarmTime.minTens == actualTime.minTens
						&& alarmTime.minUnits == actualTime.minUnits) {
					// Make the sound
					*gpioOut = 0x01;
					LedsUpdate();
					usleep(150000);
					*gpioOut = 0x00;
					LedsUpdate();
					usleep(80000);

					// To disable the alarm
					if (*buttons == 0x2) {
						alarmStatus = false;
						*leds = 0;

					}
				}
			} // Alarm not set
			settingAlarm = false;
			switchingAlarm = false;
			*leds = 0;

		}
		UpdateTimeDisplays();
		usleep(1000);
	}
	return 0;
}
