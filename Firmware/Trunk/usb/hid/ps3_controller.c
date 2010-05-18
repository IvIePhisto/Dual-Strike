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
/* based on: 
   http://wiki.ps2dev.org/ps3:hardware:sixaxis
   http://ps3.jim.sh/sixaxis/usb/
*/

	uchar	reportID;
	uchar	unknown1; // sample: 0x00

	/*
	byte 0 from low to high bit:
		Select
		L3
		R3
		Start
		DPad Up
		DPad Right
		DPad Down
		DPad Left

	byte 1 from low to high bit:
		L2
		R2
		L1
		R1
		Triangle
		Circle
		Cross
		Square

	byte 2 from low to high bit:
		PS
		rest: unknown
	*/
	uchar	buttons[3];

	// maybe placeholder for keycode
	uchar	unknown2; 

	uchar	lx;
	uchar	ly;
	uchar	rx;
	uchar	ry;

	uchar   analogue_select;	// GUESS!
	uchar   analogue_l3;		// GUESS!
	uchar   analogue_r3;		// GUESS!
	uchar   analogue_start;		// GUESS!
	uchar	analogue_dpad_up;
	uchar	analogue_dpad_right;
	uchar	analogue_dpad_down;
	uchar	analogue_dpad_left;
	uchar	analogue_l2;
	uchar	analogue_r2;
	uchar	analogue_l1;
	uchar	analogue_r1;
	uchar	analogue_triangle;
	uchar	analogue_circle;
	uchar	analogue_cross;
	uchar	analogue_square;
	uchar   analogue_ps;

	uchar	unknown3[2];

	// ? 0x03 when operating normally, 0x02 when charging cable plugged in
	uchar	charging_status;
	
	// ? 0x05=full, 0x02=dying, 0x01=just before shutdown, 0xEE=charging
	uchar	power_status;

	// ? 0x14 when operating by bluetooth, 0x10 when operating by bluetooth with cable plugged in, 0x16 when bluetooth and rumble 
	uchar	connection_type;

	// some values seem to fluctuate - maybe voltage and signal quality readings?
	// sample: 0xFF 0xB9 0x00 0x00 0x23 0x16 0x77 0x01 0x81
	uchar	unknown4[9];

	uchar	acceleration_x;
	uchar	sin_roll;
	uchar	acceleration_y;
	uchar	sin_pitch;
	uchar	acceleration_z;
	uchar	gravity_x;
	uchar	gravity_z1;
	uchar	gravity_z2;
} report_t;

extern report_t data;

#define PS3_SELECT		{ data.buttons[0] |= (1<<0); data.analogue_select = 0xFF; }
#define PS3_L3			{ data.buttons[0] |= (1<<1); data.analogue_l3 = 0xFF; }
#define PS3_R3			{ data.buttons[0] |= (1<<2); data.analogue_r3 = 0xFF; }
#define PS3_START		{ data.buttons[0] |= (1<<3); data.analogue_start = 0xFF; }
#define PS3_DPAD_UP		{ data.buttons[0] |= (1<<4); data.analogue_dpad_up = 0xFF; }
#define PS3_DPAD_RIGHT	{ data.buttons[0] |= (1<<5); data.analogue_dpad_right = 0xFF; }
#define PS3_DPAD_DOWN	{ data.buttons[0] |= (1<<6); data.analogue_dpad_down = 0xFF; }
#define PS3_DPAD_LEFT	{ data.buttons[0] |= (1<<7); data.analogue_dpad_left = 0xFF; }

#define PS3_L2			{ data.buttons[1] |= (1<<0); data.analogue_l2 = 0xFF; }
#define PS3_R2			{ data.buttons[1] |= (1<<1); data.analogue_r2 = 0xFF; }
#define PS3_L1			{ data.buttons[1] |= (1<<2); data.analogue_l1 = 0xFF; }
#define PS3_R1			{ data.buttons[1] |= (1<<3); data.analogue_r1 = 0xFF; }
#define PS3_TRIANGLE	{ data.buttons[1] |= (1<<4); data.analogue_triangle = 0xFF; }
#define PS3_CIRCLE		{ data.buttons[1] |= (1<<5); data.analogue_circle = 0xFF; }
#define PS3_CROSS		{ data.buttons[1] |= (1<<6); data.analogue_cross = 0xFF; }
#define PS3_SQUARE		{ data.buttons[1] |= (1<<7); data.analogue_square = 0xFF; }

#define PS3_PS			{ data.buttons[2] |= (1<<0); data.analogue_ps = 0xFF; }

void resetReportBuffer() {
	data.reportID = 1;
	data.unknown1 =
	data.buttons[0] =
	data.buttons[1] = 
	data.buttons[2] =
	data.unknown2 = 0;

	data.lx =
	data.ly =
	data.rx =
	data.ry = 0b10000000;

	data.analogue_select =
	data.analogue_l3 =
	data.analogue_r3 =
	data.analogue_start =
	data.analogue_dpad_up =
	data.analogue_dpad_right =
	data.analogue_dpad_down =
	data.analogue_dpad_left =
	data.analogue_l2 =
	data.analogue_r2 =
	data.analogue_l1 =
	data.analogue_r1 = 
	data.analogue_triangle =
	data.analogue_circle =
	data.analogue_cross =
	data.analogue_square =
	data.analogue_ps =
	data.unknown3[0] =
	data.unknown3[1] = 0;

	data.charging_status = 0x03;
	data.power_status = 0x05;
	data.connection_type = 0;

	data.unknown4[0] = 0xFF;
	data.unknown4[1] = 0xB9;
	data.unknown4[2] =
	data.unknown4[3] = 0;
	data.unknown4[4] = 0x23;
	data.unknown4[5] = 0x16;
	data.unknown4[6] = 0x77;
	data.unknown4[7] = 0x01;
	data.unknown4[8] = 0x81;
	/*
	data.unknown4[0] =
	data.unknown4[1] =
	data.unknown4[2] =
	data.unknown4[3] =
	data.unknown4[4] =
	data.unknown4[5] =
	data.unknown4[6] =
	data.unknown4[7] =
	data.unknown4[8] =
	*/

	data.acceleration_x =
	data.sin_roll =
	data.acceleration_y =
	data.sin_pitch =
	data.acceleration_z =
	data.gravity_x =
	data.gravity_z1 =
	data.gravity_z2 = 0;
}

PROGMEM char usbHidReportDescriptor[148] = {
    0x05, 0x01,                    // USAGE_PAGE (Generic Desktop)
    0x09, 0x04,                    // USAGE (Joystick)
    0xa1, 0x01,                    // COLLECTION (Application)
    0xa1, 0x02,                    //   COLLECTION (Logical)
    0x85, 0x01,                    //     REPORT_ID (1)
    0x75, 0x08,                    //     REPORT_SIZE (8)
    0x95, 0x01,                    //     REPORT_COUNT (1)
    0x15, 0x00,                    //     LOGICAL_MINIMUM (0)
    0x26, 0xff, 0x00,              //     LOGICAL_MAXIMUM (255)
    0x81, 0x03,                    //     INPUT (Cnst,Var,Abs)
	// 21
	// Input: 1 * 1 byte = 1 byte
    0x75, 0x01,                    //     REPORT_SIZE (1)
    0x95, 0x13,                    //     REPORT_COUNT (19)
    0x15, 0x00,                    //     LOGICAL_MINIMUM (0)
    0x25, 0x01,                    //     LOGICAL_MAXIMUM (1)
    0x35, 0x00,                    //     PHYSICAL_MINIMUM (0)
    0x45, 0x01,                    //     PHYSICAL_MAXIMUM (1)
    0x05, 0x09,                    //     USAGE_PAGE (Button)
    0x19, 0x01,                    //     USAGE_MINIMUM (Button 1)
    0x29, 0x13,                    //     USAGE_MAXIMUM (Button 19)
    0x81, 0x02,                    //     INPUT (Data,Var,Abs)
	// 20
	// Input: 19 * 1 bit = 2 byte + 3 bit
    0x75, 0x01,                    //     REPORT_SIZE (1)
    0x95, 0x0d,                    //     REPORT_COUNT (13)
    0x06, 0x00, 0xff,              //     USAGE_PAGE (Vendor Defined Page 1)
    0x81, 0x03,                    //     INPUT (Cnst,Var,Abs)
	// 9
	// Input: 13 * 1 bit = 1 byte + 5 bit
    0x15, 0x00,                    //     LOGICAL_MINIMUM (0)
    0x26, 0xff, 0x00,              //     LOGICAL_MAXIMUM (255)
    0x05, 0x01,                    //     USAGE_PAGE (Generic Desktop)
    0x09, 0x01,                    //     USAGE (Pointer)
    0xa1, 0x00,                    //     COLLECTION (Physical)
    0x75, 0x08,                    //       REPORT_SIZE (8)
    0x95, 0x04,                    //       REPORT_COUNT (4)
    0x35, 0x00,                    //       PHYSICAL_MINIMUM (0)
    0x46, 0xff, 0x00,              //       PHYSICAL_MAXIMUM (255)
    0x09, 0x30,                    //       USAGE (X)
    0x09, 0x31,                    //       USAGE (Y)
    0x09, 0x32,                    //       USAGE (Z)
    0x09, 0x35,                    //       USAGE (Rz)
    0x81, 0x02,                    //       INPUT (Data,Var,Abs)
	// Input: 4 * 1 byte = 4 byte
    0xc0,                          //     END_COLLECTION
    0x05, 0x01,                    //     USAGE_PAGE (Generic Desktop)
    0x75, 0x08,                    //     REPORT_SIZE (8)
    0x95, 0x27,                    //     REPORT_COUNT (39)
    0x09, 0x01,                    //     USAGE (Pointer)
    0x81, 0x02,                    //     INPUT (Data,Var,Abs)
	// Input: 39 * 1 byte = 39 byte
    0x75, 0x08,                    //     REPORT_SIZE (8)
    0x95, 0x30,                    //     REPORT_COUNT (48)
    0x09, 0x01,                    //     USAGE (Pointer)
    0x91, 0x02,                    //     OUTPUT (Data,Var,Abs)
	// Output: 48 * 1 byte = 48 byte
    0x75, 0x08,                    //     REPORT_SIZE (8)
    0x95, 0x30,                    //     REPORT_COUNT (48)
    0x09, 0x01,                    //     USAGE (Pointer)
    0xb1, 0x02,                    //     FEATURE (Data,Var,Abs)
	// Feature: 48 * 1 byte = 48 byte
    0xc0,                          //   END_COLLECTION
    0xa1, 0x02,                    //   COLLECTION (Logical)
    0x85, 0x02,                    //     REPORT_ID (2)
    0x75, 0x08,                    //     REPORT_SIZE (8)
    0x95, 0x30,                    //     REPORT_COUNT (48)
    0x09, 0x01,                    //     USAGE (Pointer)
    0xb1, 0x02,                    //     FEATURE (Data,Var,Abs)
	// Feature: 48 * 1 byte = 48 byte
    0xc0,                          //     END_COLLECTION
    0xa1, 0x02,                    //   COLLECTION (Logical)
    0x85, 0xee,                    //     REPORT_ID (238)
    0x75, 0x08,                    //     REPORT_SIZE (8)
    0x95, 0x30,                    //     REPORT_COUNT (48)
    0x09, 0x01,                    //     USAGE (Pointer)
    0xb1, 0x02,                    //     FEATURE (Data,Var,Abs)
	// Feature: 48 * 1 byte = 48 byte
    0xc0,                          //   END_COLLECTION
    0xa1, 0x02,                    //   COLLECTION (Logical)
    0x85, 0xef,                    //     REPORT_ID (239)
    0x75, 0x08,                    //     REPORT_SIZE (8)
    0x95, 0x30,                    //     REPORT_COUNT (48)
    0x09, 0x00,                    //     USAGE (Undefined)
    0xb1, 0x02,                    //     FEATURE (Data,Var,Abs)
	// Feature: 48 * 1 byte = 48 byte
    0xc0,                          //   END_COLLECTION
    0xc0                           // END_COLLECTION
};

#define HID_REPORT_TYPE_INPUT 1
#define HID_REPORT_TYPE_OUTPUT 2
#define HID_REPORT_TYPE_FEATURE 3

usbMsgLen_t usbFunctionSetup(uchar receivedData[8])
{
	return 0;
	usbRequest_t    *rq = (void *)receivedData;

    if((rq->bmRequestType & USBRQ_TYPE_MASK) == USBRQ_TYPE_CLASS) {    // class request
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

    return 0;   // default for not implemented requests: return no data back to host
}


/* ------------------------------------------------------------------------- */

void readInputPS3()
{
	resetReportBuffer();

	// Left Joystick Directions
	if(CFG_LEFT_STICK) {
		if (!Stick_Up)
			data.ly = 0x00;
		else if (!Stick_Down)
			data.ly = 0xFF;

		if (!Stick_Left)
			data.lx = 0x00;
		else if (!Stick_Right)
			data.lx = 0xFF;
	}

	// Right Joystick Directions
	if(CFG_RIGHT_STICK) {
		if (!Stick_Up)
			data.ry = 0;
		else if (!Stick_Down)
			data.ry = 0xFF;
		
		if (!Stick_Left)
			data.rx = 0;
		else if (!Stick_Right)
			data.rx = 0xFF;
	}

	// Digital Pad Directions
	if(CFG_DIGITAL_PAD) {
		if (!Stick_Up)
			PS3_DPAD_UP
		else if (!Stick_Down)
			PS3_DPAD_DOWN
		
		if (!Stick_Left)
			PS3_DPAD_LEFT
		else if (!Stick_Right)
			PS3_DPAD_RIGHT
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

	if(!Stick_Roundhouse)
		PS3_R2

#ifdef EXTRA_BUTTONS					
	if(!Stick_Extra0)
		PS3_L1

	if(!Stick_Extra1)
		PS3_L2
#endif

	if(CFG_HOME_EMU && !Stick_Start && !Stick_Select)
		PS3_PS
	else {
		if(!Stick_Start)
			PS3_START

		if(!Stick_Select)
			PS3_SELECT
	}

	if(!Stick_Home)
		PS3_PS
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

	/*
	if(currentByte == byteCount -1) {
		usbSetInterrupt(data + currentByte, 0);
		while(!usbInterruptIsReady()) usbPoll();
	}
	*/
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
			sendDataPS3((uchar*)&data, 49);
        }
    }
}
