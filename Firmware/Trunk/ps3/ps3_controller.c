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
	uchar   l2;
	uchar   r2;
} report_t;

extern report_t data;

#define PS3_SQUARE		data.buttons1 |= (1<<0);
#define PS3_TRIANGLE	data.buttons1 |= (1<<1);
#define PS3_CROSS		data.buttons1 |= (1<<2);
#define PS3_CIRCLE		data.buttons1 |= (1<<3);
#define PS3_L1			data.buttons1 |= (1<<4);
#define PS3_R1			data.buttons1 |= (1<<5);
#define PS3_L2			data.buttons1 |= (1<<6);
#define PS3_R2			data.buttons1 |= (1<<7);
#define PS3_SELECT		data.buttons2 |= (1<<0);
#define PS3_START		data.buttons2 |= (1<<1);
#define PS3_L3			data.buttons2 |= (1<<2);
#define PS3_R3			data.buttons2 |= (1<<3);
#define PS3_HOME		data.buttons2 |= (1<<4);

#define HID_REPORT_TYPE_INPUT 1
#define HID_REPORT_TYPE_OUTPUT 2
#define HID_REPORT_TYPE_FEATURE 3

usbMsgLen_t usbFunctionSetup(uchar receivedData[8])
{
	usbRequest_t    *rq = (void *)receivedData;

    if((rq->bmRequestType & USBRQ_TYPE_MASK) == USBRQ_TYPE_CLASS) {    /* class request */
		// wValue: ReportType (highbyte), ReportID (lowbyte)
        if(rq->bRequest == USBRQ_HID_GET_REPORT) {
			if(rq->wValue.bytes[1] == HID_REPORT_TYPE_FEATURE) {
				if(rq->wValue.bytes[0] == 0) {
					 // set buffer data
					((uchar*)&data)[0] = 0x21; // 0b00100001
					((uchar*)&data)[1] = 0x26; // 0b00100110

					for(int i = 2; i < 8; i++)
						((uchar*)&data)[i] = 0;

					usbMsgPtr = (uchar*)&data;

					return 8;
				}
			}
        }
    }

    return 0;   /* default for not implemented requests: return no data back to host */
}

void resetReportBuffer() {
	data.buttons1 =
	data.buttons2 = 0;
	data.hatswitch = 0b00001000; /* only first 4 bits*/
	data.x =
	data.y =
	data.z =
	data.rz = 0b10000000;
	data.l2 =
	data.r2 = 0;
}

PROGMEM char usbHidReportDescriptor[] = { // PC HID Report Descriptor
    0x05, 0x01,                    // USAGE_PAGE (Generic Desktop)
    0x09, 0x04,                    // USAGE (Gamepad)
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
    0x95, 0x04,                    //   REPORT_COUNT (4)
    0x75, 0x08,                    //   REPORT_SIZE (8)
    0x81, 0x02,                    //   INPUT (Data,Var,Abs)
/* report bits: + 4x8=32 */

	/*
    0x25, 0x0f,		               //   LOGICAL_MAXIMUM (15)
    0x45, 0x0f,                    //   PHYSICAL_MAXIMUM (15)

    0x09, 0x30,                    //   USAGE (X)
    0x09, 0x31,                    //   USAGE (Y)
    0x09, 0x32,                    //   USAGE (Z)
    0x09, 0x33,                    //   USAGE (Rx)
    0x09, 0x34,                    //   USAGE (Ry)
    0x09, 0x35,                    //   USAGE (Rz)
    0x09, 0x36,                    //   USAGE (Slider)
    0x09, 0x37,                    //   USAGE (Dial)
    0x09, 0x37,                    //   USAGE (Wheel)
    0x09, 0x40,                    //   USAGE (Vx)
    0x09, 0x41,                    //   USAGE (Vy)
    0x09, 0x42,                    //   USAGE (Vz)
    0x09, 0x43,                    //   USAGE (Vbrx)
    0x09, 0x44,                    //   USAGE (Vbry)
    0x09, 0x45,                    //   USAGE (Vbrz)

    0x09, 0x33,                    //   USAGE (Rx)
    0x09, 0x34,                    //   USAGE (Ry)
    0x95, 0x02,                    //   REPORT_COUNT (2)
    0x81, 0x02,                    //   INPUT (Data,Var,Abs)
	*/

    //0x25, 0x0f,		             //   LOGICAL_MAXIMUM (15)
    //0x45, 0x0f,                    //   PHYSICAL_MAXIMUM (15)
    0x09, 0x01,                    //   USAGE (Pointer)
	0x95, 0x02,                    //   REPORT_COUNT (2)
    0x81, 0x02,                    //   INPUT (Data,Var,Abs)
/* report bits: + 39x8=312 */

    0x06, 0x00, 0xff,              //   USAGE_PAGE (Vendor Defined Page 1)
    0x0a, 0x21, 0x26,              //   USAGE (Undefined, value 0x2621)
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
		if (!Stick_Up)
			data.y = 0x00;
		else if (!Stick_Down)
			data.y = 0xFF;

		if (!Stick_Left)
			data.x = 0x00;
		else if (!Stick_Right)
			data.x = 0xFF;
	}

	// Right Joystick Directions
	if(CFG_RIGHT_STICK) {
		if (!Stick_Up)
			data.rz = 0;
		else if (!Stick_Down)
			data.rz = 0xFF;
		
		if (!Stick_Left)
			data.z = 0;
		else if (!Stick_Right)
			data.z = 0xFF;
	}

	// Digital Pad Directions
	if(CFG_DIGITAL_PAD) {
		if(!Stick_Up) {
			if(!Stick_Right)
				data.hatswitch = 0b00000001;
			else if(!Stick_Left)
				data.hatswitch = 0b00000111;
			else
				data.hatswitch = 0b00000000;
		}
		else if(!Stick_Down) {
			if(!Stick_Right)
				data.hatswitch = 0b00000011;
			else if(!Stick_Left)
				data.hatswitch = 0b00000101;
			else
				data.hatswitch = 0b00000100;
		}
		else {
			if(!Stick_Right)
				data.hatswitch = 0b00000010;
			if(!Stick_Left)
				data.hatswitch = 0b00000110;
		}
	}

	// Buttons
	if(!Stick_Jab)
		PS3_SQUARE

	if(!Stick_Short)
		PS3_TRIANGLE

	if(!Stick_Forward)
		PS3_CROSS

	if(!Stick_Strong)
		PS3_CIRCLE

	if(!Stick_Fierce)
		PS3_R1

	if(!Stick_Roundhouse) {
		PS3_R2
		//data.r2_l2 |= 0xF0;
		data.r2 = 0xFF;
	}

#ifdef EXTRA_BUTTONS					
	if(!Stick_Extra0)
		PS3_L1

	if(!Stick_Extra1) {
		PS3_L2
		//data.r2_l2 |= 0x0F;
		data.l2 = 0xFF;
	}
#endif

	if(CFG_HOME_EMU && !Stick_Start && !Stick_Select /* && Stick_Jab */)
		PS3_HOME
	else {
		if(!Stick_Start)
			PS3_START

		if(!Stick_Select)
			PS3_SELECT
	}

	if(!Stick_Home)
		PS3_HOME
}

/* ------------------------------------------------------------------------- */

void sendDataPS3(uchar* data, unsigned int byteCount) {
	int currentByte;
	int currentCount;

	currentByte = 0;

	while(currentByte < byteCount) {
		currentCount = byteCount - currentByte;

		if(currentCount > 8)
			currentCount = 8;

		usbSetInterrupt(data + currentByte, currentCount*sizeof(uchar));
		currentByte += currentCount;

		while(!usbInterruptIsReady()) usbPoll();
	}
}

void ps3_controller() {
    usbDeviceDisconnect(); /* enforce re-enumeration, do this while interrupts are disabled! */
    _delay_ms(300UL);/* fake USB disconnect for > 250 ms */
    usbDeviceConnect();
    usbInit();
    sei();
	usbPoll();

    while(1) { /* main event loop */
        if(usbInterruptIsReady()) {
			readJoystickSwitch();
            readInputPS3();
			sendDataPS3((uchar*)&data, 9);
        }
    }
}
