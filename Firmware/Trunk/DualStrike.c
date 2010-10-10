#include "DualStrike.h"

#include <avr/eeprom.h> /* EEPROM functions */
#include <avr/pgmspace.h>   /* required by usbdrv.h */

// README
/* 
Configuration Mode
==================
In the configuration mode the behaviour of the Dual Strike can be changed. 
While plugging in the USB Cable just press Select. Leave it by pressing Start. 
When first entering configuration mode, it is in USB configuration mode, which 
means it is ready to be updated from a PC.

If you want to change the configuration without a PC first press one of the 
action buttons (any other than Start, Select and Home). Then pressing entering 
the following button-joystick combinations changes the configuration:

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
Up    = Dual Strike MAME
Right = Dual Strike XBox1
Down  = pass-through

revert to defaults:
-------------------
Button: MK

Start+Select=Home:
------------------
Button: LP
Left  = disabled [default]
Right = enabled
		
*/

#define WORKING_MODE_PT		0
#define WORKING_MODE_PS3	1
#define WORKING_MODE_PC		2
#define WORKING_MODE_AUTO	3

#if ATMEGA_NO == 168
#define WORKING_MODE_MAME	4
#define WORKING_MODE_XBOX	5
#endif

#define enableUsbLines()	S1_PORT |=  (1<<S1_PIN);
#define disableUsbLines()	S1_PORT &= ~(1<<S1_PIN);


// declares config and config_EEPROM
CFG_DECLARATION

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
		 	&& Stick_LP
			&& Stick_MP
			&& Stick_HP
			&& Stick_LK
			&& Stick_MK
			&& Stick_HK
#ifdef EXTRA_BUTTONS
			&& Stick_4P
			&& Stick_4K
#endif
)
			programmer_poll();

		disableUsbLines();
		readConfig(newConfig);

		while(Stick_Start) {
			if(!Stick_MK) {
				// revert to defaults
				CONFIG_SET_DEFAULTS(newConfig)
			}

			if(Stick_LK && Stick_MK && Stick_LP && Stick_MP && Stick_HK) {
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
		}

	}
	
	writeConfig(newConfig);
}

int setModePS3() {
	enableUsbLines();

	return WORKING_MODE_PS3;
}

int setModePC() {
	S3_PORT |= (1<<S3_PIN); // pin S3 is high
	S4_PORT |= (1<<S4_PIN); // pin S4 is high
	enableUsbLines();

	return WORKING_MODE_PC;
}

#if ATMEGA_NO == 168
int setModeMAME() {	
	enableUsbLines();

	return WORKING_MODE_MAME;
}

int setModeXBox() {	
	enableUsbLines();

	return WORKING_MODE_XBOX;
}
#endif

int setModePT() {
	if(CFG_JOYSTICK_SWITCH_EMU || CFG_INVERTED_TRIGGERS) {
        S3_PORT &= ~(1<<S3_PIN); // pin S3 is low	
        S4_PORT &= ~(1<<S4_PIN); // pin S4 is low
	}

	disconnectUSB();
	disableUsbLines();

	S2_PORT |= (1<<S2_PIN); // enable pass-through usb lines

	return WORKING_MODE_PT;
}

int setModeAutodetect() {
	return WORKING_MODE_AUTO;
}


int setModeDefault() {
	return setModeAutodetect();
	/*
	if(CFG_DEF_WORK_MODE_PS3)
		return setModePS3();
	else if(CFG_DEF_WORK_MODE_PC)
		return setModePC();
#if ATMEGA_NO == 168
	else if(CFG_DEF_WORK_MODE_MAME)
		return setModeMAME();
	else if(CFG_DEF_WORK_MODE_XBOX)
		return setModeXBox();
#endif
	else
		return setModePT();
		*/
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

If only one working mode is enabled, then this is activated.
If two working modes are enabled and one of the buttons LK, MK, LP, MP, HP and 4P is
pressed, then the non-default mode is activated.
If more than two working modes are enabled,:
-the button LK is pressed and the PS3 mode is enabled, then the PS3 mode is activated.
-the button MK is pressed and the MAME mode is enabled, then the MAME mode is activated.
-the button LP is pressed and the XBox mode is enabled, then the XBox mode is activated.
-the button MP is pressed and the pass-through mode is enabled, then the pass-through mode is activated.
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

	if(CFG_JOYSTICK_SWITCH_READ || CFG_X3_READ) {	
		S3_PORT |= (1<<S3_PIN); // pin S3 is high
		S4_PORT |= (1<<S4_PIN); // pin S4 is high
	}

	int enabledWorkingModes = 0;
	
	if(CFG_WORK_MODE_PS3_ENABLED)
		enabledWorkingModes++;

	if(CFG_WORK_MODE_PC_ENABLED)
		enabledWorkingModes++;

#if ATMEGA_NO == 168
	if(CFG_WORK_MODE_MAME_ENABLED)
		enabledWorkingModes++;
	
	if(CFG_WORK_MODE_XBOX_ENABLED)
		enabledWorkingModes++;
#endif

	if(CFG_WORK_MODE_PT_ENABLED)
		enabledWorkingModes++;

	if(enabledWorkingModes == 2) {
		if(!Stick_LK
		|| !Stick_MK
		|| !Stick_LP
		|| !Stick_MP
		|| !Stick_HP
		|| !Stick_4P) {
			if(!CFG_DEF_WORK_MODE_PS3 && CFG_WORK_MODE_PS3_ENABLED)
				return setModePS3();

			if(!CFG_DEF_WORK_MODE_PC && CFG_WORK_MODE_PC_ENABLED)
				return setModePC();

#if ATMEGA_NO == 168
			if(!CFG_DEF_WORK_MODE_MAME && CFG_WORK_MODE_MAME_ENABLED)
				return setModeMAME();
	
			if(!CFG_DEF_WORK_MODE_XBOX && CFG_WORK_MODE_XBOX_ENABLED)
				return setModeXBox();
#endif	

			if(!CFG_DEF_WORK_MODE_PT && CFG_WORK_MODE_PT_ENABLED)
				return setModePT();
		}
	}
	else if(enabledWorkingModes > 2) {
		if(!Stick_LK && CFG_WORK_MODE_PS3_ENABLED)
			return setModePS3();

#if ATMEGA_NO == 168
		if(!Stick_MK && CFG_WORK_MODE_MAME_ENABLED)
			return setModeMAME();

		if(!Stick_LP && CFG_WORK_MODE_XBOX_ENABLED)
			return setModeXBox();
#endif

		if(!Stick_MP && CFG_WORK_MODE_PT_ENABLED)
			return setModePT();

		if(!Stick_HK && CFG_WORK_MODE_PC_ENABLED)
			return setModePC();		
	}

	return setModeDefault();
}

/* ------------------------------------------------------------------------- */
/* Here the meta-button functionality for Start is defined. */

uchar startPressed = 0;
uchar startWasUsed = 0;
uint startSendCount = 0;
uint startSendRepeats = 0;

void updateStartState() {
	uchar lastStartPressed = startPressed;
	startPressed = !Stick_Start;
	uchar startReleased = lastStartPressed && !startPressed;

	if(startWasUsed) {
		startSendCount = 0;

		if(startReleased)
			startWasUsed = 0;
	}
	else if(startReleased)
		startSendCount = startSendRepeats;
    else if(startSendCount > 0)
		 startSendCount--;
}

/* ------------------------------------------------------------------------- */

void updateJoystickMode() {
    if(CFG_JOYSTICK_SWITCH_READ) {
        if(!Stick_S3 && Stick_S4) { 	 // S3 low and S4 high
			CFG_SET_LEFT_STICK(config)
        }
        else if(Stick_S3 && !Stick_S4) { // S3 high and S4 low
			CFG_SET_RIGHT_STICK(config)
        }
        else if(Stick_S3 && Stick_S4) {  // S3 high and S4 high
			CFG_SET_DIGITAL_PAD(config)
        }
    }
	else {
		if(startPressed) {
			if(!Stick_Up) {
				CFG_SET_DIGITAL_PAD(config)
				startWasUsed = 1;
			}
			else if(!Stick_Left) {
				CFG_SET_LEFT_STICK(config)
				startWasUsed = 1;
			}
			else if(!Stick_Right) {
				CFG_SET_RIGHT_STICK(config)
				startWasUsed = 1;
			}
		}
	}
}

/* ------------------------------------------------------------------------- */

uchar autodetectCount = 0;
uchar detected = 0;

void initAutodetectTimer() {
	TCCR0B = (1<<CS00) | (1<<CS02); // clock prescaling: 1024 cycles
}

void resetAutodetectTimer() {
	autodetectCount = 0;
	TCNT0 = 0;
	TIFR0 |= (1<<TOV0); // reset overflow flag
}

void resetAutodetect() {
	disableUsbLines();
	detected = 0;
}

#define IDLE_RATE_UNIT_COUNT_CYCLES ((uchar)(F_CPU / 250 / 1024))
#define IDLE_RATE_OVERLOW_COUNT		(255 / IDLE_RATE_UNIT_COUNT_CYCLES)
#define IDLE_RATE_OVERLOW_CYCLES	(255 % IDLE_RATE_UNIT_COUNT_CYCLES)

uchar autodetectTimePassed() {
	if(TIFR0 & (1<<TOV0)) { // overflow
		if(autodetectCount < 255)
			autodetectCount++;

		TIFR0 |= (1<<TOV0); // reset overflow flag
	}

	return (autodetectCount >= 50);
}

void autodetect() {
	initAutodetectTimer();

	if(CFG_WORK_MODE_PS3_ENABLED) {
		resetAutodetect();
		setModePS3();
		ps3_init_controller();

		while(detected != 1)
			usbPoll();

		resetAutodetectTimer();

		while(!autodetectTimePassed())
			usbPoll();

		if(detected == 2)
			ps3_controller();
	}
	
	if(CFG_WORK_MODE_PC_ENABLED) {
		resetAutodetect();
		setModePC();
		pc_init_controller();

		while(detected != 1)
			usbPoll();

		resetAutodetectTimer();

		while(!autodetectTimePassed())
			usbPoll();

		if(detected == 2)
			pc_controller();
	}

	if(CFG_WORK_MODE_XBOX_ENABLED) {
		resetAutodetect();
		setModeXBox();
		xbox_init_controller();

		while(detected != 1)
			usbPoll();

		resetAutodetectTimer();

		while(!autodetectTimePassed())
			usbPoll();

		if(detected == 2)
			xbox_controller();
	}

	if(CFG_WORK_MODE_PT_ENABLED) {
		resetAutodetect();
		setModePT();
		pass_through();
	}
}

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
	case WORKING_MODE_AUTO:
	  autodetect();
	  break;

	case WORKING_MODE_PT:
	  pass_through();
	  break;

	case WORKING_MODE_PS3:
	  ps3_init_controller();
	  ps3_controller();
	  break;

	case WORKING_MODE_PC:
	  pc_init_controller();
	  pc_controller();
	  break;

#if ATMEGA_NO == 168
	case WORKING_MODE_MAME:
	  mame_controller();
	  break;

	case WORKING_MODE_XBOX:
	  xbox_init_controller();
	  xbox_controller();
	  break;
#endif
	}

    return 0;
}
