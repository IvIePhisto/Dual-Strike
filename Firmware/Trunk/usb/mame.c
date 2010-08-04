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
#define MAME_R1_ENTER	{ data.mame_reports.report1.reportData[0] |= (1<<7); }
#define MAME_R1_ESCAPE	{ data.mame_reports.report1.reportData[1] |= (1<<0); }
#define MAME_R1_P		{ data.mame_reports.report1.reportData[1] |= (1<<2); }
#define MAME_R1_1		{ data.mame_reports.report1.reportData[1] |= (1<<5); }
#define MAME_R1_5		{ data.mame_reports.report1.reportData[1] |= (1<<6); }

#define MAME_R1_LCTRL_ARRAY 	0
#define MAME_R1_LALT_ARRAY		0
#define MAME_R1_LSHIFT_ARRAY	0

#define MAME_R1_LCTRL_BIT		4
#define MAME_R1_LALT_BIT		5
#define MAME_R1_LSHIFT_BIT		6

#define MAME_R1_SPACE_ARRAY		1
#define MAME_R1_X_ARRAY			1
#define MAME_R1_Y_ARRAY			1

#define MAME_R1_SPACE_BIT		1
#define MAME_R1_X_BIT			3
#define MAME_R1_Y_BIT			4



#define MAME_R2_D		{ data.mame_reports.report2.reportData[0] |= (1<<1); }
#define MAME_R2_F		{ data.mame_reports.report2.reportData[0] |= (1<<2); }
#define MAME_R2_G		{ data.mame_reports.report2.reportData[0] |= (1<<3); }
#define MAME_R2_P		{ data.mame_reports.report2.reportData[0] |= (1<<6); }
#define MAME_R2_R		{ data.mame_reports.report2.reportData[1] |= (1<<0); }
#define MAME_R2_2		{ data.mame_reports.report2.reportData[1] |= (1<<3); }
#define MAME_R2_6		{ data.mame_reports.report2.reportData[1] |= (1<<4); }

#define MAME_R2_A_ARRAY	0
#define MAME_R2_I_ARRAY	0
#define MAME_R2_K_ARRAY	0
#define MAME_R2_Q_ARRAY	0

#define MAME_R2_A_BIT	0
#define MAME_R2_I_BIT	4
#define MAME_R2_K_BIT	5
#define MAME_R2_Q_BIT	7

#define MAME_R2_S_ARRAY	1
#define MAME_R2_W_ARRAY	1

#define MAME_R2_S_BIT	1
#define MAME_R2_W_BIT	2


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


#define MAME_SET_REPORT_BIT_FUNCTION(functionName, reportNo, array, bit) void functionName() { data.mame_reports.report##reportNo.reportData[array] |= (1<<bit); }
// meaning: LK, MK, HK, 4K, LP, MP, HP, 4P
int buttonMapping1[8] = {4, 5, 6, -1, 1, 2, 3, 4};
int buttonMapping2[8] = {-1, -1, -1, -1, -1, -1, -1, -1};
int buttonMapping3[8] = {-1, -1, -1, -1, -1, -1, -1, -1};
int buttonMapping4[8] = {-1, -1, -1, -1, -1, -1, -1, -1};

int* buttonMapping = buttonMapping1;

#define MAME_LK 0
#define MAME_MK 1
#define MAME_HK 2
#define MAME_4K 3
#define MAME_LP 4
#define MAME_MP 5
#define MAME_HP 6
#define MAME_4P 7

#define MAME_BUTTON_CFG(buttonLayout, stickButton, mameButton) CFG_MAME_BL##buttonLayout##_##stickButton##_##mameButton

#define MAME_LOAD_BUTTON_MAPPING(buttonLayout, stickButton) \
	if(CFG_MAME_BL##buttonLayout##_##stickButton##_NOTHING)\
		buttonMapping##buttonLayout[MAME_##stickButton] = -1;\
	else if(MAME_BUTTON_CFG(buttonLayout, stickButton, BUTTON1))\
		buttonMapping##buttonLayout[MAME_##stickButton] = 0;\
	else if(MAME_BUTTON_CFG(buttonLayout, stickButton, BUTTON2))\
		buttonMapping##buttonLayout[MAME_##stickButton] = 1;\
	else if(MAME_BUTTON_CFG(buttonLayout, stickButton, BUTTON3))\
		buttonMapping##buttonLayout[MAME_##stickButton] = 2;\
	else if(MAME_BUTTON_CFG(buttonLayout, stickButton, BUTTON4))\
		buttonMapping##buttonLayout[MAME_##stickButton] = 3;\
	else if(MAME_BUTTON_CFG(buttonLayout, stickButton, BUTTON5))\
		buttonMapping##buttonLayout[MAME_##stickButton] = 4;\
	else if(MAME_BUTTON_CFG(buttonLayout, stickButton, BUTTON6))\
		buttonMapping##buttonLayout[MAME_##stickButton] = 5;\

#define MAME_LOAD_BUTTON_MAPPINGS(buttonLayout)\
	MAME_LOAD_BUTTON_MAPPING(buttonLayout, LK)\
	MAME_LOAD_BUTTON_MAPPING(buttonLayout, MK)\
	MAME_LOAD_BUTTON_MAPPING(buttonLayout, HK)\
	MAME_LOAD_BUTTON_MAPPING(buttonLayout, 4K)\
	MAME_LOAD_BUTTON_MAPPING(buttonLayout, LP)\
	MAME_LOAD_BUTTON_MAPPING(buttonLayout, MP)\
	MAME_LOAD_BUTTON_MAPPING(buttonLayout, HP)\
	MAME_LOAD_BUTTON_MAPPING(buttonLayout, 4P)\

void initMAMEButtonMappings() {
	MAME_LOAD_BUTTON_MAPPINGS(1)
	MAME_LOAD_BUTTON_MAPPINGS(2)
	MAME_LOAD_BUTTON_MAPPINGS(3)
	MAME_LOAD_BUTTON_MAPPINGS(4)
}

MAME_SET_REPORT_BIT_FUNCTION(mamePlayer1Button1, 1, MAME_R1_LCTRL_ARRAY, MAME_R1_LCTRL_BIT)
MAME_SET_REPORT_BIT_FUNCTION(mamePlayer1Button2, 1, MAME_R1_LALT_ARRAY, MAME_R1_LALT_BIT)
MAME_SET_REPORT_BIT_FUNCTION(mamePlayer1Button3, 1, MAME_R1_SPACE_ARRAY, MAME_R1_SPACE_BIT)
MAME_SET_REPORT_BIT_FUNCTION(mamePlayer1Button4, 1, MAME_R1_LSHIFT_ARRAY, MAME_R1_LSHIFT_BIT)
MAME_SET_REPORT_BIT_FUNCTION(mamePlayer1Button5, 1, MAME_R1_X_ARRAY, MAME_R1_X_BIT)
MAME_SET_REPORT_BIT_FUNCTION(mamePlayer1Button6, 1, MAME_R1_Y_ARRAY, MAME_R1_Y_BIT)

void (*player1Buttons[6])() = {mamePlayer1Button1, mamePlayer1Button2, mamePlayer1Button3, mamePlayer1Button4, mamePlayer1Button5, mamePlayer1Button6 };

MAME_SET_REPORT_BIT_FUNCTION(mamePlayer2Button1, 2, MAME_R2_A_ARRAY, MAME_R2_A_BIT)
MAME_SET_REPORT_BIT_FUNCTION(mamePlayer2Button2, 2, MAME_R2_S_ARRAY, MAME_R2_S_BIT)
MAME_SET_REPORT_BIT_FUNCTION(mamePlayer2Button3, 2, MAME_R2_Q_ARRAY, MAME_R2_Q_BIT)
MAME_SET_REPORT_BIT_FUNCTION(mamePlayer2Button4, 2, MAME_R2_W_ARRAY, MAME_R2_W_BIT)
MAME_SET_REPORT_BIT_FUNCTION(mamePlayer2Button5, 2, MAME_R2_K_ARRAY, MAME_R2_K_BIT)
MAME_SET_REPORT_BIT_FUNCTION(mamePlayer2Button6, 2, MAME_R2_I_ARRAY, MAME_R2_I_BIT)

void (*player2Buttons[6])() = {mamePlayer2Button1, mamePlayer2Button2, mamePlayer2Button3, mamePlayer2Button4, mamePlayer2Button5, mamePlayer2Button6 };

#define MAME_SET_BUTTON(playerNo, buttonID) { if(buttonMapping[buttonID] >= 0) (*player##playerNo##Buttons[buttonMapping[buttonID]])(); }


uchar selectPressed;
uchar selectWasPressed = 0;
uchar selectWasUsed = 0;
uchar selectReleased = 0;
uchar mameControl = 0;
uchar mamePlayer = 1;

void sendMAMEReportPlayer1() {
	resetMAMEReport1();

	if(selectPressed) {
		if(!Stick_LK) {
			MAME_R1_ENTER
			selectWasUsed = 1;
		}

		if(!Stick_MK) {
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

		if(!Stick_LK)
			MAME_SET_BUTTON(1, MAME_LK)

		if(!Stick_MK)
			MAME_SET_BUTTON(1, MAME_MK)

		if(!Stick_HK)
			MAME_SET_BUTTON(1, MAME_HK)

		if(!Stick_LP)
			MAME_SET_BUTTON(1, MAME_LP)

		if(!Stick_MP)
			MAME_SET_BUTTON(1, MAME_MP)

		if(!Stick_HP)
			MAME_SET_BUTTON(1, MAME_HP)

	#ifdef EXTRA_BUTTONS
		if(!Stick_4K)
			MAME_SET_BUTTON(1, MAME_4K)

		if(!Stick_4P)
			MAME_SET_BUTTON(1, MAME_4P)
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

	if(!Stick_LK)
		MAME_SET_BUTTON(2, MAME_LK)

	if(!Stick_MK)
		MAME_SET_BUTTON(2, MAME_MK)

	if(!Stick_HK)
		MAME_SET_BUTTON(2, MAME_HK)

	if(!Stick_LP)
		MAME_SET_BUTTON(2, MAME_LP)

	if(!Stick_MP)
		MAME_SET_BUTTON(2, MAME_MP)

	if(!Stick_HP)
		MAME_SET_BUTTON(2, MAME_HP)

#ifdef EXTRA_BUTTONS
	if(!Stick_4K)
		MAME_SET_BUTTON(2, MAME_4K)

	if(!Stick_4P)
		MAME_SET_BUTTON(2, MAME_4P)
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

	if(!Stick_LP)
		MAME_R3_ENTER

	if(!Stick_MP)
		MAME_R3_ESCAPE

	if(!Stick_LK)
		MAME_R3_F2

	if(!Stick_MK)
		MAME_R3_F3

	if(!Stick_Start)
		MAME_R3_TAB

	if(selectReleased) {
		MAME_R3_9
		selectReleased = 0;
	}

#ifdef EXTRA_BUTTONS		
	if(!Stick_4P)
		MAME_R4_VOL_UP

	if(!Stick_4K)
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

		if(!Stick_LP) {
			if(!Stick_Up) {
				buttonMapping = buttonMapping1;
				selectWasUsed = 1;
			}
			else if(!Stick_Right) {
				buttonMapping = buttonMapping2;
				selectWasUsed = 1;
			}
			else if(!Stick_Down) {
				buttonMapping = buttonMapping3;
				selectWasUsed = 1;
			}
			else if(!Stick_Left) {
				buttonMapping = buttonMapping4;
				selectWasUsed = 1;
			}
		}
		else if(!Stick_Up) {
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
	initMAMEButtonMappings();

    while(1) { /* main event loop */
		// could be used for mode switching: readJoystickSwitch();
        sendMAMEReports();
    }
}
