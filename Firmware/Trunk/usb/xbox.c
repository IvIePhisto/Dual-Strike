/* for information about XBox controller see:
	http://euc.jp/periphs/xbox-controller.ja.html
*/

void resetDataXBox() {
	data.array[1] = 20; // size of report

	data.array[0] =
	data.array[2] =
	data.array[3] =
	data.array[4] =
	data.array[5] =
	data.array[6] =
	data.array[7] =
	data.array[8] =
	data.array[9] =
	data.array[10] =
	data.array[11] =
	data.array[12] =
	data.array[13] =
	data.array[14] =
	data.array[15] =
	data.array[16] =
	data.array[17] =
	data.array[18] =
	data.array[19] = 0;
}

#define XBOX_DPAD_UP	{ data.array[2] |= (1<<0); }
#define XBOX_DPAD_DOWN	{ data.array[2] |= (1<<1); }
#define XBOX_DPAD_LEFT	{ data.array[2] |= (1<<2); }
#define XBOX_DPAD_RIGHT	{ data.array[2] |= (1<<3); }
#define XBOX_START		{ data.array[2] |= (1<<4); }
#define XBOX_BACK		{ data.array[2] |= (1<<5); }
#define XBOX_L_STICK	{ data.array[2] |= (1<<6); }
#define XBOX_R_STICK	{ data.array[2] |= (1<<7); }

#define XBOX_A			{ data.array[4] |= 0xFF; }
#define XBOX_B			{ data.array[5] |= 0xFF; }
#define XBOX_X			{ data.array[6] |= 0xFF; }
#define XBOX_Y			{ data.array[7] |= 0xFF; }
#define XBOX_BLACK		{ data.array[8] |= 0xFF; }
#define XBOX_WHITE		{ data.array[9] |= 0xFF; }
#define XBOX_LT			{ data.array[10] |= 0xFF; }
#define XBOX_RT			{ data.array[11] |= 0xFF; }
#define XBOX_LS_RIGHT	{ data.array[12] |= 0xFF; data.array[13] |= 0b01111111; }
#define XBOX_LS_LEFT	{ data.array[13] &= 0b10000000; }
#define XBOX_LS_UP		{ data.array[14] |= 0xFF; data.array[15] |= 0b01111111; }
#define XBOX_LS_DOWN	{ data.array[15] &= 0b10000000; }
#define XBOX_RS_RIGHT	{ data.array[16] |= 0xFF; data.array[17] |= 0b01111111; }
#define XBOX_RS_LEFT	{ data.array[17] &= 0b10000000; }
#define XBOX_RS_UP		{ data.array[18] |= 0xFF; data.array[19] |= 0b01111111; }
#define XBOX_RS_DOWN	{ data.array[19] &= 0b10000000; }

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
	usbMode = USB_MODE_XBOX;
	setupUSB();
	resetDataXBox();
	sendDataUSB(data.array, 20);

    while(1) { /* main event loop */
		usbPoll();

        if(usbInterruptIsReady()) {
			readJoystickSwitch();
            readInputXBox();
			sendDataUSB(data.array, 20);
        }
    }
}
