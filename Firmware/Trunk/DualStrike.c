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
Up    = XBox
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
#define WORKING_MODE_XBOX	1
#define WORKING_MODE_PT		2
#define WORKING_MODE_WII	3


uint8_t config[2] = {EEPROM_DEF, EEPROM_DEF};
uint8_t config_EEPROM[2] EEMEM = {CONFIG_0_DEF, CONFIG_1_DEF};

#define enableUsbLines()	PORTD |= (1<<0)
#define disableUsbLines()	PORTD &= ~(1<<0);

void configInit() {
	uint8_t newConfig[2] = {0,0};

	 // read config from EEPROM
	eeprom_read_block(config, config_EEPROM, 2);

	if(config[0] == EEPROM_DEF || config[1] == EEPROM_DEF) {
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
		enableUsbLines();
		programmer_setup();

		while(Stick_Start) {
			programmer_poll();

			if(!Stick_Up) {
				ENABLE_CFG_DIGITAL_PAD(newConfig)
				DISABLE_CFG_LEFT_STICK(newConfig)
				DISABLE_CFG_RIGHT_STICK(newConfig)
			}
			else if(!Stick_Left) {
				DISABLE_CFG_DIGITAL_PAD(newConfig)
				ENABLE_CFG_LEFT_STICK(newConfig)
				DISABLE_CFG_RIGHT_STICK(newConfig)
			}
			else if(!Stick_Right) {
				DISABLE_CFG_DIGITAL_PAD(newConfig)
				DISABLE_CFG_LEFT_STICK(newConfig)
				ENABLE_CFG_RIGHT_STICK(newConfig)
			}
			
			if(!Stick_Down)
				ENABLE_CFG_DIGITAL_PAD(newConfig)

			if(!Stick_Short) {
#if USE_PS3
				if(!Stick_Left)
					SET_CFG_DEF_WORK_MODE_PS3(newConfig)
#endif

#if USE_XBOX
				if(!Stick_Up)
					SET_CFG_DEF_WORK_MODE_XBOX(newConfig)
#endif

#if USE_PT
				if(!Stick_Right)
					SET_CFG_DEF_WORK_MODE_PT(newConfig)
#endif
			}
			
			if(!Stick_Forward) {
				// revert to defaults
				newConfig[0] = CONFIG_0_DEF;
				newConfig[1] = CONFIG_1_DEF;
			}
			
			if(!Stick_Jab) {
				if(!Stick_Left)
					DISABLE_CFG_HOME_EMU(newConfig)
				
				if(!Stick_Right)
					ENABLE_CFG_HOME_EMU(newConfig)
			}

			if(!Stick_Strong) {
				if(!Stick_Up) {
					SET_CFG_NO_EXTRA_PINS(newConfig)
				}
				else if(!Stick_Left) {
					SET_CFG_JOYSTICK_SWITCH_READ(newConfig)
				}
				else if(!Stick_Right) {
					SET_CFG_JOYSTICK_SWITCH_EMU(newConfig)
				}
				else if(!Stick_Down) {
					SET_CFG_INVERTED_TRIGGERS(newConfig)
				}
			}
		}

		disableUsbLines();
	}

	if(newConfig[0] != config[0] || newConfig[1] != config[1]) {
		/* if newConfig was changed update configuration */
		eeprom_write_block(newConfig, config_EEPROM, 2);
		config[0] = newConfig[0];
		config[1] = newConfig[1];
	}
}

#if USE_PS3
int setModePS3() {
	enableUsbLines();

	return WORKING_MODE_PS3;
}
#endif

#if USE_XBOX
int setModeXBox() {
	enableUsbLines();

	return WORKING_MODE_XBOX;
}
#endif

#if USE_WII
int setModeWii() {
	PORTD |= 0b00001001; // enable pull-up for S1 and S2
	DDRD  &= 0b11110110; // make S1 and S2 input

	PORTC &= 0b11001111; // disable pull-up for SDA and SCL
	DDRC  |= 0b00110000; // make SDA and SCL output

	return WORKING_MODE_WII;
}
#endif

#if USE_PT
int setModePT() {	
	if(CFG_HOME_EMU)
		SET_HOME_OUTPUT

	if(CFG_JOYSTICK_SWITCH_EMU || CFG_INVERTED_TRIGGERS) {
		DDRD |= (1<<4); // pin S3 is output
		DDRC |= (1<<6); // pin S4 is output
	}

	PORTD |= (1<<3); // enable pass-through usb lines

	return WORKING_MODE_PT;
}
#endif

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
		ENABLE_CFG_DIGITAL_PAD(config)
		DISABLE_CFG_LEFT_STICK(config)
		DISABLE_CFG_RIGHT_STICK(config)
	}
	else if(!Stick_Left) {
		DISABLE_CFG_DIGITAL_PAD(config)
		ENABLE_CFG_LEFT_STICK(config)
		DISABLE_CFG_RIGHT_STICK(config)
	}
	else if(!Stick_Right) {
		DISABLE_CFG_DIGITAL_PAD(config)
		DISABLE_CFG_LEFT_STICK(config)
		ENABLE_CFG_RIGHT_STICK(config)
	}

	if(CFG_JOYSTICK_SWITCH_READ) {	
		PORTD |= (1<<4); // pin S3 is high
		PORTC |= (1<<6); // pin S4 is high
	}

#if USE_WII
	return setModeWii();
#else
	if(!Stick_Short)
		return setModePS3();
	else if(!Stick_Forward)
		return setModeXBox();
	else if(!Stick_Jab)
		return setModePT();
	else {
		if(CFG_DEF_WORK_MODE_PS3)
			return setModePS3();
		else if(CFG_DEF_WORK_MODE_XBOX)
			return setModeXBox();
		else
			return setModePT();
	}
#endif
}

/* ------------------------------------------------------------------------- */

void readJoystickSwitch() {
    if(CFG_JOYSTICK_SWITCH_READ) {
        if(!(PIND & (1<<4)) && (PINC & (1<<6))) { // S3 low and S4 high
			DISABLE_CFG_DIGITAL_PAD(config)
			ENABLE_CFG_LEFT_STICK(config)
			DISABLE_CFG_RIGHT_STICK(config)
        }
        else if((PIND & (1<<4)) && !(PINC & (1<<6))) { // S3 high and S4 low
			DISABLE_CFG_DIGITAL_PAD(config)
			DISABLE_CFG_LEFT_STICK(config)
			ENABLE_CFG_RIGHT_STICK(config)
        }
        else if((PIND & (1<<4)) && (PINC & (1<<6))) { // S3 high and S4 high
			ENABLE_CFG_DIGITAL_PAD(config)
			DISABLE_CFG_LEFT_STICK(config)
			DISABLE_CFG_RIGHT_STICK(config)
        }
        else if(!(PIND & (1<<4)) && !(PINC & (1<<6))) { // S3 low and S4 low, SHOULD NEVER HAPPEN
			DISABLE_CFG_DIGITAL_PAD(config)
			DISABLE_CFG_LEFT_STICK(config)
			DISABLE_CFG_RIGHT_STICK(config)
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
uchar* data[130];

int main(void)
{
#if USE_WII
	wiimote_extension_controller();
#else
	switch(hardwareInit()) {
	case WORKING_MODE_PS3:
	  ps3_controller();
	  break;

	case WORKING_MODE_XBOX:
	  //xbox_controller();
	  break;

	case WORKING_MODE_PT:
	  pass_through();
	  break;
	}
#endif

    return 0;
}
