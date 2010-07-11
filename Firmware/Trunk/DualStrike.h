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

//PC Button 9 - Select
#define Stick_Select		(PINC & (1<<1))
//PC Button 10 - Start
#define Stick_Start			(PINC & (1<<0))

#if DUAL_STRIKE_SMD
	#define SET_HOME_OUTPUT DDRB |= (1<<5);
	#define SWITCH_HOME_LOW PORTB &= ~(1<<5);
	#define SWITCH_HOME_HIGH PORTB |= (1<<5);

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
	//PC Button 13 - Home
	#define Stick_Home			(PINB & (1<<5))

	#ifdef EXTRA_BUTTONS
		//PC Button 5 - L1 - 4P
		#define Stick_Extra0		(PINB & (1<<1))
		//PC Button 7 - L2 - 4K
		#define Stick_Extra1		(PINB & (1<<3))
	#endif
#else
	#define SET_HOME_OUTPUT DDRC |= (1<<5);
	#define SWITCH_HOME_LOW PORTC &= ~(1<<5);
	#define SWITCH_HOME_HIGH PORTC |= (1<<5);

	//PC Button 13 - Home
	#define Stick_Home			(PINC & (1<<5))

	#if EXTRA_BUTTONS
		//PC Button 7 - L2 - 4K
		#define Stick_Extra1		(PINC & (1<<4))
	#endif

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
		//PC Button 5 - L1 - 4P
		#define Stick_Extra0		(PINC & (1<<3))
	#endif
#endif

void readJoystickSwitch();

#endif
