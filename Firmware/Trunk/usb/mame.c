void resetMAMEReportBuffer() {
	data.mame_report.reportID =
	data.mame_report.reportData[0] =
	data.mame_report.reportData[1] = 0;
}

#define MAME_UP			{ data.mame_report.reportData[0] |= (1<<0); }
#define MAME_RIGHT		{ data.mame_report.reportData[0] |= (1<<1); }
#define MAME_DOWN		{ data.mame_report.reportData[0] |= (1<<2); }
#define MAME_LEFT		{ data.mame_report.reportData[0] |= (1<<3); }
#define MAME_LK			{ data.mame_report.reportData[0] |= (1<<4); }
#define MAME_MK			{ data.mame_report.reportData[0] |= (1<<5); }
#define MAME_HK			{ data.mame_report.reportData[0] |= (1<<6); }
#define MAME_4K			{ data.mame_report.reportData[0] |= (1<<7); }

#define MAME_LP			{ data.mame_report.reportData[1] |= (1<<0); }
#define MAME_MP			{ data.mame_report.reportData[1] |= (1<<1); }
#define MAME_HP			{ data.mame_report.reportData[1] |= (1<<2); }
#define MAME_4P			{ data.mame_report.reportData[1] |= (1<<3); }
#define MAME_SELECT		{ data.mame_report.reportData[1] |= (1<<4); }
#define MAME_START		{ data.mame_report.reportData[1] |= (1<<5); }
#define MAME_HOME		{ data.mame_report.reportData[1] |= (1<<6); }

#define MAME_REPORT_ID_PLAYER_1 1
#define MAME_REPORT_ID_PLAYER_2 2
#define MAME_REPORT_ID_CONTROL 3

uchar selectWasPressed = 0;
uchar selectWasUsed = 0;
uchar mameControl = 0;
uchar mamePlayer = 1;

void readInputMAME() {
	uchar selectPressed = 0;
	uchar selectReleased = 0;

	resetMAMEReportBuffer();

	selectPressed = !Stick_Select;

	if(selectPressed) {
		if(CFG_HOME_EMU && !Stick_Start) {
				MAME_HOME
				selectWasUsed = 1;
				selectWasPressed = 1;
		}

		if(selectPressed && !Stick_Up) {
			mameControl = 0;
			selectWasUsed = 1;
			selectWasPressed = 1;
		}
		else if(selectPressed && !Stick_Down) {
			mameControl = 1;
			selectWasUsed = 1;
			selectWasPressed = 1;
		}
	}

	if(!mameControl) {
		if(selectPressed && !Stick_Left) {
			mamePlayer = MAME_REPORT_ID_PLAYER_1;
			selectWasUsed = 1;
			selectWasPressed = 1;
		}
		else if(selectPressed && !Stick_Right) {
			mamePlayer = MAME_REPORT_ID_PLAYER_2;
			selectWasUsed = 1;
			selectWasPressed = 1;
		}
	}

	if(selectWasUsed) {
		if(!selectPressed) {
			selectWasUsed = 0;
			selectWasPressed = 0;
		}
	}
	else {
		if(selectPressed) {
			selectWasPressed = 1;
		}
		else if(selectWasPressed) {
			selectWasPressed = 0;
			selectReleased = 1;
		}
	}

	if(mameControl)
		data.mame_report.reportID = MAME_REPORT_ID_CONTROL;
	else
		data.mame_report.reportID = mamePlayer;
		
	if(!selectPressed) {
		if (!Stick_Up)
			MAME_UP
		else if (!Stick_Down)
			MAME_DOWN

		if (!Stick_Left)
			MAME_LEFT
		else if (!Stick_Right)
			MAME_RIGHT

		// Buttons
		if(!Stick_Jab)
			MAME_LP

		if(!Stick_Strong)
			MAME_MP

		if(!Stick_Fierce)
			MAME_HP

		if(!Stick_Short)
			MAME_LK

		if(!Stick_Forward)
			MAME_MK

		if(!Stick_Roundhouse)
			MAME_HK

#ifdef EXTRA_BUTTONS					
		if(!Stick_Extra0)
			MAME_4P

		if(!Stick_Extra1)
			MAME_4K
#endif

		if(!Stick_Start)
			MAME_START

		if(selectReleased) {
			MAME_SELECT
			selectReleased = 0;
		}

		if(!Stick_Home)
			MAME_HOME
	}
}


void sendVolumeControl() {
	if(mameControl) {		
		uchar pressed4P = data.mame_report.reportData[0] & (1<<7);
		uchar pressed4K = data.mame_report.reportData[1] & (1<<3);

		if(pressed4P || pressed4K) {
			resetMAMEReportBuffer();
			data.mame_report.reportID = 4;
		
			if(pressed4K)
				data.mame_report.reportData[0] |= (1<<0);

			if(pressed4P)
				data.mame_report.reportData[0] |= (1<<1);		
			
			sendDataUSB(data.array, 2);
		}
		
	}
}

void mame_controller() {
	usbMode = USB_MODE_MAME;
	setupUSB();
	usbPoll();
	resetMAMEReportBuffer();
	data.mame_report.reportID = MAME_REPORT_ID_CONTROL;
	sendDataUSB(data.array, 3);


    while(1) { /* main event loop */
		usbPoll();
		// could be used for mode switching: readJoystickSwitch();
        readInputMAME();
		sendDataUSB(data.array, 3);
		sendVolumeControl();
    }
}
