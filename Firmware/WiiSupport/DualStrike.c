#include "DualStrike.h"

#include <avr/eeprom.h> /* EEPROM functions */
#include <avr/pgmspace.h>   /* required by usbdrv.h */
#include "ps3/usbdrv/usbdrv.h"

uint8_t config[2] = {EEPROM_DEF, EEPROM_DEF};;
uint8_t config_EEPROM[2] EEMEM = {CONFIG_0_DEF, CONFIG_1_DEF};

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
Left  = Dual Strike PS3 [default]
Up    = Dual Strike Wii
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
	uint8_t newConfig[2];

	 // read config from EEPROM
	config[0] = eeprom_read_byte(&config_EEPROM[0]);
	config[1] = eeprom_read_byte(&config_EEPROM[1]);

	if(config[0] == EEPROM_DEF && config[1] == EEPROM_DEF) {
		/* if EEPROM is unitialized set to default config */
        newConfig[0] = CONFIG_0_DEF;
        newConfig[1] = CONFIG_1_DEF;
	}
	else {
		newConfig[0] = config[0];
		newConfig[1] = config[1];
	}

	if(!Stick_Select) {
		/* enter configuration modification mode */
		usbDeviceDisconnect(); /* prevents unsuccessful initialization by host */

		while(Stick_Start) {
			if(!Stick_Up) {
				ENABLE_CFG_DIGITAL_PAD
				DISABLE_CFG_LEFT_STICK
				DISABLE_CFG_RIGHT_STICK
			}
			else if(!Stick_Left) {
				DISABLE_CFG_DIGITAL_PAD
				ENABLE_CFG_LEFT_STICK
				DISABLE_CFG_RIGHT_STICK
			}
			else if(!Stick_Right) {
				DISABLE_CFG_DIGITAL_PAD
				DISABLE_CFG_LEFT_STICK
				ENABLE_CFG_RIGHT_STICK
			}
			
			if(!Stick_Down)
				ENABLE_CFG_DIGITAL_PAD

			if(!Stick_Short) {
#if USE_PS3
				if(!Stick_Left)
					SET_CFG_DEF_WORK_MODE_PS3
#endif

#if USE_PT
				if(!Stick_Right)
					SET_CFG_DEF_WORK_MODE_PT
#endif

#if USE_WII
				if(!Stick_Up)
					SET_CFG_DEF_WORK_MODE_WII
#endif
			}
			
			if(!Stick_Forward) {
				// revert to defaults
				newConfig[0] = CONFIG_0_DEF;
				newConfig[1] = CONFIG_1_DEF;
			}
			
			if(!Stick_Jab) {
				if(!Stick_Left)
					DISABLE_CFG_HOME_EMU
				
				if(!Stick_Right)
					ENABLE_CFG_HOME_EMU
			}

			if(!Stick_Strong) {
				if(!Stick_Up) {
					SET_CFG_NO_EXTRA_PINS
				}
				else if(!Stick_Left) {
					SET_CFG_JOYSTICK_SWITCH_READ
				}
				else if(!Stick_Right) {
					SET_CFG_JOYSTICK_SWITCH_EMU
				}
				else if(!Stick_Down) {
					SET_CFG_INVERTED_TRIGGERS
				}
			}
		}
	}

	if(newConfig != config) {
		/* if newConfig was changed update configuration */
		eeprom_write_byte(&config_EEPROM[0], newConfig[0]);
		eeprom_write_byte(&config_EEPROM[1], newConfig[1]);
		config[0] = newConfig[0];
		config[1] = newConfig[1];
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

If the X/Cross button is pressed, then the Dual Strike PS3 working mode is activated.
If the Y/Circle button is pressed, then the Dual Strike Wii working mode is activated.
If the A/Squard button is pressed, then the pass-through working mode is activated.

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
		ENABLE_CFG_DIGITAL_PAD
		DISABLE_CFG_LEFT_STICK
		DISABLE_CFG_RIGHT_STICK
	}
	else if(!Stick_Left) {
		DISABLE_CFG_DIGITAL_PAD
		ENABLE_CFG_LEFT_STICK
		DISABLE_CFG_RIGHT_STICK
	}
	else if(!Stick_Right) {
		DISABLE_CFG_DIGITAL_PAD
		DISABLE_CFG_LEFT_STICK
		ENABLE_CFG_RIGHT_STICK
	}

#if USE_PS3
	if(!Stick_Short) {
		setModePS3();
		return;
	}
#endif

#if USE_WII
	if(!Stick_Forward) {
		setModeWii();
		return;
	}
#endif

#if USE_PT
	if(!Stick_Jab) {
		setModePT();
		return;
	}
#endif

#if USE_PS3
	if(CFG_DEF_WORK_MODE_PS3) {
		setModePS3();
		return;
	}
#endif

#if USE_WII
	if(CFG_DEF_WORK_MODE_WII) {
		setModeWii();
		return;
	}
#endif

#if USE_PT
	if(CFG_DEF_WORK_MODE_PT) {
		setModePT();
		return;
	}
#endif
}

/* ------------------------------------------------------------------------- */

void readJoystickSwitch() {
    if(CFG_JOYSTICK_SWITCH_READ) {
        if(!(PIND & (1<<4)) && (PINC & (1<<6))) { // S3 low and S4 high
			DISABLE_CFG_DIGITAL_PAD
			ENABLE_CFG_LEFT_STICK
			DISABLE_CFG_RIGHT_STICK
        }
        else if((PIND & (1<<4)) && !(PINC & (1<<6))) { // S3 high and S4 low
			DISABLE_CFG_DIGITAL_PAD
			DISABLE_CFG_LEFT_STICK
			ENABLE_CFG_RIGHT_STICK
        }
        else if((PIND & (1<<4)) && (PINC & (1<<6))) { // S3 high and S4 high
			ENABLE_CFG_DIGITAL_PAD
			DISABLE_CFG_LEFT_STICK
			DISABLE_CFG_RIGHT_STICK
        }
        else if(!(PIND & (1<<4)) && !(PINC & (1<<6))) { // S3 low and S4 low
			DISABLE_CFG_DIGITAL_PAD
			DISABLE_CFG_LEFT_STICK
			DISABLE_CFG_RIGHT_STICK
        }
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
