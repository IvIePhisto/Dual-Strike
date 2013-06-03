// see <http://romanblack.com/trackball.htm> for trackball protocoll example

#define ATMEGA_NO 8

unsigned char last_x_1 = 0;
unsigned char last_x_2 = 0;
unsigned char last_y_1 = 0;
unsigned char last_y_2 = 0;
unsigned char x_1 = 0;
unsigned char x_2 = 0;
unsigned char y_1 = 0;
unsigned char y_2 = 0;

void resetInputKeyboard() {
	data.game_report.keys[0] =
	data.game_report.keys[1] =
	data.game_report.keys[2] = 0;
}

void resetInputTrackball() {
	data.game_report.trackballButtons =
	data.game_report.trackballX =
	data.game_report.trackballY = 0;
}

void resetRotaryValues() {
	last_x_1 = x_1 = Stick_Up?1:0;
	last_x_2 = x_2 = Stick_Down?1:0;
	last_x_2 = y_1 = Stick_Left?1:0;
	last_x_2 = y_2 = Stick_Right?1:0;
}

void readRotaryValues() {
	last_x_1 = x_1;
	last_x_2 = x_2;
	last_y_1 = y_1;
	last_y_2 = y_2;

	x_1 = Stick_Up?1:0;
	x_2 = Stick_Down?1:0;
	y_1 = Stick_Left?1:0;
	y_2 = Stick_Right?1:0;

	if((x_1 != last_x_1) || (x_2 != last_x_2)) {
		if(x_2 != last_x_1) {
			if(data.game_report.trackballX > -127) {
				data.game_report.trackballX--;
			}
		}
		else {
			if(data.game_report.trackballX < 127) {
				data.game_report.trackballX++;
			}
		}
	}

	if((y_1 != last_y_1) || (y_2 != last_y_2)) {
		if(y_2 != last_y_1) {
			if(data.game_report.trackballY < 127) {
				data.game_report.trackballY++;
			}
		}
		else {
			if(data.game_report.trackballY > -127) {
				data.game_report.trackballY--;
			}
		}
	}
}

void readInputGameTrackball() {
	unsigned char timerCounter = 0;

	resetRotaryValues();

	// reset timer
	TCNT0 = 0;

	while(timerCounter < 5) {
		if(
 		// overflow
#if ATMEGA_NO == 168
		TIFR0 & (1<<TOV0)
#else
		TIFR & (1<<TOV0)
#endif
		) {
			// reset overflow flag
#if ATMEGA_NO == 168
			TIFR0 |= (1<<TOV0);
#else
			TIFR |= (1<<TOV0);
#endif
			timerCounter++;
		}

		readRotaryValues();
	}

	if(!Stick_Start)
		data.game_report.keys[0] |= (1<<7);

	if(!Stick_Select)
		data.game_report.keys[1] |= (1<<0);
}

void readInputGameKeyboard() {
	resetInputKeyboard();

	if(!Stick_Start)
		data.game_report.keys[0] |= (1<<7);

	if(!Stick_Select)
		data.game_report.keys[1] |= (1<<0);
}

/* ------------------------------------------------------------------------- */

void game_controller() {
	usbMode = USB_MODE_GAME;
	setupUSB();
	usbPoll();
	data.game_report.keyboardReportID = 1;
	data.game_report.trackballReportID = 2;

#define TIMER_PRESCALING_1 (1<<CS00)
#define TIMER_PRESCALING_8 (1<<CS01)
#define TIMER_PRESCALING_64 ((1<<CS00) | (1<<CS01))
#define TIMER_PRESCALING_256 (1<<CS02)
#define TIMER_PRESCALING_1024 ((1<<CS00) | (1<<CS02))

#define TIMER_BITMASK TIMER_PRESCALING_8

// start timer
#if ATMEGA_NO == 168
	TCCR0B = TIMER_BITMASK;
#else
	TCCR0 = TIMER_BITMASK;
#endif

    while(1) { /* main event loop */
		readInputGameKeyboard();
		readInputGameTrackball();
		usbSetInterrupt(&data.game_report.keyboardReportID, 4);

		while(!usbInterruptIsReady()) {
			usbPoll();
			readInputGameTrackball();
		}

		usbSetInterrupt(&data.game_report.trackballReportID, 4);
		resetInputTrackball();

		while(!usbInterruptIsReady()) {
			usbPoll();
			readInputGameTrackball();
		}
    }
}

