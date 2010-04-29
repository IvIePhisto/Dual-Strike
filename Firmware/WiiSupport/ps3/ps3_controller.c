#include "ps3_controller.h"

#include "../DualStrike.h"

#include <avr/io.h>
#include <avr/interrupt.h>  /* for sei() */
#include <util/delay.h>     /* for _delay_ms() */

#include <avr/pgmspace.h>   /* required by usbdrv.h */
#include "usbdrv/usbdrv.h"

extern uint8_t config[2];

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

extern report_t data;

usbMsgLen_t usbFunctionSetup(uchar receivedData[8])
{
	usbRequest_t    *rq = (void *)receivedData;

    if((rq->bmRequestType & USBRQ_TYPE_MASK) == USBRQ_TYPE_CLASS) {    /* class request */
		/* wValue: ReportType (highbyte), ReportID (lowbyte) */
        if(rq->bRequest == USBRQ_HID_GET_REPORT) {
			 // set buffer data
			data.buttons1 = 33;
			data.buttons2 = 38;
			data.hatswitch =
			data.x =
			data.y =
			data.z = 
			data.rz =
			data.extra = 0;
			usbMsgPtr = (void *)&data;

			return 8;
        }
    }

    return 0;   /* default for not implemented requests: return no data back to host */
}

void resetReportBuffer() {
	data.buttons1 =
	data.buttons2 =
	data.extra = 0;
	data.hatswitch = 0x08;
	data.x =
	data.y =
	data.z =
	data.rz = 0x80;
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

void readInputPS3()
{
	resetReportBuffer();

	// Left Joystick Directions
	if(CFG_LEFT_STICK) {
		if (!Stick_Up) data.y = 0x00;
		else if (!Stick_Down) data.y = 0xFF;

		if (!Stick_Left) data.x = 0x00;
		else if (!Stick_Right) data.x = 0xFF;
	}

	// Right Joystick Directions
	if(CFG_RIGHT_STICK) {
		if (!Stick_Up) data.rz = 0;
		else if (!Stick_Down) data.rz = 0xFF;
		
		if (!Stick_Left) data.z = 0;
		else if (!Stick_Right) data.z = 0xFF;

	}

	// Digital Pad Directions
	if(CFG_DIGITAL_PAD) {
		if(!Stick_Up)
		{
			if(!Stick_Right) data.hatswitch=0x01;
			else if(!Stick_Left) data.hatswitch=0x07;
			else data.hatswitch=0x00;
		}
		else if(!Stick_Down)
		{
			if(!Stick_Right) data.hatswitch=0x03;
			else if(!Stick_Left) data.hatswitch=0x05;
			else data.hatswitch=0x04;
		}
		else
		{
			if(!Stick_Right) data.hatswitch=0x02;
			if(!Stick_Left) data.hatswitch=0x06;
		}
	}

	// Buttons
	if(!Stick_Jab)
		data.buttons1 |= 0b00000001;

	if(!Stick_Short)
		data.buttons1 |= 0b00000010;

	if(!Stick_Forward)
		data.buttons1 |= 0b00000100;

	if(!Stick_Strong)
		data.buttons1 |= 0b00001000;

#ifdef EXTRA_BUTTONS					
	if(!Stick_Extra0)
		data.buttons1 |=0b00010000;

	if(!Stick_Extra1)
		data.buttons1 |=0b01000000;
#endif

	if(!Stick_Fierce)
		data.buttons1 |= 0b00100000;

	if(!Stick_Roundhouse)
		data.buttons1 |= 0b10000000;

	if(CFG_HOME_EMU && !Stick_Start && !Stick_Select /* && Stick_Jab */)
		data.buttons2 |= 0b00010000;
	else {
		if(!Stick_Start)
			data.buttons2 |=0b00000010;

		if(!Stick_Select)
			data.buttons2 |=0b00000001;
	}

	if(!Stick_Home)
		data.buttons2 |= 0b00010000;
}

/* ------------------------------------------------------------------------- */

void ps3_controller() {
    usbDeviceDisconnect(); /* enforce re-enumeration, do this while interrupts are disabled! */
    _delay_ms(300UL);/* fake USB disconnect for > 250 ms */
    usbDeviceConnect();
    usbInit();
    sei();

    while(1) { /* main event loop */
        usbPoll();

        if(usbInterruptIsReady()) {
            /* called after every poll of the interrupt endpoint */				
			readJoystickSwitch();
            readInputPS3();
            usbSetInterrupt((void *)&data, 7*sizeof(uchar));
        }
    }
}
