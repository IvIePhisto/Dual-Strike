void resetPCReportBuffer() {
	data.pcreport.buttons[0] =
	data.pcreport.buttons[1] = 0;
	data.pcreport.hatswitch  = 0b00001111;
}

#define PC_LK		{ data.ps3report.buttons[0] |= (1<<0); }
#define PC_MK		{ data.ps3report.buttons[0] |= (1<<1); }
#define PC_HK		{ data.ps3report.buttons[0] |= (1<<2); }
#define PC_4K		{ data.ps3report.buttons[0] |= (1<<3); }
#define PC_LP		{ data.ps3report.buttons[0] |= (1<<4); }
#define PC_MP		{ data.ps3report.buttons[0] |= (1<<5); }
#define PC_HP		{ data.ps3report.buttons[0] |= (1<<6); }
#define PC_4P		{ data.ps3report.buttons[0] |= (1<<7); }

#define PC_S3		{ data.ps3report.buttons[1] |= (1<<0); }
#define PC_S4		{ data.ps3report.buttons[1] |= (1<<1); }
#define PC_START	{ data.ps3report.buttons[1] |= (1<<2); }
#define PC_SELECT	{ data.ps3report.buttons[1] |= (1<<3); }
#define PC_HOME		{ data.ps3report.buttons[1] |= (1<<4); }

#define PC_UP			{ data.ps3report.hatswitch = 0; }
#define PC_UP_RIGHT		{ data.ps3report.hatswitch = 1; }
#define PC_RIGHT		{ data.ps3report.hatswitch = 2; }
#define PC_DOWN_RIGHT	{ data.ps3report.hatswitch = 3; }
#define PC_DOWN			{ data.ps3report.hatswitch = 4; }
#define PC_DOWN_LEFT	{ data.ps3report.hatswitch = 5; }
#define PC_LEFT			{ data.ps3report.hatswitch = 6; }
#define PC_UP_LEFT		{ data.ps3report.hatswitch = 7; }

void readInputPC() {
	resetPCReportBuffer();

	if (!Stick_Up) {
		if (!Stick_Left)
			PC_UP_LEFT
		else if (!Stick_Right)
			PC_UP_RIGHT
		else
			PC_UP
	}
	else if (!Stick_Down) {
		if (!Stick_Left)
			PC_DOWN_LEFT
		else if (!Stick_Right)
			PC_DOWN_RIGHT
		else
			PC_DOWN
	}
	else if (!Stick_Left)
		PC_LEFT
	else if (!Stick_Right)
		PC_RIGHT

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

	if(!Stick_S3)
		PC_S3

	if(!Stick_S4)
		PC_S4

	if(!Stick_Start)
		PC_START

	if(!Stick_Select)
		PC_SELECT

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
        readInputPC();
		sendDataUSB(data.array, 3);
    }
}
