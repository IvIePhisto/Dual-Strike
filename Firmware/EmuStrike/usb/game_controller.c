// see <http://romanblack.com/trackball.htm> for trackball protocoll example

#define ATMEGA_NO 8

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
	trackballX =
	trackballY = 0;

	// reset timer
	TCNT0 = 0;

 	// reset overflow flag
#if ATMEGA_NO == 168
	TIFR0 |= (1<<TOV0);
#else
	TIFR |= (1<<TOV0);
#endif

	x_1 = Stick_Up?1:0;
	x_2 = Stick_Down?1:0;
	y_1 = Stick_Left?1:0;
	y_2 = Stick_Right?1:0;

	while(!(
 // overflow
#if ATMEGA_NO == 168
		TIFR0 & (1<<TOV0)
#else
		TIFR & (1<<TOV0)
#endif
	)) {

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
				if(trackballX > -127)
					trackballX--;
			}
			else {
				if(trackballX < 127)
					trackballX++;
			}
		}

		if((y_1 != last_y_1) || (y_2 != last_y_2)) {
			if(y_2 != last_y_1) {
				if(trackballY < 127)
					trackballY++;
			}
			else {
				if(trackballY > -127)
					trackballY--;
			}
		}
	}

	data.game_report.trackball_x = trackballX;
	data.game_report.trackball_y = trackballY;
}

/* ------------------------------------------------------------------------- */

void game_controller() {
	usbMode = USB_MODE_GAME;
	setupUSB();
	usbPoll();
	data.game_report.trackball_x =
	data.game_report.trackball_y = 0;
	usbSetInterrupt(data.array, 2);

 // start timer with a clock-prescaling of 1024
#define TIMER_BITMASK (1<<CS00) | (1<<CS02)
#if ATMEGA_NO == 168
	TCCR0B = TIMER_BITMASK;
#else
	TCCR0 = TIMER_BITMASK;
#endif

    while(1) { /* main event loop */
		if(!usbInterruptIsReady())
			usbPoll();

        readInputGame();
		usbSetInterrupt(data.array, 2);
    }
}

