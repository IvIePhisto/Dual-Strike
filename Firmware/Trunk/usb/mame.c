void initMAMEReports() {
	data.mame_reports.report1.reportID = 1;
	data.mame_reports.report2.reportID = 2;
	data.mame_reports.report3.reportID = 3;
	data.mame_reports.report4.reportID = 4;
}

void resetMAMEReport1() {
	data.mame_reports.report1.reportData[0] =
	data.mame_reports.report1.reportData[1] = 0;
}

void resetMAMEReport2() {
	data.mame_reports.report2.reportData[0] =
	data.mame_reports.report2.reportData[1] = 0;
}

void resetMAMEReport3() {
	data.mame_reports.report3.reportData[0] =
	data.mame_reports.report3.reportData[1] = 0;
}

void resetMAMEReport4() {
	data.mame_reports.report4.reportData = 0;
}

#define MAME_R1_UP		{ data.mame_reports.report1.reportData[0] |= (1<<0); }
#define MAME_R1_RIGHT	{ data.mame_reports.report1.reportData[0] |= (1<<1); }
#define MAME_R1_DOWN	{ data.mame_reports.report1.reportData[0] |= (1<<2); }
#define MAME_R1_LEFT	{ data.mame_reports.report1.reportData[0] |= (1<<3); }
#define MAME_R1_LCTRL	{ data.mame_reports.report1.reportData[0] |= (1<<4); }
#define MAME_R1_LALT	{ data.mame_reports.report1.reportData[0] |= (1<<5); }
#define MAME_R1_LSHIFT	{ data.mame_reports.report1.reportData[0] |= (1<<6); }
#define MAME_R1_ENTER	{ data.mame_reports.report1.reportData[0] |= (1<<7); }

#define MAME_R1_ESCAPE	{ data.mame_reports.report1.reportData[1] |= (1<<0); }
#define MAME_R1_SPACE	{ data.mame_reports.report1.reportData[1] |= (1<<1); }
#define MAME_R1_P		{ data.mame_reports.report1.reportData[1] |= (1<<2); }
#define MAME_R1_X		{ data.mame_reports.report1.reportData[1] |= (1<<3); }
#define MAME_R1_Y		{ data.mame_reports.report1.reportData[1] |= (1<<4); }
#define MAME_R1_1		{ data.mame_reports.report1.reportData[1] |= (1<<5); }
#define MAME_R1_5		{ data.mame_reports.report1.reportData[1] |= (1<<6); }


#define MAME_R2_A		{ data.mame_reports.report2.reportData[0] |= (1<<0); }
#define MAME_R2_D		{ data.mame_reports.report2.reportData[0] |= (1<<1); }
#define MAME_R2_F		{ data.mame_reports.report2.reportData[0] |= (1<<2); }
#define MAME_R2_G		{ data.mame_reports.report2.reportData[0] |= (1<<3); }
#define MAME_R2_I		{ data.mame_reports.report2.reportData[0] |= (1<<4); }
#define MAME_R2_K		{ data.mame_reports.report2.reportData[0] |= (1<<5); }
#define MAME_R2_P		{ data.mame_reports.report2.reportData[0] |= (1<<6); }

#define MAME_R2_Q		{ data.mame_reports.report2.reportData[0] |= (1<<7); }
#define MAME_R2_R		{ data.mame_reports.report2.reportData[1] |= (1<<0); }
#define MAME_R2_S		{ data.mame_reports.report2.reportData[1] |= (1<<1); }
#define MAME_R2_W		{ data.mame_reports.report2.reportData[1] |= (1<<2); }
#define MAME_R2_2		{ data.mame_reports.report2.reportData[1] |= (1<<3); }
#define MAME_R2_6		{ data.mame_reports.report2.reportData[1] |= (1<<4); }


#define MAME_R3_UP		{ data.mame_reports.report3.reportData[0] |= (1<<0); }
#define MAME_R3_RIGHT	{ data.mame_reports.report3.reportData[0] |= (1<<1); }
#define MAME_R3_DOWN	{ data.mame_reports.report3.reportData[0] |= (1<<2); }
#define MAME_R3_LEFT	{ data.mame_reports.report3.reportData[0] |= (1<<3); }
#define MAME_R3_ENTER	{ data.mame_reports.report3.reportData[0] |= (1<<4); }
#define MAME_R3_ESCAPE	{ data.mame_reports.report3.reportData[0] |= (1<<5); }
#define MAME_R3_TAB		{ data.mame_reports.report3.reportData[0] |= (1<<6); }
#define MAME_R3_F2		{ data.mame_reports.report3.reportData[0] |= (1<<7); }

#define MAME_R3_F3		{ data.mame_reports.report3.reportData[1] |= (1<<0); }
#define MAME_R3_9		{ data.mame_reports.report3.reportData[1] |= (1<<1); }


#define MAME_R4_VOL_UP	 { data.mame_reports.report4.reportData |= (1<<0); }
#define MAME_R4_VOL_DOWN { data.mame_reports.report4.reportData |= (1<<1); }

uchar selectPressed;
uchar selectWasPressed = 0;
uchar selectWasUsed = 0;
uchar selectReleased = 0;
uchar mameControl = 0;
uchar mamePlayer = 1;

void sendMAMEReportPlayer1() {
	resetMAMEReport1();

	if(selectPressed) {
		if(!Stick_Short) {
			MAME_R1_ENTER
			selectWasUsed = 1;
		}

		if(!Stick_Forward) {
			MAME_R1_ESCAPE	
			selectWasUsed = 1;
		}

		if(CFG_HOME_EMU && !Stick_Start) {
			MAME_R1_P
			selectWasUsed = 1;
		}
	}
	else {
		if (!Stick_Up)
			MAME_R1_UP
		else if (!Stick_Down)
			MAME_R1_DOWN

		if (!Stick_Left)
			MAME_R1_LEFT
		else if (!Stick_Right)
			MAME_R1_RIGHT

		if(!Stick_Short)
			MAME_R1_LSHIFT

		if(!Stick_Forward)
			MAME_R1_X

		if(!Stick_Roundhouse)
			MAME_R1_Y

		if(!Stick_Jab)
			MAME_R1_LCTRL

		if(!Stick_Strong)
			MAME_R1_LALT

		if(!Stick_Fierce)
			MAME_R1_SPACE

	#ifdef EXTRA_BUTTONS					
		if(!Stick_Extra0)
			MAME_R1_LSHIFT
	#endif

		if(!Stick_Start)
			MAME_R1_1

		if(selectReleased) {
			MAME_R1_5
			selectReleased = 0;
		}

		if(!Stick_Home)
			MAME_R1_P
	}

	sendDataUSB((void*)&data.mame_reports.report1, 3);
}

void sendMAMEReportPlayer2() {
	resetMAMEReport2();

	if (!Stick_Up)
		MAME_R2_R
	else if (!Stick_Down)
		MAME_R2_F

	if (!Stick_Left)
		MAME_R2_D
	else if (!Stick_Right)
		MAME_R2_G

	if(!Stick_Short)
		MAME_R2_W

	if(!Stick_Forward)
		MAME_R2_K

	if(!Stick_Roundhouse)
		MAME_R2_I

	if(!Stick_Jab)
		MAME_R2_A

	if(!Stick_Strong)
		MAME_R2_S

	if(!Stick_Fierce)
		MAME_R2_Q

#ifdef EXTRA_BUTTONS					
	if(!Stick_Extra0)
		MAME_R2_W
#endif

	if(!Stick_Start)
		MAME_R2_2

	if(selectReleased) {
		MAME_R2_6
		selectReleased = 0;
	}

	if(!Stick_Home)
		MAME_R2_P

	if(CFG_HOME_EMU && selectPressed && !Stick_Start) {
		MAME_R2_P
		selectWasUsed = 1;
	}

	sendDataUSB((void*)&data.mame_reports.report2, 3);
}

void sendMAMEReportsControl() {
	resetMAMEReport3();
	resetMAMEReport4();

	if (!Stick_Up)
		MAME_R3_UP
	else if (!Stick_Down)
		MAME_R3_DOWN

	if (!Stick_Left)
		MAME_R3_LEFT
	else if (!Stick_Right)
		MAME_R3_RIGHT

	if(!Stick_Jab)
		MAME_R3_ENTER

	if(!Stick_Strong)
		MAME_R3_ESCAPE

	if(!Stick_Short)
		MAME_R3_F2

	if(!Stick_Forward)
		MAME_R3_F3

	if(!Stick_Start)
		MAME_R3_TAB

	if(selectReleased) {
		MAME_R3_9
		selectReleased = 0;
	}

#ifdef EXTRA_BUTTONS		
	if(!Stick_Extra0)
		MAME_R4_VOL_UP

	if(!Stick_Extra1)
		MAME_R4_VOL_DOWN
#endif

	sendDataUSB((void*)&data.mame_reports.report3, 3);
	sendDataUSB((void*)&data.mame_reports.report4, 2);
}

void sendMAMEReports() {	
	selectReleased = 0;
	selectPressed = !Stick_Select;

	if(selectPressed) {
		selectWasPressed = 1;

		if(!Stick_Up) {
			mameControl = 0;
			selectWasUsed = 1;
		}
		else if(!Stick_Down) {
			mameControl = 1;
			selectWasUsed = 1;
		}
	
		if(!mameControl) {
			if(!Stick_Left) {
				mamePlayer = 1;
				selectWasUsed = 1;
			}
			else if(!Stick_Right) {
				mamePlayer = 2;
				selectWasUsed = 1;
			}
		}
	}

	if(selectWasUsed) {
		if(!selectPressed) {
			selectWasUsed = 0;
			selectWasPressed = 0;
		}
	}
	else {
		if(!selectPressed && selectWasPressed) {
			selectWasPressed = 0;
			selectReleased = 1;
		}
	}

	if(mameControl) {
		if(!selectPressed)
			sendMAMEReportsControl();
	}
	else {
		if(mamePlayer == 1)
			sendMAMEReportPlayer1();
		else if(!selectPressed)
			sendMAMEReportPlayer2();
	}
}



void mame_controller() {
	usbMode = USB_MODE_MAME;
	setupUSB();
	usbPoll();
	initMAMEReports();

    while(1) { /* main event loop */
		// could be used for mode switching: readJoystickSwitch();
        sendMAMEReports();
    }
}
