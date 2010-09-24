#ifndef __DualStrike_h_included__
#define __DualStrike_h_included__

#include "usb/usb_controller.h"
#include "pass-through/pass-through.h"
#include "configuration.h"

// BUTTON DEFINITIONS

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

	#define Stick_Up			(PINC & (1<<4))
	#define Stick_Down			(PINC & (1<<5))
	#define Stick_Left			(PINC & (1<<3))
	#define Stick_Right			(PINC & (1<<2))

	//PC Button 1 - Square - LP
	#define Stick_Jab			(PIND & (1<<7))
	//PC Button 4 - Triangle - MP
	#define Stick_Strong		(PINB & (1<<0))
	//PC Button 6 - R1 - HP
	#define Stick_Fierce		(PINB & (1<<2))
	//PC Button 2 - Cross - LK
	#define Stick_Short			(PIND & (1<<5))
	//PC Button 3 - Circle - MK
	#define Stick_Forward		(PIND & (1<<6))
	//PC Button 8 - R2 - HK
	#define Stick_Roundhouse	(PINB & (1<<4))

	#ifdef EXTRA_BUTTONS
		#define EXTRA_4P_PIN		1
		#define EXTRA_4P_PORT_NAME 	B
		#define EXTRA_4K_PIN		3
		#define EXTRA_4K_PORT_NAME 	B
	#endif
#else
	#define HOME_PORT_NAME 	C

	#define Stick_Up			(PIND & (1<<6))
	#define Stick_Down			(PIND & (1<<7))
	#define Stick_Left			(PINB & (1<<0))
	#define Stick_Right			(PINB & (1<<1))

	//PC Button 1 - Square - LP
	#define Stick_Jab			(PINB & (1<<2))
	//PC Button 4 - Triangle - MP
	#define Stick_Strong		(PINB & (1<<5))
	//PC Button 6 - R1 - HP
	#define Stick_Fierce		(PIND & (1<<5))
	//PC Button 2 - Cross - LK
	#define Stick_Short			(PINB & (1<<3))
	//PC Button 3 - Circle - MK
	#define Stick_Forward		(PINB & (1<<4))
	//PC Button 8 - R2 - HK
	#define Stick_Roundhouse	(PINC & (1<<2))

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
	#define Stick_Extra0	(CONCAT(PIN, EXTRA_4P_PORT_NAME) & (1<<EXTRA_4P_PIN))

	#define EXTRA_4K_PORT	CONCAT(PORT, EXTRA_4K_PORT_NAME)
	#define EXTRA_4K_DDR	CONCAT(DDR, EXTRA_4K_PORT_NAME)
	#define Stick_Extra1	(CONCAT(PIN, EXTRA_4K_PORT_NAME) & (1<<EXTRA_4K_PIN))
#endif

#define HOME_PIN		5
#define HOME_PORT		CONCAT(PORT, HOME_PORT_NAME)
#define HOME_DDR		CONCAT(DDR, HOME_PORT_NAME)
#define Stick_Home		(CONCAT(PIN, HOME_PORT_NAME) & (1<<HOME_PIN))

#define Stick_Select	(PINC & (1<<1))
#define Stick_Start		(PINC & (1<<0))

#define S3_PIN			4
#define S3_PORT			PORTD
#define S3_DDR			DDRD
#define Stick_S3		(PIND & (1<<4))

#define S4_PIN			6
#define S4_PORT			PORTC
#define S4_DDR			DDRC
#define Stick_S4		(PINC & (1<<6))

void readJoystickSwitch();

#endif
