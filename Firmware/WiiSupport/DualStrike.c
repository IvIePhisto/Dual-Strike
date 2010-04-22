#include "DualStrike.h"

#include <avr/eeprom.h> /* EEPROM functions */
#include <avr/pgmspace.h>   /* required by usbdrv.h */
#include "ps3/usbdrv/usbdrv.h"

uint8_t config = EEPROM_DEF;
uint8_t config_EEPROM EEMEM = CONFIG_DEF;

/* ------------------------------------------------------------------------- */

/* 
Configuration Mode
==================
In the configuration mode the behaviour of the Dual Strike can be changed,
see Startup Behaviour for how to enter it. Leave it by pressing Start.

While in configuration mode pressing a button and/or a joystick
direction, changes part of the configuration:

Dual Strike default stick mode:
-------------------------------
Up    = digital pad only (precedence over Left and Right) [default]
Left  = left analogue stick only
Right = right analogue stick only
Down  = activate digital pad additionallly to left or right analogue stick

Default Working Mode:
---------------------
Button: LK
Left  = Dual Strike [default]
Right = pass-through

revert to defaults:
-------------------
Button: MK

Start+Select=Home:
------------------
Button: LP
Left  = disabled [default]
Right = enabled

Extra Pins Mode
---------------
Button: MP
Up    = deactivated (precedence over Left and Right) [default]
Left  = read joystick mode switch (precedence over Down)
		S3 and S4 have to be connected to a triple switch
Right = emulate joystick mode switch for pass-through (precedence over Down)
        S3 and S4 have to be connected to joystick mode pins on the 
		pass-through PCB
Down  = inverted triggers for pass-through
        S3 and S4 have to be connected to trigger pins with active high on the
		pass-through PCB		
*/

int SwitchMode = -1;

void configInit() {
	uint8_t newConfig;

	config = eeprom_read_byte(&config_EEPROM); /* read config from EEPROM */

	if(config == EEPROM_DEF)
		/* if EEPROM is unitialized set to default config */
        newConfig = CONFIG_DEF;
	else
		newConfig = config;

	if(!Stick_Select) {
		/* enter configuration modification mode */
		usbDeviceDisconnect(); /* prevents unsuccessful initialization by host */

		while(Stick_Start) {
			if(!Stick_Up) {
				// Dual Strike digital pad: enabled
				newConfig |= (1<<2);
				// Dual Strike left stick: disabled
				newConfig &= ~(1<<1);
				// Dual Strike right stick: disabled
				newConfig &= ~(1<<3);
			}
			else if(!Stick_Left) {
				// Dual Strike digital pad: disabled
				newConfig &= ~(1<<2);
				// Dual Strike left stick: enabled
				newConfig |= (1<<1);
				// Dual Strike right stick: disabled
				newConfig &= ~(1<<3);
			}
			else if(!Stick_Right) {
				// Dual Strike digital pad: disabled
				newConfig &= ~(1<<2);
				// Dual Strike left stick: disabled
				newConfig &= ~(1<<1);
				// Dual Strike right stick: enabled
				newConfig |= (1<<3);
			}
			
			if(!Stick_Down)
				// Dual Strike digital pad: enabled
				newConfig |= (1<<2);

			if(!Stick_Short) {
#if USE_PS3
				if(!Stick_Left)
					// default working mode: Dual Strike
					newConfig &= ~(1<<0);
#endif

#if USE_PT
				if(!Stick_Right)
					// default working mode: pass-through
					newConfig |= (1<<0);
#endif
			}
			
			if(!Stick_Forward) {
				// revert to defaults
				newConfig = CONFIG_DEF;
			}
			
			if(!Stick_Jab) {
				if(!Stick_Left)
					// Start+Select=Home: disabled
					newConfig &= ~(1<<4);
				
				if(!Stick_Right)
					// Start+Select=Home: enabled
					newConfig |= (1<<4);
			}

			if(!Stick_Strong) {
				if(!Stick_Up) {
					// extra PINs mode: disabled
					newConfig &= ~(1<<5);
					newConfig &= ~(1<<6);
				}
				else if(!Stick_Left) {
					// extra PINs mode: read Joystick mode switch
					newConfig |= (1<<5);
					newConfig &= ~(1<<6);
				}
				else if(!Stick_Right) {
					// extra PINs mode: emulate Joystick mode switch for pass-through
					newConfig &= ~(1<<5);
					newConfig |= (1<<6);
				}
				else if(!Stick_Down) {
					// extra PINs mode: inverted triggers for pass-through
					newConfig |= (1<<5);
					newConfig |= (1<<6);
				}
			}
		}
	}

	if(newConfig != config) {
		/* if newConfig was changed update configuration */
		eeprom_write_byte(&config_EEPROM, newConfig);
		config = newConfig;
	}
}

#if USE_PS3
void setModePS3() {
	if(CFG_JOYSTICK_SWITCH_READ) {		
		PORTD |= (1<<4); // pin S3 is high
		PORTC |= (1<<6); // pin S4 is high
	}

	SwitchMode = 0;
}
#endif

#if USE_PT
void setModePT() {	
	if(CFG_HOME_EMU)
		SET_HOME_OUTPUT

	if(CFG_JOYSTICK_SWITCH_READ) {	
		PORTD |= (1<<4); // pin S3 is high
		PORTC |= (1<<6); // pin S4 is high
	}
	else if(CFG_JOYSTICK_SWITCH_EMU || CFG_INVERTED_TRIGGERS) {
		DDRD |= (1<<4); // pin S3 is output
		DDRC |= (1<<6); // pin S4 is output
	}

	PORTD &= ~(1<<0); // disable ps3 usb
	PORTD |= (1<<3); // enable pass-through usb

	SwitchMode = 2;
}
#endif

/*
Startup Behaviour
=================
If a button or joystick direction is pressed, when the Dual Strike controller
is activated (if the machine it is plugged in is turned on or the controller gets
plugged into the machine), then special functions are activated:

If the Select button is pressed, then configuration mode is entered (see below).

If the Start button is pressed, then firmware update mode is entered (see below).

If any other Button except Home is pressed, then the non default working mode is entered.
The working mode is either the Dual Strike acting as a controller (default) or pass-through (e.g.
a XBox360 controller PCB).

If the joystick is moved to the up direction, the joystick is acting as a digital pad
when in Dual Strike working mode (default).
If the joystick is moved to the left direction, the joystick is acting as a left analogue
stick when in Dual Strike working mode.
If the joystick is moved to the right direction, the joystick is acting as a right analogue
stick when in Dual Strike working mode.
*/
void HardwareInit() {
	DDRC	= 0b00000000;	// PINC inputs
	PORTC	= 0b00111111;	// PORTC with pull-ups except S4

	DDRB	= 0b00000000;	// PINC inputs
	PORTB	= 0b00111111;	// PORTB with pull-ups except clock
	

	DDRD	= 0b00001001;  // PINC inputs, except S1 and S2
	PORTD	= 0b11100001;  // PORTD with pull-ups except S2, D+, D- and S3

	configInit();

	if(!Stick_Up) // [precedence]
	{
		// Dual Strike digital pad: enabled
		config |= (1<<2);
		// Dual Strike left stick: disabled
		config &= ~(1<<1);
		// Dual Strike right stick: disabled
		config &= ~(1<<3);
	}
	else if(!Stick_Left) {
		// Dual Strike digital pad: disabled
		config &= ~(1<<2);
		// Dual Strike left stick: enabled
		config |= (1<<1);
		// Dual Strike right stick: disabled
		config &= ~(1<<3);
	}
	else if(!Stick_Right) {
		// Dual Strike digital pad: disabled
		config &= ~(1<<2);
		// Dual Strike left stick: disabled
		config &= ~(1<<1);
		// Dual Strike right stick: enabled
		config |= (1<<3);
	}
	
	if (   (!Stick_Jab)
		|| (!Stick_Strong) 
		|| (!Stick_Fierce)
		|| (!Stick_Short)
		|| (!Stick_Forward)
#ifdef EXTRA_BUTTONS
		|| (!Stick_Extra0)
#endif
		) {
		// if any punch or kick is held down, then set to non-default mode
#if USE_PT
#if USE_PS3
		if(CFG_DEF_WORK_MODE_DS)
			setModePT();
		else
			setModePS3();
#else
		setModePT();
#endif
#else
#if USE_PS3
		setModePS3();
#endif
#endif

	}
	else {
		// else set to default mode
#if USE_PS3
#if USE_PT
		if(CFG_DEF_WORK_MODE_DS)
			setModePS3();
		else
			setModePT();
#else
		setModePS3();
#endif
#else
#if USE_PT
		setModePT();
#endif
#endif

	}
}

/* ------------------------------------------------------------------------- */

int main(void)
{
	HardwareInit();

#if USE_PS3
	if(SwitchMode == 0)
	  ps3_controller();
#endif

#if USE_WII
	if(SwitchMode == 1)
	  wiimote_extension_controller();
#endif

#if USE_PT
	if(SwitchMode == 2)
	  pass_through();
#endif

    return 0;
}
