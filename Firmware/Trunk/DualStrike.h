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

#define Stick_Select		(PINC & (1<<1))
#define Stick_Start			(PINC & (1<<0))

#if DUAL_STRIKE_SMD
	#define SET_HOME_OUTPUT DDRB	|=  (1<<5);
	#define SWITCH_HOME_LOW PORTB	&= ~(1<<5);
	#define SWITCH_HOME_HIGH PORTB	|=  (1<<5);
	#define Stick_Home	(PINB & (1<<5))

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
		#define Stick_4P	(PINB & (1<<1))
		#define Stick_4K	(PINB & (1<<3))
	#endif
#else
	#define SET_HOME_OUTPUT DDRC	|=  (1<<5);
	#define SWITCH_HOME_LOW PORTC	&= ~(1<<5);
	#define SWITCH_HOME_HIGH PORTC	|=  (1<<5);
	#define Stick_Home	(PINC & (1<<5))

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
		#define Stick_4P	(PINC & (1<<3))
		#define Stick_4K	(PINC & (1<<4))
	#endif
#endif

void readJoystickSwitch();
void readConfig(uint8_t newConfig[CONFIG_BYTE_WIDTH + 2]);
void writeConfig(uint8_t newConfig[CONFIG_BYTE_WIDTH + 2]);

#endif
