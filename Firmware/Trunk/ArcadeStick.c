#include <avr/io.h>
#include <avr/interrupt.h>  /* for sei() */
#include <util/delay.h>     /* for _delay_ms() */

#include <avr/pgmspace.h>   /* required by usbdrv.h */
#include "usbconfig.h"
#include "usbdrv.h"
#include <avr/eeprom.h> /* EEPROM functions */

#define EEPROM_DEF 0xFF /* for uninitialized EEPROMs */

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
uint8_t config = EEPROM_DEF;
uint8_t config_EEPROM EEMEM = CONFIG_DEF;

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
//PC Button 9 - Select
#define Stick_Select		(PINC & (1<<1))
//PC Button 10 - Start
#define Stick_Start			(PINC & (1<<0))
//PC Button 13 - Home
#define Stick_Home			(PINC & (1<<5))

#define EXTRA_BUTTONS 1
#ifdef EXTRA_BUTTONS
	//PC Button 5 - L1 - 4P
	#define Stick_Extra0		(PINC & (1<<3))
	//PC Button 7 - L2 - 4K
	#define Stick_Extra1		(PINC & (1<<4))
#endif

unsigned char SwitchMode;

/* ------------------------------------------------------------------------- */
/* ----------------------------- USB interface ----------------------------- */
/* ------------------------------------------------------------------------- */

/* ------------------------------------------------------------------------- */


typedef struct {
	uchar	buttons1;
	uchar	buttons2;	
	uchar   hatswitch;
	uchar	x;
	uchar	y;
	uchar	z;
	uchar	rz;
	uchar   extra; // only used for HID report
} report_t;

static	report_t reportBuffer;

usbMsgLen_t usbFunctionSetup(uchar data[8])
{
	usbRequest_t    *rq = (void *)data;

    if((rq->bmRequestType & USBRQ_TYPE_MASK) == USBRQ_TYPE_CLASS) {    /* class request */
		/* wValue: ReportType (highbyte), ReportID (lowbyte) */
        if(rq->bRequest == USBRQ_HID_GET_REPORT) {
			 // set buffer data
			reportBuffer.buttons1 = 33;
			reportBuffer.buttons2 = 38;
			reportBuffer.hatswitch =
			reportBuffer.x =
			reportBuffer.y =
			reportBuffer.z = 
			reportBuffer.rz =
			reportBuffer.extra = 0;
			usbMsgPtr = (void *)&reportBuffer;

			return sizeof(reportBuffer);
        }
    }

    return 0;   /* default for not implemented requests: return no data back to host */
}

void resetReportBuffer() {
	reportBuffer.buttons1 =
	reportBuffer.buttons2 =
	reportBuffer.extra = 0;
	reportBuffer.hatswitch = 0x08;
	reportBuffer.x =
	reportBuffer.y =
	reportBuffer.z =
	reportBuffer.rz = 0x80;
}

PROGMEM char usbHidReportDescriptor[] = { // PC HID Report Descriptor
    0x05, 0x01,                    // USAGE_PAGE (Generic Desktop)
    0x09, 0x05,                    // USAGE (Game Pad)
    0xa1, 0x01,                    // COLLECTION (Application)
    0x15, 0x00,                    //   LOGICAL_MINIMUM (0)
    0x25, 0x01,                    //   LOGICAL_MAXIMUM (1)
    0x35, 0x00,                    //   PHYSICAL_MINIMUM (0)
    0x45, 0x01,                    //   PHYSICAL_MAXIMUM (1)
    0x75, 0x01,                    //   REPORT_SIZE (1)
    0x95, 0x0d,                    //   REPORT_COUNT (13)
    0x05, 0x09,                    //   USAGE_PAGE (Button)
    0x19, 0x01,                    //   USAGE_MINIMUM (Button 1)
    0x29, 0x0d,                    //   USAGE_MAXIMUM (Button 13)
    0x81, 0x02,                    //   INPUT (Data,Var,Abs)
/* report bits: 13x1=13 */
    0x95, 0x03,                    //   REPORT_COUNT (3)
    0x81, 0x01,                    //   INPUT (Cnst,Ary,Abs)
/* report bits: + 3x1=3 */
    0x05, 0x01,                    //   USAGE_PAGE (Generic Desktop)
    0x25, 0x07,                    //   LOGICAL_MAXIMUM (7)
    0x46, 0x3b, 0x01,              //   PHYSICAL_MAXIMUM (315)
    0x75, 0x04,                    //   REPORT_SIZE (4)
    0x95, 0x01,                    //   REPORT_COUNT (1)
    0x65, 0x14,                    //   UNIT (Eng Rot:Angular Pos)
    0x09, 0x39,                    //   USAGE (Hat switch)
    0x81, 0x42,                    //   INPUT (Data,Var,Abs,Null)
/* report bits: + 1x4=4 */
    0x65, 0x00,                    //   UNIT (None)
    0x95, 0x01,                    //   REPORT_COUNT (1)
    0x81, 0x01,                    //   INPUT (Cnst,Ary,Abs)
/* report bits: + 1x4=4 */
    0x26, 0xff, 0x00,              //   LOGICAL_MAXIMUM (255)
    0x46, 0xff, 0x00,              //   PHYSICAL_MAXIMUM (255)
    0x09, 0x30,                    //   USAGE (X)
    0x09, 0x31,                    //   USAGE (Y)
    0x09, 0x32,                    //   USAGE (Z)
    0x09, 0x35,                    //   USAGE (Rz)
    0x75, 0x08,                    //   REPORT_SIZE (8)
    0x95, 0x04,                    //   REPORT_COUNT (4)
    0x81, 0x02,                    //   INPUT (Data,Var,Abs)
/* report bits: + 4x8=32 */
    0x06, 0x00, 0xff,              //   USAGE_PAGE (Vendor Defined Page 1)
    0x0a, 0x21, 0x26,              //   UNKNOWN
    0x95, 0x08,                    //   REPORT_COUNT (8)
    0xb1, 0x02,                    //   FEATURE (Data,Var,Abs)
    0xc0                           // END_COLLECTION
};

/* ------------------------------------------------------------------------- */

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
				if(!Stick_Left)
					// default working mode: Dual Strike
					newConfig &= ~(1<<0);
				
				if(!Stick_Right)
					// default working mode: pass-through
					newConfig |= (1<<0);
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

void setModeDS() {
	if(CFG_JOYSTICK_SWITCH_READ) {		
		PORTD |= (1<<4); // pin S3 is high
		PORTC |= (1<<6); // pin S4 is high
	}

	SwitchMode = 0;
}

void setModePT() {	
	if(CFG_HOME_EMU)
		DDRC |= (1<<5);	// make the home an output

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

	SwitchMode = 1;
}

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
		if(CFG_DEF_WORK_MODE_DS)
			setModePT();
		else
			setModeDS();
	}
	else {
		// else set to default mode
		if(CFG_DEF_WORK_MODE_DS)
			setModeDS();
		else
			setModePT();
	}
}

/* ------------------------------------------------------------------------- */

void ReadJoystick()
{
	resetReportBuffer();

	// Left Joystick Directions
	if(CFG_LEFT_STICK) {
		if (!Stick_Up) reportBuffer.y = 0x00;
		else if (!Stick_Down) reportBuffer.y = 0xFF;

		if (!Stick_Left) reportBuffer.x = 0x00;
		else if (!Stick_Right) reportBuffer.x = 0xFF;
	}

	// Right Joystick Directions
	if(CFG_RIGHT_STICK) {
		if (!Stick_Up) reportBuffer.rz = 0;
		else if (!Stick_Down) reportBuffer.rz = 0xFF;
		
		if (!Stick_Left) reportBuffer.z = 0;
		else if (!Stick_Right) reportBuffer.z = 0xFF;

	}

	// Digital Pad Directions
	if(CFG_DIGITAL_PAD) {
		if(!Stick_Up)
		{
			if(!Stick_Right) reportBuffer.hatswitch=0x01;
			else if(!Stick_Left) reportBuffer.hatswitch=0x07;
			else reportBuffer.hatswitch=0x00;
		}
		else if(!Stick_Down)
		{
			if(!Stick_Right) reportBuffer.hatswitch=0x03;
			else if(!Stick_Left) reportBuffer.hatswitch=0x05;
			else reportBuffer.hatswitch=0x04;
		}
		else
		{
			if(!Stick_Right) reportBuffer.hatswitch=0x02;
			if(!Stick_Left) reportBuffer.hatswitch=0x06;
		}
	}

	// Buttons
	if(!Stick_Jab)
		reportBuffer.buttons1 |= 0b00000001;

	if(!Stick_Short)
		reportBuffer.buttons1 |= 0b00000010;

	if(!Stick_Forward)
		reportBuffer.buttons1 |= 0b00000100;

	if(!Stick_Strong)
		reportBuffer.buttons1 |= 0b00001000;

#ifdef EXTRA_BUTTONS					
	if(!Stick_Extra0)
		reportBuffer.buttons1 |=0b00010000;

	if(!Stick_Extra1)
		reportBuffer.buttons1 |=0b01000000;
#endif

	if(!Stick_Fierce)
		reportBuffer.buttons1 |= 0b00100000;

	if(!Stick_Roundhouse)
		reportBuffer.buttons1 |= 0b10000000;

	if(CFG_HOME_EMU && !Stick_Start && !Stick_Select /* && Stick_Jab */)
		reportBuffer.buttons2 |= 0b00010000;
	else {
		if(!Stick_Start)
			reportBuffer.buttons2 |=0b00000010;

		if(!Stick_Select)
			reportBuffer.buttons2 |=0b00000001;
	}

	if(!Stick_Home)
		reportBuffer.buttons2 |= 0b00010000;
}

/* ------------------------------------------------------------------------- */

int main(void)
{
	HardwareInit();

	if(SwitchMode == 0) { // if switched to Dual Strike
	    usbDeviceDisconnect(); /* enforce re-enumeration, do this while interrupts are disabled! */
	    _delay_ms(300UL);/* fake USB disconnect for > 250 ms */
	    usbDeviceConnect();
		usbInit();
	    sei();

	    while(1) { /* main event loop */
	        usbPoll();

	        if(usbInterruptIsReady()) {
	            /* called after every poll of the interrupt endpoint */				
				if(CFG_JOYSTICK_SWITCH_READ) {
					if(!(PIND & (1<<4)) && (PINC & (1<<6))) { // S3 low and S4 high
						// Dual Strike digital pad: disabled
						config &= ~(1<<2);
						// Dual Strike left stick: enabled
						config |= (1<<1);
						// Dual Strike right stick: disabled
						config &= ~(1<<3);
					}
					else if((PIND & (1<<4)) && !(PINC & (1<<6))) { // S3 high and S4 low
						// Dual Strike digital pad: disabled
						config &= ~(1<<2);
						// Dual Strike left stick: disabled
						config &= ~(1<<1);
						// Dual Strike right stick: enabled
						config |= (1<<3);
					}
					else if((PIND & (1<<4)) && (PINC & (1<<6))) { // S3 high and S4 high
						// Dual Strike digital pad: enabled
						config |= (1<<2);
						// Dual Strike left stick: disabled
						config &= ~(1<<1);
						// Dual Strike right stick: disabled
						config &= ~(1<<3);
					}
					else if(!(PIND & (1<<4)) && !(PINC & (1<<6))) { // S3 low and S4 low
						// Dual Strike digital pad: disabled
						config &= ~(1<<2);
						// Dual Strike left stick: disabled
						config &= ~(1<<1);
						// Dual Strike right stick: disabled
						config &= ~(1<<3);
					}
				}

				ReadJoystick();
	            usbSetInterrupt((void *)&reportBuffer, 7*sizeof(uchar));
	        }
	    }
	}
	else if(SwitchMode == 1) { // if switched to pass-through
		if(CFG_JOYSTICK_SWITCH_EMU) {	
			if(CFG_LEFT_STICK) {
				PORTD &= ~(1<<4); // pin S3 is low	
				PORTC |= (1<<6); // pin S4 is high
			}
			else if(CFG_RIGHT_STICK) {
				PORTD |= (1<<4); // pin S3 is high	
				PORTC &= ~(1<<6); // pin S4 is low
			}
			else if(CFG_DIGITAL_PAD) {
				PORTD |= (1<<4); // pin S3 is high	
				PORTC |= (1<<6); // pin S4 is high
			}
		}

	    while(1) { /* main event loop */
			if(CFG_HOME_EMU) {
				if((!Stick_Start) &&  (!Stick_Select))
					//if start and select are pressed then make guide line low
					PORTC &= ~(1<<5);
				else
					//else guide line is high
					PORTC |= (1<<5);
			}

			if(CFG_INVERTED_TRIGGERS) {
				// if LT is pressed then invert trigger and make it high (pressed)
				if(!Stick_Extra1)
					PORTD |= (1<<4);
				else
					PORTD &= ~(1<<4);
		
				// if RT is pressed then invert trigger and make it high (pressed)
				if(!Stick_Roundhouse)
					PORTC |= (1<<6);
				// else keep trigger low
				else
					PORTC &= ~(1<<6);
			}
	    }
	}

    return 0;
}
