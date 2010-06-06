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
    1, 							// endpoints excl 0: number of endpoint descriptors to follow  | ORIGINALLY: 2
    0x58,						// USB interface class: Unknown 
    0x42,						// USB interface subclass 
    0,							// USB interface protocol 
    0,          				// string index for interface 
    7,          				// sizeof(usbDescrEndpoint): length of descriptor in bytes 
    USBDESCR_ENDPOINT,			// descriptor type = endpoint 
    0x81,						// IN endpoint number 1 | ORIGINALLY: 0x82
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

void xbox_controller() {
	mode = USB_MODE_XBOX;
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
