#ifndef __DualStrike_h_included__
#define __DualStrike_h_included__

#define USE_PS3 1
#define USE_WII 0
#define USE_PT 1

#if USE_PS3
#include "ps3/ps3_controller.h"
#endif

#if USE_WII
#include "wii/wiimote_extension_controller.h"
#endif

#if USE_PT
#include "pass-through/pass-through.h"
#endif

#define EEPROM_DEF 0xFF /* for uninitialized EEPROMs */

/*
config byte description by bits:
--------------------------------
0:   default working mode (0 == Dual Strike; 1 == pass-through
1:   Dual Strike left stick (0 == deactivated; 1 == activated)
2:   Dual Strike digital pad (0 == deactivated; 1 == activated)
3:   Dual Strike right stick (0 == deactivated; 1 == activated)
4:   Start+Select=Home (0 == disabled, 1 == enabled)
5-6: extra PINs mode
     (00 == deactivated,
	  10 == read Joystick mode switch,
	  01 == emulate Joystick mode switch for pass-through,
	  11 == inverted triggers for pass-through)
*/

#define CONFIG_DEF 0b00000100 /* default config */

// test configuration: default working mode == Dual Strike
#define CFG_DEF_WORK_MODE_DS 	!(config & (1<<0))
// test configuration:  Dual Strike left stick == enabled
#define CFG_LEFT_STICK 			(config & (1<<1))
// test configuration:  Dual Strike digital pad == enabled
#define CFG_DIGITAL_PAD		    (config & (1<<2))
// test configuration:  Dual Strike right stick == enabled
#define CFG_RIGHT_STICK			(config & (1<<3))
// test configuration: Start+Select=Home == enabled
#define CFG_HOME_EMU		 	(config & (1<<4))
// test configuration: extra PINs mode == read Joystick mode switch
#define CFG_JOYSTICK_SWITCH_READ	((config & (1<<5)) && !(config & (1<<6)))
// test configuration: extra PINs mode == emulate Joystick mode switch for pass-through
#define CFG_JOYSTICK_SWITCH_EMU		(!(config & (1<<5) && (config & (1<<6))))
// test configuration: extra PINs mode == inverted triggers for pass-through
#define CFG_INVERTED_TRIGGERS		((config & (1<<5)) && (config & (1<<6)))

// set to 1 for SMD version of the Dual Strike
#define DUAL_STRIKE_SMD 0

// set to 1 to use L1 and L2
#define EXTRA_BUTTONS 1

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
	//PC Button 13 - Home
	#define Stick_Home			(PINC & (1<<5))

	#if EXTRA_BUTTONS
		//PC Button 5 - L1 - 4P
		#define Stick_Extra0		(PINC & (1<<3))
		//PC Button 7 - L2 - 4K
		#define Stick_Extra1		(PINC & (1<<4))
	#endif
#endif

#endif
