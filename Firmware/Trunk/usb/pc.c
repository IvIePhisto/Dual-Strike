void resetPCReportBuffer() {
	data.pcreport.buttons[0] =
	data.pcreport.buttons[1] = 0;
	data.pcreport.hatswitch  = 0b00001111;
	data.pcreport.joystick_axes = 0b01010101;
}

#define PC_LK		{ data.pcreport.buttons[0] |= (1<<0); }
#define PC_MK		{ data.pcreport.buttons[0] |= (1<<1); }
#define PC_HK		{ data.pcreport.buttons[0] |= (1<<2); }
#define PC_4K		{ data.pcreport.buttons[0] |= (1<<3); }
#define PC_LP		{ data.pcreport.buttons[0] |= (1<<4); }
#define PC_MP		{ data.pcreport.buttons[0] |= (1<<5); }
#define PC_HP		{ data.pcreport.buttons[0] |= (1<<6); }
#define PC_4P		{ data.pcreport.buttons[0] |= (1<<7); }

#define PC_S3		{ data.pcreport.buttons[1] |= (1<<0); }
#define PC_S4		{ data.pcreport.buttons[1] |= (1<<1); }
#define PC_START	{ data.pcreport.buttons[1] |= (1<<2); }
#define PC_SELECT	{ data.pcreport.buttons[1] |= (1<<3); }
#define PC_HOME		{ data.pcreport.buttons[1] |= (1<<4); }

#define PC_DPAD_UP			{ data.pcreport.hatswitch = 0; }
#define PC_DPAD_UP_RIGHT	{ data.pcreport.hatswitch = 1; }
#define PC_DPAD_RIGHT		{ data.pcreport.hatswitch = 2; }
#define PC_DPAD_DOWN_RIGHT	{ data.pcreport.hatswitch = 3; }
#define PC_DPAD_DOWN		{ data.pcreport.hatswitch = 4; }
#define PC_DPAD_DOWN_LEFT	{ data.pcreport.hatswitch = 5; }
#define PC_DPAD_LEFT		{ data.pcreport.hatswitch = 6; }
#define PC_DPAD_UP_LEFT		{ data.pcreport.hatswitch = 7; }

#define PC_LS_LEFT	{ data.pcreport.joystick_axes &= 0b11111100; }
#define PC_LS_RIGHT	{ data.pcreport.joystick_axes |= 0b00000010; data.pcreport.joystick_axes &= 0b11111110; }

#define PC_LS_UP	{ data.pcreport.joystick_axes &= 0b11110011; }
#define PC_LS_DOWN	{ data.pcreport.joystick_axes |= 0b00001000; data.pcreport.joystick_axes &= 0b11111011; }

#define PC_RS_LEFT	{ data.pcreport.joystick_axes &= 0b11001111; }
#define PC_RS_RIGHT	{ data.pcreport.joystick_axes |= 0b00100000; data.pcreport.joystick_axes &= 0b11101111; }

#define PC_RS_UP	{ data.pcreport.joystick_axes &= 0b00111111; }
#define PC_RS_DOWN	{ data.pcreport.joystick_axes |= 0b10000000; data.pcreport.joystick_axes &= 0b10111111; }

void readInputPC() {
	resetPCReportBuffer();
	updateStickState();	

	if(CFG_JOYSTICK_SWITCH_READ || !metaPressed) {
		// Left Joystick Directions
		if(CFG_LEFT_STICK) {
			if(STICK_STATE_SIGNAL(stickState, STICK_STATE_UP))
				PC_LS_UP
			
			if(STICK_STATE_SIGNAL(stickState, STICK_STATE_DOWN))
				PC_LS_DOWN

			if(STICK_STATE_SIGNAL(stickState, STICK_STATE_LEFT))
				PC_LS_LEFT
			
			if(STICK_STATE_SIGNAL(stickState, STICK_STATE_RIGHT))
				PC_LS_RIGHT
		}

		// Right Joystick Directions
		if(CFG_RIGHT_STICK) {
			if(STICK_STATE_SIGNAL(stickState, STICK_STATE_UP))
				PC_RS_UP
			
			if(STICK_STATE_SIGNAL(stickState, STICK_STATE_DOWN))
				PC_RS_DOWN
		
			if(STICK_STATE_SIGNAL(stickState, STICK_STATE_LEFT))
				PC_RS_LEFT
			
			if(STICK_STATE_SIGNAL(stickState, STICK_STATE_RIGHT))
				PC_RS_RIGHT
		}

		// Digital Pad Directions
		if(CFG_DIGITAL_PAD) {
			if(STICK_STATE_SIGNAL(stickState, STICK_STATE_UP)) {
				if(STICK_STATE_SIGNAL(stickState, STICK_STATE_LEFT)) {
					PC_DPAD_UP_LEFT
				}
				else if(STICK_STATE_SIGNAL(stickState, STICK_STATE_RIGHT)) {
					PC_DPAD_UP_RIGHT
				}
				else {
					PC_DPAD_UP
				}
			}
			else if(STICK_STATE_SIGNAL(stickState, STICK_STATE_DOWN)) {
				if(STICK_STATE_SIGNAL(stickState, STICK_STATE_LEFT)) {
					PC_DPAD_DOWN_LEFT
				}
				else if(STICK_STATE_SIGNAL(stickState, STICK_STATE_RIGHT)) {
					PC_DPAD_DOWN_RIGHT
				}
				else {
					PC_DPAD_DOWN
				}
			}
			else if(STICK_STATE_SIGNAL(stickState, STICK_STATE_LEFT)) {
				PC_DPAD_LEFT
			}
			else if(STICK_STATE_SIGNAL(stickState, STICK_STATE_RIGHT)) {
				PC_DPAD_RIGHT
			}
		}
	}

	// Buttons
	if(!Stick_LP)
		PC_LP

	if(!Stick_MP)
		PC_MP

	if(!Stick_HP)
		PC_HP

	if(!Stick_LK)
		PC_LK

	if(!Stick_MK)
		PC_MK

	if(!Stick_HK)
		PC_HK

#ifdef EXTRA_BUTTONS			
	if(!Stick_4P)
		PC_4P

	if(!Stick_4K)
		PC_4K
#endif

	if(CFG_X3_READ) {
		if(!Stick_S3)
			PC_S3

		if(!Stick_S4)
			PC_S4
	}

	if(CFG_HOME_EMU && !Stick_Start && !Stick_Select) {
		PC_HOME
		metaWasUsed = 1;
	}
	else {
		if(CFG_META_BUTTON_START) {
			if(metaSendCount > 0)
				PC_START

			if(!Stick_Select)
				PC_SELECT
		}
		else {
			if(!Stick_Start)
				PC_START

			if(metaSendCount > 0)
				PC_SELECT
		}
	}

	if(!Stick_Home)
		PC_HOME
}

/* ------------------------------------------------------------------------- */

void pc_init_controller() {
	usbMode = USB_MODE_PC;
	resetPCReportBuffer();
	setupUSB();
}

void pc_test_controller() {
	usbSetInterrupt(data.array, 3);
	usbPoll();

	if(usbInterruptIsReady())
		detected = 2;
}

void pc_controller() {
    while(1) { /* main event loop */
		usbPoll();
		updateMetaState();
		updateJoystickMode();
        readInputPC();
		sendDataUSB(data.array, 4);
    }
}
