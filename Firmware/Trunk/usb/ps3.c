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

PROGMEM char usbHidReportDescriptorPS3[124] = {
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

void resetDataPS3() {
	ps3_report_t* ps3Data = (ps3_report_t*)data;

	ps3Data->buttons[0] =
	ps3Data->buttons[1] = 0;
	ps3Data->hatswitch  = 0b00001111;
	ps3Data->joystick_axes = 0b01010101;
	ps3Data->analogue_buttons[0] =
	ps3Data->analogue_buttons[1] =
	ps3Data->analogue_buttons[2] =
	ps3Data->analogue_buttons[3] =
	ps3Data->analogue_buttons[4] =
	ps3Data->analogue_buttons[5] =
	ps3Data->analogue_buttons[6] =
	ps3Data->analogue_buttons[7] =
	ps3Data->analogue_buttons[8] =
	ps3Data->analogue_buttons[9] =
	ps3Data->analogue_buttons[10] =
	ps3Data->analogue_buttons[11] = 0;
}

#define PS3_SQUARE		{ ps3Data->buttons[0] |= (1<<0); ps3Data->analogue_buttons[7] = 0xFF; }
#define PS3_CROSS		{ ps3Data->buttons[0] |= (1<<1); ps3Data->analogue_buttons[6] = 0xFF; }
#define PS3_CIRCLE		{ ps3Data->buttons[0] |= (1<<2); ps3Data->analogue_buttons[5] = 0xFF; }
#define PS3_TRIANGLE	{ ps3Data->buttons[0] |= (1<<3); ps3Data->analogue_buttons[4] = 0xFF; }
#define PS3_L1			{ ps3Data->buttons[0] |= (1<<4); ps3Data->analogue_buttons[8] = 0xFF; }
#define PS3_R1			{ ps3Data->buttons[0] |= (1<<5); ps3Data->analogue_buttons[9] = 0xFF; }
#define PS3_L2			{ ps3Data->buttons[0] |= (1<<6); ps3Data->analogue_buttons[10] = 0xFF; }
#define PS3_R2			{ ps3Data->buttons[0] |= (1<<7); ps3Data->analogue_buttons[11] = 0xFF; }

#define PS3_SELECT		{ ps3Data->buttons[1] |= (1<<0); }
#define PS3_START		{ ps3Data->buttons[1] |= (1<<1); }
#define PS3_PS			{ ps3Data->buttons[1] |= (1<<4); }

#define PS3_DPAD_UP			{ ps3Data->hatswitch = 0; ps3Data->analogue_buttons[2] = 0xFF; }
#define PS3_DPAD_UP_RIGHT	{ ps3Data->hatswitch = 1; ps3Data->analogue_buttons[2] = 0xFF; ps3Data->analogue_buttons[0] = 0xFF; }
#define PS3_DPAD_RIGHT		{ ps3Data->hatswitch = 2; 								   ps3Data->analogue_buttons[0] = 0xFF; }
#define PS3_DPAD_DOWN_RIGHT	{ ps3Data->hatswitch = 3; ps3Data->analogue_buttons[3] = 0xFF; ps3Data->analogue_buttons[0] = 0xFF; }
#define PS3_DPAD_DOWN		{ ps3Data->hatswitch = 4; ps3Data->analogue_buttons[3] = 0xFF; }
#define PS3_DPAD_DOWN_LEFT	{ ps3Data->hatswitch = 5; ps3Data->analogue_buttons[3] = 0xFF; ps3Data->analogue_buttons[1] = 0xFF; }
#define PS3_DPAD_LEFT		{ ps3Data->hatswitch = 6; 								   ps3Data->analogue_buttons[1] = 0xFF; }
#define PS3_DPAD_UP_LEFT	{ ps3Data->hatswitch = 7; ps3Data->analogue_buttons[2] = 0xFF; ps3Data->analogue_buttons[1] = 0xFF; }

#define PS3_LS_LEFT		{ ps3Data->joystick_axes &= 0b11111100; }
#define PS3_LS_RIGHT	{ ps3Data->joystick_axes |= 0b00000010; ps3Data->joystick_axes &= 0b11111110; }

#define PS3_LS_UP		{ ps3Data->joystick_axes &= 0b11110011; }
#define PS3_LS_DOWN		{ ps3Data->joystick_axes |= 0b00001000; ps3Data->joystick_axes &= 0b11111011; }

#define PS3_RS_LEFT		{ ps3Data->joystick_axes &= 0b11001111; }
#define PS3_RS_RIGHT	{ ps3Data->joystick_axes |= 0b00100000; ps3Data->joystick_axes &= 0b11101111; }

#define PS3_RS_UP		{ ps3Data->joystick_axes &= 0b00111111; }
#define PS3_RS_DOWN		{ ps3Data->joystick_axes |= 0b10000000; ps3Data->joystick_axes &= 0b10111111; }

void readInputPS3() {
	ps3_report_t* ps3Data = (ps3_report_t*)data;
	
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

void ps3_controller() {
	mode = USB_MODE_PS3;
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

usbMsgLen_t usbFunctionSetupPS3(uchar receivedData[8]) {
	usbRequest_t* rq = (void *)receivedData;

    if((rq->bmRequestType & USBRQ_TYPE_MASK) == USBRQ_TYPE_CLASS) {    // class request
		// wValue: ReportType (highbyte), ReportID (lowbyte)
        if(rq->bRequest == USBRQ_HID_GET_REPORT) {
			if(rq->wValue.bytes[1] == HID_REPORT_TYPE_FEATURE) {
				if(rq->wValue.bytes[0] == 0) {
					 // set buffer data
					data[0] = 0x21; // 0b00100001 0d33
					data[1] = 0x26; // 0b00100110 0d38

					for(int i = 2; i < 8; i++)
						data[i] = 0;

					usbMsgPtr = data;

					return 8;
				}
			}
        }
    }

	return 0;
}

