#include "usb_controller.h"

#include "../DualStrike.h"

#include <avr/io.h>
#include <avr/interrupt.h>  /* for sei() */
#include <util/delay.h>     /* for _delay_ms() */

#include <avr/pgmspace.h>   /* required by usbdrv.h */
#include "usbdrv/usbdrv.h"

extern uint8_t config[2];

/* ------------------------------------------------------------------------- */
PROGMEM char usbDescriptorConfiguration[0] = {};

PROGMEM char usbDescriptorConfigurationPS3[] = {
    /* HID USB configuration descriptor */
    9,          				/* sizeof(usbDescriptorConfiguration): length of descriptor in bytes */
    USBDESCR_CONFIG,   	 		/* descriptor type */
    34, 0,      				/* total length of data returned (including inlined descriptors) */
    1,          				/* number of interfaces in this configuration */
    1,          				/* index of this configuration */
    0,          				/* configuration name string index */
    (char)USBATTR_BUSPOWER, 	/* attributes */
    USB_CFG_MAX_BUS_POWER/2,	/* max USB current in 2mA units */
/* interface descriptor follows inline: */
    9,   			 	    	/* sizeof(usbDescrInterface): length of descriptor in bytes */
    USBDESCR_INTERFACE,			/* descriptor type */
    0,          				/* index of this interface */
    0,							/* alternate setting for this interface */
    1, 							/* endpoints excl 0: number of endpoint descriptors to follow */
    3,							/* USB interface class: HID */
    0,							/* USB interface subclass */
    0,							/* USB interface protocol */
    0,          				/* string index for interface */
    9,          				/* sizeof(usbDescrHID): length of descriptor in bytes */
    USBDESCR_HID,   			/* descriptor type: HID */
    0x01, 0x01,					/* BCD representation of HID version */
    0x00,       				/* target country code */
    0x01,       				/* number of HID Report (or other HID class) Descriptor infos to follow */
    0x22,       				/* descriptor type: report */
    124, 0,						/* total length of report descriptor */
    7,          				/* sizeof(usbDescrEndpoint) */
    USBDESCR_ENDPOINT,			/* descriptor type = endpoint */
    0x81,						/* IN endpoint number 1 */
    0x03,						/* attrib: Interrupt endpoint */
    8, 0,						/* maximum packet size */
    1, 							/* interrupt poll interval in ms */
};

PROGMEM char usbHidReportDescriptor[124] = {
    0x05, 0x01,                    // USAGE_PAGE (Generic Desktop)
    0x09, 0x04,                    // USAGE (Joystick)
    0xa1, 0x01,                    // COLLECTION (Application)
    0xa1, 0x02,                    //   COLLECTION (Logical)
    0x75, 0x01,                    //     REPORT_SIZE (1)
    0x95, 0x0d,                    //     REPORT_COUNT (13)
    0x15, 0x00,                    //     LOGICAL_MINIMUM (0)
    0x25, 0x01,                    //     LOGICAL_MAXIMUM (1)
    0x35, 0x00,                    //     PHYSICAL_MINIMUM (0)
    0x45, 0x01,                    //     PHYSICAL_MAXIMUM (1)
    0x05, 0x09,                    //     USAGE_PAGE (Button)
    0x19, 0x01,                    //     USAGE_MINIMUM (Button 1)
    0x29, 0x0d,                    //     USAGE_MAXIMUM (Button 13)
    0x81, 0x02,                    //     INPUT (Data,Var,Abs)
    0x95, 0x03,                    //     REPORT_COUNT (3)
    0x81, 0x01,                    //     INPUT (Cnst,Ary,Abs)
    0x05, 0x01,                    //     USAGE_PAGE (Generic Desktop)
    0x75, 0x04,                    //     REPORT_SIZE (4)
    0x95, 0x01,                    //     REPORT_COUNT (1)
    0x25, 0x07,                    //     LOGICAL_MAXIMUM (7)
    0x46, 0x3b, 0x01,              //     PHYSICAL_MAXIMUM (315)
    0x65, 0x14,                    //     UNIT (Eng Rot:Angular Pos)
    0x09, 0x39,                    //     USAGE (Hat switch)
    0x81, 0x42,                    //     INPUT (Data,Var,Abs,Null)
    0x65, 0x00,                    //     UNIT (None)
    0x95, 0x01,                    //     REPORT_COUNT (1)
    0x81, 0x03,                    //     INPUT (Cnst,Var,Abs)
    0xa1, 0x00,                    //     COLLECTION (Physical)
    0x75, 0x02,                    //       REPORT_SIZE (2)
    0x95, 0x04,                    //       REPORT_COUNT (4)
    0x25, 0x02,                    //       LOGICAL_MAXIMUM (2)
    0x45, 0x02,                    //       PHYSICAL_MAXIMUM (2)
    0x09, 0x30,                    //       USAGE (X)
    0x09, 0x31,                    //       USAGE (Y)
    0x09, 0x32,                    //       USAGE (Z)
    0x09, 0x35,                    //       USAGE (Rz)
    0x81, 0x02,                    //       INPUT (Data,Var,Abs)
    0xc0,                          //     END_COLLECTION
    0xc0,                          //   END_COLLECTION
    0x75, 0x08,                    //   REPORT_SIZE (8)
    0x95, 0x08,                    //   REPORT_COUNT (8)
    0x26, 0xff, 0x00,              //   LOGICAL_MAXIMUM (255)
    0x46, 0xff, 0x00,              //   PHYSICAL_MAXIMUM (255)
    0x06, 0x00, 0xff,              //   USAGE_PAGE (Vendor Defined Page 1)
	0x09, 0x20,                    //   USAGE (Unknown)
	0x09, 0x21,                    //   USAGE (Unknown)
	0x09, 0x22,                    //   USAGE (Unknown)
	0x09, 0x23,                    //   USAGE (Unknown) 
	0x09, 0x24,                    //   USAGE (Unknown)
	0x09, 0x25,                    //   USAGE (Unknown)
	0x09, 0x26,                    //   USAGE (Unknown)
	0x09, 0x27,                    //   USAGE (Unknown)
	0x09, 0x28,                    //   USAGE (Unknown)
	0x09, 0x29,                    //   USAGE (Unknown)
	0x09, 0x2a,                    //   USAGE (Unknown)
	0x09, 0x2b,                    //   USAGE (Unknown)
	0x95, 0x0c,                    //   REPORT_COUNT (12)
    0x81, 0x02,                    //   INPUT (Data,Var,Abs)
    0x0a, 0x21, 0x26,              //   UNKNOWN
    0xb1, 0x02,                    //   FEATURE (Data,Var,Abs)
    0xc0                           // END_COLLECTION
};

typedef struct {
	uchar	buttons[2];
	uchar	hatswitch;
	uchar	joystick_axes;
	uchar	analogue_buttons[12];
} ps3_report_t;

extern ps3_report_t data;

void resetDataPS3() {
	data.buttons[0] =
	data.buttons[1] = 0;
	data.hatswitch  = 0b00001111;
	data.joystick_axes = 0b01010101;
	data.analogue_buttons[0] =
	data.analogue_buttons[1] =
	data.analogue_buttons[2] =
	data.analogue_buttons[3] =
	data.analogue_buttons[4] =
	data.analogue_buttons[5] =
	data.analogue_buttons[6] =
	data.analogue_buttons[7] =
	data.analogue_buttons[8] =
	data.analogue_buttons[9] =
	data.analogue_buttons[10] =
	data.analogue_buttons[11] = 0;
}

#define PS3_SQUARE		{ data.buttons[0] |= (1<<0); data.analogue_buttons[7] = 0xFF; }
#define PS3_CROSS		{ data.buttons[0] |= (1<<1); data.analogue_buttons[6] = 0xFF; }
#define PS3_CIRCLE		{ data.buttons[0] |= (1<<2); data.analogue_buttons[5] = 0xFF; }
#define PS3_TRIANGLE	{ data.buttons[0] |= (1<<3); data.analogue_buttons[4] = 0xFF; }
#define PS3_L1			{ data.buttons[0] |= (1<<4); data.analogue_buttons[8] = 0xFF; }
#define PS3_R1			{ data.buttons[0] |= (1<<5); data.analogue_buttons[9] = 0xFF; }
#define PS3_L2			{ data.buttons[0] |= (1<<6); data.analogue_buttons[10] = 0xFF; }
#define PS3_R2			{ data.buttons[0] |= (1<<7); data.analogue_buttons[11] = 0xFF; }

#define PS3_SELECT		{ data.buttons[1] |= (1<<0); }
#define PS3_START		{ data.buttons[1] |= (1<<1); }
#define PS3_PS			{ data.buttons[1] |= (1<<4); }

#define PS3_DPAD_UP			{ data.hatswitch = 0; data.analogue_buttons[2] = 0xFF; }
#define PS3_DPAD_UP_RIGHT	{ data.hatswitch = 1; data.analogue_buttons[2] = 0xFF; data.analogue_buttons[0] = 0xFF; }
#define PS3_DPAD_RIGHT		{ data.hatswitch = 2; 								   data.analogue_buttons[0] = 0xFF; }
#define PS3_DPAD_DOWN_RIGHT	{ data.hatswitch = 3; data.analogue_buttons[3] = 0xFF; data.analogue_buttons[0] = 0xFF; }
#define PS3_DPAD_DOWN		{ data.hatswitch = 4; data.analogue_buttons[3] = 0xFF; }
#define PS3_DPAD_DOWN_LEFT	{ data.hatswitch = 5; data.analogue_buttons[3] = 0xFF; data.analogue_buttons[1] = 0xFF; }
#define PS3_DPAD_LEFT		{ data.hatswitch = 6; 								   data.analogue_buttons[1] = 0xFF; }
#define PS3_DPAD_UP_LEFT	{ data.hatswitch = 7; data.analogue_buttons[2] = 0xFF; data.analogue_buttons[1] = 0xFF; }

#define PS3_LS_LEFT		{ data.joystick_axes &= 0b11111100; }
#define PS3_LS_RIGHT	{ data.joystick_axes |= 0b00000010; data.joystick_axes &= 0b11111110; }

#define PS3_LS_UP		{ data.joystick_axes &= 0b11110011; }
#define PS3_LS_DOWN		{ data.joystick_axes |= 0b00001000; data.joystick_axes &= 0b11111011; }

#define PS3_RS_LEFT		{ data.joystick_axes &= 0b11001111; }
#define PS3_RS_RIGHT	{ data.joystick_axes |= 0b00100000; data.joystick_axes &= 0b11101111; }

#define PS3_RS_UP		{ data.joystick_axes &= 0b00111111; }
#define PS3_RS_DOWN		{ data.joystick_axes |= 0b10000000; data.joystick_axes &= 0b10111111; }


PROGMEM const unsigned char usbDescriptorConfigurationXBox[] = {
	// XBox USB configuration descriptor 
    9,          				// sizeof(usbDescriptorConfiguration): length of descriptor in bytes 
    USBDESCR_CONFIG,   	 		// descriptor type 
    32-7, 0,      				// total length of data returned (including inlined descriptors) 
    1,          				// number of interfaces in this configuration 
    1,          				// index of this configuration 
    0,          				// configuration name string index 
    (char)USBATTR_BUSPOWER, 	// attributes 
    USB_CFG_MAX_BUS_POWER/2,	// max USB current in 2mA units 
// interface descriptor follows inline: 
    9,   			 	    	// sizeof(usbDescrInterface): length of descriptor in bytes 
    USBDESCR_INTERFACE,			// descriptor type 
    0,          				// index of this interface 
    0,							// alternate setting for this interface 
    1, 							// endpoints excl 0: number of endpoint descriptors to follow 
    0x58,						// USB interface class: Unknown 
    0x42,						// USB interface subclass 
    0,							// USB interface protocol 
    0,          				// string index for interface 
    7,          				// sizeof(usbDescrEndpoint): length of descriptor in bytes 
    USBDESCR_ENDPOINT,			// descriptor type = endpoint 
    0x81,						// IN endpoint number 1 ORIGINALLY: 0x82
    0x03,						// attrib: Interrupt endpoint 
    8, 0,						// maximum packet size 
    4, 							// interrupt poll interval in ms 
	/*
    7,          				// sizeof(usbDescrEndpoint): length of descriptor in bytes 
    USBDESCR_ENDPOINT,			// descriptor type = endpoint 
    0x02,						// OUT endpoint number 2 
    0x03,						// attrib: Interrupt endpoint 
    8, 0,						// maximum packet size 
    4, 							// interrupt poll interval in ms 
	*/
};

/* for information about XBox controller see:
	http://euc.jp/periphs/xbox-controller.ja.html
*/

void resetDataXBox() {
	((uchar*)&data)[1] = 20; // size of report

	((uchar*)&data)[0] =
	((uchar*)&data)[2] =
	((uchar*)&data)[3] =
	((uchar*)&data)[4] =
	((uchar*)&data)[5] =
	((uchar*)&data)[6] =
	((uchar*)&data)[7] =
	((uchar*)&data)[8] =
	((uchar*)&data)[9] =
	((uchar*)&data)[10] =
	((uchar*)&data)[11] =
	((uchar*)&data)[12] =
	((uchar*)&data)[13] =
	((uchar*)&data)[14] =
	((uchar*)&data)[15] =
	((uchar*)&data)[16] =
	((uchar*)&data)[17] =
	((uchar*)&data)[18] =
	((uchar*)&data)[19] = 0;
}

#define XBOX_DPAD_UP	{ ((uchar*)&data)[2] |= (1<<0); }
#define XBOX_DPAD_DOWN	{ ((uchar*)&data)[2] |= (1<<1); }
#define XBOX_DPAD_LEFT	{ ((uchar*)&data)[2] |= (1<<2); }
#define XBOX_DPAD_RIGHT	{ ((uchar*)&data)[2] |= (1<<3); }
#define XBOX_START		{ ((uchar*)&data)[2] |= (1<<4); }
#define XBOX_BACK		{ ((uchar*)&data)[2] |= (1<<5); }
#define XBOX_L_STICK	{ ((uchar*)&data)[2] |= (1<<6); }
#define XBOX_R_STICK	{ ((uchar*)&data)[2] |= (1<<7); }

#define XBOX_A			{ ((uchar*)&data)[4] |= 0xFF; }
#define XBOX_B			{ ((uchar*)&data)[5] |= 0xFF; }
#define XBOX_X			{ ((uchar*)&data)[6] |= 0xFF; }
#define XBOX_Y			{ ((uchar*)&data)[7] |= 0xFF; }
#define XBOX_BLACK		{ ((uchar*)&data)[8] |= 0xFF; }
#define XBOX_WHITE		{ ((uchar*)&data)[9] |= 0xFF; }
#define XBOX_LT			{ ((uchar*)&data)[10] |= 0xFF; }
#define XBOX_RT			{ ((uchar*)&data)[11] |= 0xFF; }
#define XBOX_LS_RIGHT	{ ((uchar*)&data)[12] |= 0xFF; ((uchar*)&data)[13] |= 0b01111111; }
#define XBOX_LS_LEFT	{ ((uchar*)&data)[12] |= 0;    ((uchar*)&data)[13] |= 0b10000000; }
#define XBOX_LS_UP		{ ((uchar*)&data)[14] |= 0xFF; ((uchar*)&data)[15] |= 0b01111111; }
#define XBOX_LS_DOWN	{ ((uchar*)&data)[14] |= 0;    ((uchar*)&data)[15] |= 0b10000000; }
#define XBOX_RS_RIGHT	{ ((uchar*)&data)[16] |= 0xFF; ((uchar*)&data)[17] |= 0b01111111; }
#define XBOX_RS_LEFT	{ ((uchar*)&data)[16] |= 0;    ((uchar*)&data)[17] |= 0b10000000; }
#define XBOX_RS_UP		{ ((uchar*)&data)[18] |= 0xFF; ((uchar*)&data)[19] |= 0b01111111; }
#define XBOX_RS_DOWN	{ ((uchar*)&data)[18] |= 0;    ((uchar*)&data)[19] |= 0b10000000; }

#define USB_MODE_PS3 1
#define USB_MODE_XBOX 2

uchar mode = USB_MODE_PS3;

usbMsgLen_t usbFunctionDescriptor(struct usbRequest *rq)
{
	usbMsgLen_t len = 0;

    switch(rq->wValue.bytes[1]) {
    case USBDESCR_CONFIG:
		if(mode == USB_MODE_PS3) {
        	usbMsgPtr = (uchar *)(usbDescriptorConfigurationPS3);
        	len = sizeof(usbDescriptorConfigurationPS3);
		}
		else if(mode == USB_MODE_XBOX) {
        	usbMsgPtr = (uchar *)(usbDescriptorConfigurationXBox);
        	len = sizeof(usbDescriptorConfigurationXBox);
		}
		else
			return 0;

		break;
    }

    return len;
}

#define HID_REPORT_TYPE_INPUT 1
#define HID_REPORT_TYPE_OUTPUT 2
#define HID_REPORT_TYPE_FEATURE 3

usbMsgLen_t usbFunctionSetup(uchar receivedData[8])
{
	if(mode != USB_MODE_PS3)
		return 0;

	usbRequest_t    *rq = (void *)receivedData;

    if((rq->bmRequestType & USBRQ_TYPE_MASK) == USBRQ_TYPE_CLASS) {    // class request
		// wValue: ReportType (highbyte), ReportID (lowbyte)
        if(rq->bRequest == USBRQ_HID_GET_REPORT) {
			if(rq->wValue.bytes[1] == HID_REPORT_TYPE_FEATURE) {
				if(rq->wValue.bytes[0] == 0) {
					 // set buffer data
					((uchar*)&data)[0] = 0x21; // 0b00100001 0d33
					((uchar*)&data)[1] = 0x26; // 0b00100110 0d38

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

/* ------------------------------------------------------------------------- */

void readInputXBox() {
	resetDataXBox();

	// Left Joystick Directions
	if(CFG_LEFT_STICK) {
		if (!Stick_Up)
			XBOX_LS_UP
		else if (!Stick_Down)
			XBOX_LS_DOWN

		if (!Stick_Left)
			XBOX_LS_LEFT
		else if (!Stick_Right)
			XBOX_LS_RIGHT
	}

	// Right Joystick Directions
	if(CFG_RIGHT_STICK) {
		if (!Stick_Up)
			XBOX_RS_UP
		else if (!Stick_Down)
			XBOX_RS_DOWN
		
		if (!Stick_Left)
			XBOX_RS_LEFT
		else if (!Stick_Right)
			XBOX_RS_RIGHT
	}

	// Digital Pad Directions
	if(CFG_DIGITAL_PAD) {
		if (!Stick_Up) {
			XBOX_DPAD_UP
		}
		else if (!Stick_Down) {
			XBOX_DPAD_DOWN
		}
		else if (!Stick_Left)
			XBOX_DPAD_LEFT
		else if (!Stick_Right)
			XBOX_DPAD_RIGHT
	}

	// Buttons
	if(!Stick_Jab)
		XBOX_X

	if(!Stick_Strong)
		XBOX_Y

	if(!Stick_Fierce)
		XBOX_BLACK

	if(!Stick_Short)
		XBOX_A

	if(!Stick_Forward)
		XBOX_B

	if(!Stick_Roundhouse)
		XBOX_BLACK

#ifdef EXTRA_BUTTONS					
	if(!Stick_Extra0)
		XBOX_LT

	if(!Stick_Extra1)
		XBOX_RT
#endif

	if(!Stick_Start)
		XBOX_START

	if(!Stick_Select)
		XBOX_BACK
}

/* ------------------------------------------------------------------------- */

void readInputPS3() {
	resetDataPS3();

	// Left Joystick Directions
	if(CFG_LEFT_STICK) {
		if (!Stick_Up)
			PS3_LS_UP
		else if (!Stick_Down)
			PS3_LS_DOWN

		if (!Stick_Left)
			PS3_LS_LEFT
		else if (!Stick_Right)
			PS3_LS_RIGHT
	}

	// Right Joystick Directions
	if(CFG_RIGHT_STICK) {
		if (!Stick_Up)
			PS3_RS_UP
		else if (!Stick_Down)
			PS3_RS_DOWN
		
		if (!Stick_Left)
			PS3_RS_LEFT
		else if (!Stick_Right)
			PS3_RS_RIGHT
	}

	// Digital Pad Directions
	if(CFG_DIGITAL_PAD) {
		if (!Stick_Up) {
			if (!Stick_Left)
				PS3_DPAD_UP_LEFT
			else if (!Stick_Right)
				PS3_DPAD_UP_RIGHT
			else
				PS3_DPAD_UP
		}
		else if (!Stick_Down) {
			if (!Stick_Left)
				PS3_DPAD_DOWN_LEFT
			else if (!Stick_Right)
				PS3_DPAD_DOWN_RIGHT
			else
				PS3_DPAD_DOWN
		}
		else if (!Stick_Left)
			PS3_DPAD_LEFT
		else if (!Stick_Right)
			PS3_DPAD_RIGHT
	}

	// Buttons
	if(!Stick_Jab)
		PS3_SQUARE

	if(!Stick_Strong)
		PS3_TRIANGLE

	if(!Stick_Fierce)
		PS3_R1

	if(!Stick_Short)
		PS3_CROSS

	if(!Stick_Forward)
		PS3_CIRCLE

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

void sendUSBData(uchar* data, unsigned int byteCount) {
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

void usbPrepare() {
    usbDeviceDisconnect(); /* enforce re-enumeration, do this while interrupts are disabled! */
    _delay_ms(300UL);/* fake USB disconnect for > 250 ms */
    usbDeviceConnect();
    usbInit();
    sei();
	usbPoll();
}

void ps3_controller() {
	usbPrepare();
	resetDataPS3();
	sendUSBData((uchar*)&data, 16);

    while(1) { /* main event loop */
		usbPoll();

        if(usbInterruptIsReady()) {
			readJoystickSwitch();
            readInputPS3();
			sendUSBData((uchar*)&data, 16);
        }
    }
}

void xbox_controller() {
	usbPrepare();
	resetDataXBox();
	sendUSBData((uchar*)&data, 20);

    while(1) { /* main event loop */
		usbPoll();

        if(usbInterruptIsReady()) {
			readJoystickSwitch();
            readInputXBox();
			sendUSBData((uchar*)&data, 20);
        }
    }
}
