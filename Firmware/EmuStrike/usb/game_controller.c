// see <http://romanblack.com/trackball.htm> for trackball protocoll example

char trackballX = 0;
char trackballY = 0;

unsigned char last_x_1 = 0;
unsigned char last_x_2 = 0;
unsigned char last_y_1 = 0;
unsigned char last_y_2 = 0;
unsigned char x_1 = 0;
unsigned char x_2 = 0;
unsigned char y_1 = 0;
unsigned char y_2 = 0;

void readInputGame() {
	last_x_1 = x_1;
	last_x_2 = x_2;
	last_y_1 = y_1;
	last_y_2 = y_2;

	x_1 = Stick_Up;
	x_2 = Stick_Down;
	y_1 = Stick_Left;
	y_2 = Stick_Right;

	/*
	if((x_1 != last_x_1) || (x_2 != last_x_2)) {
		if(x_2 != last_x_1) {

			//if(trackballX < 127)
				trackballX = 1;
		}
		else {

			//if(trackballX > -126)
				trackballX = -1;
		}
	}

	if((y_1 != last_y_1) || (y_2 != last_y_2)) {
		if(y_2 != last_y_1) {
			//if(trackballY < 127)
				trackballY = 1;
		}
		else {
			//if(trackballY > -126)
				trackballY = -1;
		}
	}
	*/

	if(!Stick_MK) {
		if(trackballY < 127)
			trackballY++;
	}
	else if(!Stick_MP) {
		if(trackballY > -126)
			trackballY--;
	}

	if(!Stick_LP) {
		if(trackballX < 127)
			trackballX++;
	}
	else if(!Stick_LK) {
		if(trackballX > -126)
			trackballX--;
	}
}

void updateReportGame() {
	data.game_report.trackball_x = trackballX;
	data.game_report.trackball_y = trackballY;
}

void resetReportGame() {
	trackballX =
	trackballY = 0;
}

/* ------------------------------------------------------------------------- */

void game_controller() {
	usbMode = USB_MODE_GAME;
	setupUSB();
	usbPoll();
	resetReportGame();
	usbSetInterrupt(data.array, 2);

    while(1) { /* main event loop */
		while(!usbInterruptIsReady()) {
			//readInputGame();
			usbPoll();
		}

        readInputGame();
		updateReportGame();
		usbSetInterrupt(data.array, 2);
		resetReportGame();
    }
}
