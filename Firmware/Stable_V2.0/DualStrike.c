#include "DualStrike.h"

#include <avr/eeprom.h> /* EEPROM functions */
#include <avr/pgmspace.h>   /* required by usbdrv.h */

#ifndef uchar
#define uchar   unsigned char
#endif

// README
/* 
Configuration Mode
==================
In the configuration mode the behaviour of the Dual Strike can be changed,
see Startup Behaviour for how to enter it. Leave it by pressing Start. 
When first entering configuration mode, it is in USB configuration mode, 
which means it is ready to be updated from a PC.

If you want to change the configuration without a PC first press one of the
action buttons (any other than Start, Select and Home). Then pressing 
entering the following button-joystick combinations changes the 
configuration:

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

#define WORKING_MODE_PS3	0
#define WORKING_MODE_PT		1

// declares config and config_EEPROM
CFG_DECLARATION

#define enableUsbLines()	PORTD |= (1<<0)
#define disableUsbLines()	PORTD &= ~(1<<0);

void readConfig(uint8_t newConfig[CONFIG_BYTE_WIDTH + 2]) {
	 // read config from EEPROM
	eeprom_read_block(config, config_EEPROM, CONFIG_BYTE_WIDTH + 2);

	if(config[0] != CONFIG_DEVICE || config[1] != CONFIG_VERSION) {
		/* if EEPROM does not conform to current version and device set to default config */
        CONFIG_SET_DEFAULTS(newConfig)
	}
	else {
		for(int i = 0; i < (CONFIG_BYTE_WIDTH + 2); i++)
			newConfig[i] = config[i];
	}
}


uchar configsDiffer(uint8_t newConfig0[CONFIG_BYTE_WIDTH + 2]) {
	for(int i = 0; i < (CONFIG_BYTE_WIDTH + 2); i++)
		if(config[i] != newConfig0[i])
			return 1;

	return 0;
}

void writeConfig(uint8_t newConfig[CONFIG_BYTE_WIDTH + 2]) {
	if(configsDiffer(newConfig)) {
		/* if newConfig was changed update configuration */
		eeprom_write_block(newConfig, config_EEPROM, CONFIG_BYTE_WIDTH + 2);

		for(int i = 0; i < (CONFIG_BYTE_WIDTH + 2); i++)
			config[i] = newConfig[i];
	}
}

void configInit() {
	uint8_t newConfig[CONFIG_BYTE_WIDTH + 2];

	readConfig(newConfig);

	if(!Stick_Select) {
		/* enter configuration modification mode */

		enableUsbLines();
		programmer_setup();

		while( Stick_Start
		 	&& Stick_Jab
			&& Stick_Strong
			&& Stick_Fierce
			&& Stick_Short
			&& Stick_Forward
			&& Stick_Roundhouse
#ifdef EXTRA_BUTTONS
			&& Stick_Extra0
			&& Stick_Extra1
#endif
)
			programmer_poll();

		disableUsbLines();
		readConfig(newConfig);

		while(Stick_Start) {
			if(!Stick_Forward) {
				// revert to defaults
				CONFIG_SET_DEFAULTS(newConfig)
			}

			if(Stick_Short && Stick_Forward && Stick_Jab && Stick_Strong && Stick_Roundhouse) {
				if(!Stick_Up) {
					CFG_SET_DIGITAL_PAD(newConfig)
				}
				else if(!Stick_Left) {
					CFG_SET_LEFT_STICK(newConfig)
				}
				else if(!Stick_Right) {
					CFG_SET_RIGHT_STICK(newConfig)
				}
			}

			if(!Stick_Short) {
				if(!Stick_Left) {
					CFG_SET_DEF_WORK_MODE_PS3(newConfig)
				}
				if(!Stick_Right) {
					CFG_SET_DEF_WORK_MODE_PT(newConfig)
				}
			}
			
			if(!Stick_Jab) {
				if(!Stick_Left) {
					CFG_DISABLE_HOME_EMU(newConfig)
				}
				
				if(!Stick_Right) {
					CFG_ENABLE_HOME_EMU(newConfig)
				}
			}

			if(!Stick_Strong) {
				if(!Stick_Up) {
					CFG_SET_NO_EXTRA_PINS(newConfig)
				}
				else if(!Stick_Left) {
					CFG_SET_JOYSTICK_SWITCH_READ(newConfig)
				}
				else if(!Stick_Right) {
					CFG_SET_JOYSTICK_SWITCH_EMU(newConfig)
				}
				else if(!Stick_Down) {
					CFG_SET_INVERTED_TRIGGERS(newConfig)
				}
			}			
		}

	}
	
	writeConfig(newConfig);
}

int setModePS3() {
	enableUsbLines();

	return WORKING_MODE_PS3;
}

#include <util/delay.h>

int setModePT() {	
	if(CFG_HOME_EMU)
		SET_HOME_OUTPUT

	if(CFG_JOYSTICK_SWITCH_EMU || CFG_INVERTED_TRIGGERS) {
		DDRD |= (1<<4); // pin S3 is output
		DDRC |= (1<<6); // pin S4 is output
	}

	disconnectUSB();
	disableUsbLines();
	PORTD |= (1<<3); // enable pass-through usb lines

	return WORKING_MODE_PT;
}

// README
/*
Startup Behaviour
================
If a button or joystick direction is pressed, when the Dual Strike controller
is activated (if the machine it is plugged in is turned on or the controller gets
plugged into the machine), then special functions are activated:

If the Select button is pressed, then configuration mode is entered (see below).

If the Start button is pressed, then firmware update mode is entered (see below).

If the button A/Cross is pressed, then the PS3 mode is activated.
If the button B/Circle is pressed, then the XBox mode is activated.
If the button X/Square is pressed, then the pass-through mode is activated.
Otherwise the default working mode is activated.

If the joystick is moved to the up direction, the joystick is acting as a digital pad
when in Dual Strike working mode (default).
If the joystick is moved to the left direction, the joystick is acting as a left analogue
stick when in Dual Strike working mode.
If the joystick is moved to the right direction, the joystick is acting as a right analogue
stick when in Dual Strike working mode.
*/
int hardwareInit() {
	DDRC	= 0b00000000;	// PINC inputs
	PORTC	= 0b00111111;	// PORTC with pull-ups except S4

	DDRB	= 0b00000000;	// PINB inputs
	PORTB	= 0b00111111;	// PORTB with pull-ups except clock
	

	DDRD	= 0b00001001;  // PIND inputs, except S1 and S2
	PORTD	= 0b11100000;  // PORTD with pull-ups except S1, S2, D+, D- and S3

	configInit();

	if(!Stick_Up) // [precedence]
	{
		CFG_SET_DIGITAL_PAD(config)
	}
	else if(!Stick_Left) {
		CFG_SET_LEFT_STICK(config)
	}
	else if(!Stick_Right) {
		CFG_SET_RIGHT_STICK(config)
	}

	if(CFG_JOYSTICK_SWITCH_READ) {	
		PORTD |= (1<<4); // pin S3 is high
		PORTC |= (1<<6); // pin S4 is high
	}

	if(!Stick_Jab
	|| !Stick_Strong
	|| !Stick_Short
	|| !Stick_Forward) {
	 	if(CFG_DEF_WORK_MODE_PS3) {
			return setModePT();
		}
		else {
			return setModePS3();
		}
	}
	else {
		if(CFG_DEF_WORK_MODE_PS3) {
			return setModePS3();
		}
		else {
			return setModePT();
		}
	}
}

/* ------------------------------------------------------------------------- */

void readJoystickSwitch() {
    if(CFG_JOYSTICK_SWITCH_READ) {
        if(!(PIND & (1<<4)) && (PINC & (1<<6))) { // S3 low and S4 high
			CFG_SET_LEFT_STICK(config)
        }
        else if((PIND & (1<<4)) && !(PINC & (1<<6))) { // S3 high and S4 low
			CFG_SET_RIGHT_STICK(config)
        }
        else if((PIND & (1<<4)) && (PINC & (1<<6))) { // S3 high and S4 high
			CFG_SET_DIGITAL_PAD(config)
       }
    }
}

/* ------------------------------------------------------------------------- */

/* buffer for data */
/*
NOTE:
As V-USB conforms to the USB low speed standard, the maximum packet size is 8 bytes for one call of usbSetInterrupt()!
But it is possible to send longer reports by using
---
while(!usbInterruptIsReady()) usbPoll();
---
to wait until the data is sent, then you can send the next part like this:
---
usbSetInterrupt((uchar *)&data + 8, 1*sizeof(uchar));
---
*/
uchar* data[132];

int main(void)
{
	switch(hardwareInit()) {
	case WORKING_MODE_PS3:
	  ps3_controller();
	  break;

	case WORKING_MODE_PT:
	  pass_through();
	  break;
	}

    return 0;
}
