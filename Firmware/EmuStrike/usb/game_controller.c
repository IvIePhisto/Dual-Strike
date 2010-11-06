void resetGameReportBuffer() {
	data.game_report.trackball_x =
	data.game_report.trackball_y = 0;
}

void readInputGame() {
	resetGameReportBuffer();

}

/* ------------------------------------------------------------------------- */


void game_controller() {
	usbMode = USB_MODE_GAME;
	setupUSB();
	usbPoll();
	resetGameReportBuffer();
	usbSetInterrupt(data.array, 2);

    while(1) { /* main event loop */
		while(!usbInterruptIsReady())
			usbPoll();
			
        readInputGame();
		usbSetInterrupt(data.array, 2);
    }
}
