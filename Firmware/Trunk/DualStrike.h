#ifndef __DualStrike_h_included__
#define __DualStrike_h_included__

#include "usb/usb_controller.h"
#include "pass-through/pass-through.h"

#ifndef ulong
#   define ulong    unsigned long
#endif
#ifndef uint
#   define uint     unsigned int
#endif
#ifndef uchar
#define uchar   unsigned char
#endif

// BUTTON DEFINITIONS

#ifndef ATMEGA_NO
#error ATmega number has to be defined.
#endif

#if ((ATMEGA_NO != 8) && (ATMEGA_NO != 168))
#error ATmega with unsupported number used.
#endif

#if (ATMEGA_NO == 8)
#include "configuration_atmega8.h"
#endif

#if (ATMEGA_NO == 168)
#include "configuration_atmega168.h"
#endif

// set to 1 for SMD version of the Dual Strike
#ifndef DUAL_STRIKE_SMD
#define DUAL_STRIKE_SMD 0
#endif

// set to 1 to use L1 and L2
#ifndef EXTRA_BUTTONS
#define EXTRA_BUTTONS 1
#endif


#if DUAL_STRIKE_SMD
	#define HOME_PORT_NAME 	B

	#define Stick_Up	(PINC & (1<<4))
	#define Stick_Down	(PINC & (1<<5))
	#define Stick_Left	(PINC & (1<<3))
	#define Stick_Right	(PINC & (1<<2))

	#define Stick_LP	(PIND & (1<<7))
	#define Stick_MP	(PINB & (1<<0))
	#define Stick_HP	(PINB & (1<<2))
	#define Stick_LK	(PIND & (1<<5))
	#define Stick_MK	(PIND & (1<<6))
	#define Stick_HK	(PINB & (1<<4))

	#ifdef EXTRA_BUTTONS
		#define EXTRA_4P_PIN			1
		#define EXTRA_4P_PORT_NAME 	B
		#define EXTRA_4K_PIN			3
		#define EXTRA_4K_PORT_NAME 	B
	#endif
#else
	#define HOME_PORT_NAME 	C

	#define Stick_Up	(PIND & (1<<6))
	#define Stick_Down	(PIND & (1<<7))
	#define Stick_Left	(PINB & (1<<0))
	#define Stick_Right	(PINB & (1<<1))

	#define Stick_LP	(PINB & (1<<2))
	#define Stick_MP	(PINB & (1<<5))
	#define Stick_HP	(PIND & (1<<5))
	#define Stick_LK	(PINB & (1<<3))
	#define Stick_MK	(PINB & (1<<4))
	#define Stick_HK	(PINC & (1<<2))

	#if EXTRA_BUTTONS
		#define EXTRA_4P_PIN			3
		#define EXTRA_4P_PORT_NAME 	C
		#define EXTRA_4K_PIN			4
		#define EXTRA_4K_PORT_NAME 	C
	#endif
#endif

#define CONCAT_EVAL(start, end) start##end
#define CONCAT(start, end) CONCAT_EVAL(start, end)

#if EXTRA_BUTTONS
	#define EXTRA_4P_PORT	CONCAT(PORT, EXTRA_4P_PORT_NAME)
	#define EXTRA_4P_DDR	CONCAT(DDR, EXTRA_4P_PORT_NAME)
	#define Stick_4P		(CONCAT(PIN, EXTRA_4P_PORT_NAME) & (1<<EXTRA_4P_PIN))

	#define EXTRA_4K_PORT	CONCAT(PORT, EXTRA_4K_PORT_NAME)
	#define EXTRA_4K_DDR	CONCAT(DDR, EXTRA_4K_PORT_NAME)
	#define Stick_4K		(CONCAT(PIN, EXTRA_4K_PORT_NAME) & (1<<EXTRA_4K_PIN))
#endif

#define HOME_PIN		5
#define HOME_PORT		CONCAT(PORT, HOME_PORT_NAME)
#define HOME_DDR		CONCAT(DDR, HOME_PORT_NAME)
#define Stick_Home		(CONCAT(PIN, HOME_PORT_NAME) & (1<<HOME_PIN))

#define Stick_Select	(PINC & (1<<1))
#define Stick_Start		(PINC & (1<<0))


#define S1_PORT	PORTD
#define S1_PIN	0

#define S2_PORT	PORTD
#define S2_PIN	3

#define S3_PIN			4
#define S3_PORT			PORTD
#define S3_DDR			DDRD
#define Stick_S3		(PIND & (1<<4))

#define S4_PIN			6
#define S4_PORT			PORTC
#define S4_DDR			DDRC
#define Stick_S4		(PINC & (1<<6))

#define START_STATE_VARIABLES \
extern uchar startPressed;\
extern uchar startWasUsed;\
extern uint startSendCount;\
extern uint startSendRepeats;\

void updateJoystickMode();
void updateStartState();
void readConfig(uint8_t newConfig[CONFIG_BYTE_WIDTH + 2]);
void writeConfig(uint8_t newConfig[CONFIG_BYTE_WIDTH + 2]);

#endif
