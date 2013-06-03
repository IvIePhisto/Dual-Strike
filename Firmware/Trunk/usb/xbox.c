/* for information about XBox controller see:
	http://euc.jp/periphs/xbox-controller.ja.html
*/

void initDataXBox() {
	data.array[0] =  0;  //  0 - no effect, report ID?
	data.array[1] = 20;  // 20 - no effect, size of report?
	data.array[3] =  0;  //  0 - no effect

	// Vendor Request data
	data.array[20] = 16; // 16 - must be greater than 7, length of this report?
	data.array[21] = 66; // 66 - needed, USB interface subclass?
	data.array[22] =  0; //  0 - needed, USB interface protocol?
	data.array[23] =  1; //  1 - must be greater than 0, number of interfaces?
	data.array[24] =  1; //  1 - needed, configuration index?
	data.array[25] =  2; //  2 - must be greater than 0, number of endpoints?
	data.array[26] = 20; // 20 - must be less or equal than max packet size for in endpoint, in max packet size?
	data.array[27] =  6; //  6 - must be less or equal than max packet size for out endpoint, out max packet size?

	for(int i = 8; i < 16; i++)
		data.array[20 + i] = 0xFF;

}

void resetDataXBox() {
	data.array[2] =		// DPAD
	data.array[4] =		// A
	data.array[5] =		// B
	data.array[6] =		// X
	data.array[7] =		// Y
	data.array[8] =		// black
	data.array[9] =		// white
	data.array[10] =	// LT
	data.array[11] =	// RT
	data.array[12] =	// left analogue x high byte
	data.array[13] =	// left analogue x low byte
	data.array[14] =	// left analogue y high byte
	data.array[15] =	// left analogue y low byte
	data.array[16] =	// right analogue x high byte
	data.array[17] =	// right analogue x low byte
	data.array[18] =	// right analogue y high byte
	data.array[19] = 0;	// right analogue y low byte
	
	// analogue axes are north/east positive
}

#define XBOX_DPAD_UP	{ data.array[2] |= (1<<0); }
#define XBOX_DPAD_DOWN	{ data.array[2] |= (1<<1); }
#define XBOX_DPAD_LEFT	{ data.array[2] |= (1<<2); }
#define XBOX_DPAD_RIGHT	{ data.array[2] |= (1<<3); }
#define XBOX_START		{ data.array[2] |= (1<<4); }
#define XBOX_BACK		{ data.array[2] |= (1<<5); }
#define XBOX_L_STICK	{ data.array[2] |= (1<<6); }
#define XBOX_R_STICK	{ data.array[2] |= (1<<7); }

#define XBOX_A			{  data.array[4] = 0xFF; }
#define XBOX_B			{  data.array[5] = 0xFF; }
#define XBOX_X			{  data.array[6] = 0xFF; }
#define XBOX_Y			{  data.array[7] = 0xFF; }
#define XBOX_BLACK		{  data.array[8] = 0xFF; }
#define XBOX_WHITE		{  data.array[9] = 0xFF; }
#define XBOX_LT			{ data.array[10] = 0xFF; }
#define XBOX_RT			{ data.array[11] = 0xFF; }
#define XBOX_LS_RIGHT	{ data.array[12] = 0xFF; data.array[13] = 0b01111111; }
#define XBOX_LS_LEFT	{ data.array[13] = 0b10000000; }
#define XBOX_LS_UP		{ data.array[14] = 0xFF; data.array[15] = 0b01111111; }
#define XBOX_LS_DOWN	{ data.array[15] = 0b10000000; }
#define XBOX_RS_RIGHT	{ data.array[16] = 0xFF; data.array[17] = 0b01111111; }
#define XBOX_RS_LEFT	{ data.array[17] = 0b10000000; }
#define XBOX_RS_UP		{ data.array[18] = 0xFF; data.array[19] = 0b01111111; }
#define XBOX_RS_DOWN	{ data.array[19] = 0b10000000; }

void readInputXBox() {
	resetDataXBox();
	updateStickState();	

	if(CFG_JOYSTICK_SWITCH_READ || !metaPressed) {
		// Left Joystick Directions
		if(CFG_LEFT_STICK) {
			if(STICK_STATE_SIGNAL(stickState, STICK_STATE_UP))
				XBOX_LS_UP
			
			if(STICK_STATE_SIGNAL(stickState, STICK_STATE_DOWN))
				XBOX_LS_DOWN

			if(STICK_STATE_SIGNAL(stickState, STICK_STATE_LEFT))
				XBOX_LS_LEFT
			
			if(STICK_STATE_SIGNAL(stickState, STICK_STATE_RIGHT))
				XBOX_LS_RIGHT
		}

		// Right Joystick Directions
		if(CFG_RIGHT_STICK) {
			if(STICK_STATE_SIGNAL(stickState, STICK_STATE_UP))
				XBOX_RS_UP
			
			if(STICK_STATE_SIGNAL(stickState, STICK_STATE_DOWN))
				XBOX_RS_DOWN
		
			if(STICK_STATE_SIGNAL(stickState, STICK_STATE_LEFT))
				XBOX_RS_LEFT
			
			if(STICK_STATE_SIGNAL(stickState, STICK_STATE_RIGHT))
				XBOX_RS_RIGHT
		}

		// Digital Pad Directions
		if(CFG_DIGITAL_PAD) {
			if(STICK_STATE_SIGNAL(stickState, STICK_STATE_UP))
				XBOX_DPAD_UP
			
			if(STICK_STATE_SIGNAL(stickState, STICK_STATE_DOWN))
				XBOX_DPAD_DOWN
		
			if(STICK_STATE_SIGNAL(stickState, STICK_STATE_LEFT))
				XBOX_DPAD_LEFT
			
			if(STICK_STATE_SIGNAL(stickState, STICK_STATE_RIGHT))
				XBOX_DPAD_RIGHT
		}
	}

	// Buttons
	if(!Stick_LP)
		XBOX_X

	if(!Stick_MP)
		XBOX_Y

	if(!Stick_HP) {
		if(CFG_EMU_4X && metaPressed) {
			XBOX_LT
			metaWasUsed = 1;
		}
		else
			XBOX_BLACK
	}
		

	if(!Stick_LK)
		XBOX_A

	if(!Stick_MK)
		XBOX_B

	if(!Stick_HK) {
		if(CFG_EMU_4X && metaPressed) {
			XBOX_RT
			metaWasUsed = 1;
		}
		else
			XBOX_WHITE
	}

#ifdef EXTRA_BUTTONS
	if(!Stick_4P)
		XBOX_LT

	if(!Stick_4K)
		XBOX_RT
#endif

	if(CFG_JOYSTICK_SWITCH_READ && !CFG_EMU_4X) {
		if(!Stick_Start)
			XBOX_START

		if(!Stick_Select)
			XBOX_BACK
	}
	else if(CFG_META_BUTTON_START) {
		if(metaSendCount)
			XBOX_START

		if(!Stick_Select)
			XBOX_BACK
	}
	else {
		if(!Stick_Start)
			XBOX_START

		if(metaSendCount)
			XBOX_BACK
	}
}

void xbox_init_controller() {
	usbMode = USB_MODE_XBOX;
	initDataXBox();
	setupUSB();
}

void xbox_controller() {
	metaSendRepeats = 20;
	//eeprom_write_word((void*)E2END-1, 0); // DEBUG

    while(1) { /* main event loop */
		usbPoll();
		updateMetaState();
		updateJoystickMode();
        readInputXBox();
		//sendDataUSB3(data.array, 20); // each packet is interpreted by XBox as whole report, works on Windows when max packet size is 8

		while(!usbInterruptIsReady3())
			usbPoll();

		usbSetInterrupt3(data.array, 20); // V-USB is patched to have up to 27 bytes transferred, 20 working for XBox
    }
}
