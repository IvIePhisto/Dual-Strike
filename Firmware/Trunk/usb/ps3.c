void resetPS3ReportBuffer() {
	data.ps3report.buttons[0] =
	data.ps3report.buttons[1] = 0;
	data.ps3report.hatswitch  = 0b00001111;
	data.ps3report.joystick_axes = 0b01010101;
	data.ps3report.analogue_buttons[0] =
	data.ps3report.analogue_buttons[1] =
	data.ps3report.analogue_buttons[2] =
	data.ps3report.analogue_buttons[3] =
	data.ps3report.analogue_buttons[4] =
	data.ps3report.analogue_buttons[5] =
	data.ps3report.analogue_buttons[6] =
	data.ps3report.analogue_buttons[7] =
	data.ps3report.analogue_buttons[8] =
	data.ps3report.analogue_buttons[9] =
	data.ps3report.analogue_buttons[10] =
	data.ps3report.analogue_buttons[11] = 0;
}

#define PS3_SQUARE		{ data.ps3report.buttons[0] |= (1<<0); data.ps3report.analogue_buttons[7] = 0xFF; }
#define PS3_CROSS		{ data.ps3report.buttons[0] |= (1<<1); data.ps3report.analogue_buttons[6] = 0xFF; }
#define PS3_CIRCLE		{ data.ps3report.buttons[0] |= (1<<2); data.ps3report.analogue_buttons[5] = 0xFF; }
#define PS3_TRIANGLE	{ data.ps3report.buttons[0] |= (1<<3); data.ps3report.analogue_buttons[4] = 0xFF; }
#define PS3_L1			{ data.ps3report.buttons[0] |= (1<<4); data.ps3report.analogue_buttons[8] = 0xFF; }
#define PS3_R1			{ data.ps3report.buttons[0] |= (1<<5); data.ps3report.analogue_buttons[9] = 0xFF; }
#define PS3_L2			{ data.ps3report.buttons[0] |= (1<<6); data.ps3report.analogue_buttons[10] = 0xFF; }
#define PS3_R2			{ data.ps3report.buttons[0] |= (1<<7); data.ps3report.analogue_buttons[11] = 0xFF; }

#define PS3_SELECT		{ data.ps3report.buttons[1] |= (1<<0); }
#define PS3_START		{ data.ps3report.buttons[1] |= (1<<1); }
#define PS3_PS			{ data.ps3report.buttons[1] |= (1<<4); }

#define PS3_DPAD_UP			{ data.ps3report.hatswitch = 0; data.ps3report.analogue_buttons[2] = 0xFF; }
#define PS3_DPAD_UP_RIGHT	{ data.ps3report.hatswitch = 1; data.ps3report.analogue_buttons[2] = 0xFF; data.ps3report.analogue_buttons[0] = 0xFF; }
#define PS3_DPAD_RIGHT		{ data.ps3report.hatswitch = 2; 								   data.ps3report.analogue_buttons[0] = 0xFF; }
#define PS3_DPAD_DOWN_RIGHT	{ data.ps3report.hatswitch = 3; data.ps3report.analogue_buttons[3] = 0xFF; data.ps3report.analogue_buttons[0] = 0xFF; }
#define PS3_DPAD_DOWN		{ data.ps3report.hatswitch = 4; data.ps3report.analogue_buttons[3] = 0xFF; }
#define PS3_DPAD_DOWN_LEFT	{ data.ps3report.hatswitch = 5; data.ps3report.analogue_buttons[3] = 0xFF; data.ps3report.analogue_buttons[1] = 0xFF; }
#define PS3_DPAD_LEFT		{ data.ps3report.hatswitch = 6; 								   data.ps3report.analogue_buttons[1] = 0xFF; }
#define PS3_DPAD_UP_LEFT	{ data.ps3report.hatswitch = 7; data.ps3report.analogue_buttons[2] = 0xFF; data.ps3report.analogue_buttons[1] = 0xFF; }

#define PS3_LS_LEFT		{ data.ps3report.joystick_axes &= 0b11111100; }
#define PS3_LS_RIGHT	{ data.ps3report.joystick_axes |= 0b00000010; data.ps3report.joystick_axes &= 0b11111110; }

#define PS3_LS_UP		{ data.ps3report.joystick_axes &= 0b11110011; }
#define PS3_LS_DOWN		{ data.ps3report.joystick_axes |= 0b00001000; data.ps3report.joystick_axes &= 0b11111011; }

#define PS3_RS_LEFT		{ data.ps3report.joystick_axes &= 0b11001111; }
#define PS3_RS_RIGHT	{ data.ps3report.joystick_axes |= 0b00100000; data.ps3report.joystick_axes &= 0b11101111; }

#define PS3_RS_UP		{ data.ps3report.joystick_axes &= 0b00111111; }
#define PS3_RS_DOWN		{ data.ps3report.joystick_axes |= 0b10000000; data.ps3report.joystick_axes &= 0b10111111; }

void readInputPS3() {
	resetPS3ReportBuffer();

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
	if(!Stick_LP)
		PS3_SQUARE

	if(!Stick_MP)
		PS3_TRIANGLE

	if(!Stick_HP)
		PS3_R1

	if(!Stick_LK)
		PS3_CROSS

	if(!Stick_MK)
		PS3_CIRCLE

	if(!Stick_HK)
		PS3_R2

#ifdef EXTRA_BUTTONS					
	if(!Stick_4P)
		PS3_L1

	if(!Stick_4K)
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


void ps3_controller() {
	usbMode = USB_MODE_PS3;
	setupUSB();
	resetPS3ReportBuffer();
	sendDataUSB(data.array, 16);

    while(1) { /* main event loop */
		usbPoll();
		readJoystickSwitch();
        readInputPS3();
		sendDataUSB(data.array, 16);
    }
}
