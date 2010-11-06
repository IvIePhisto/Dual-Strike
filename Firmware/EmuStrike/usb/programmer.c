#include <avr/wdt.h>

void programmer_setup() {
	usbMode = USB_MODE_PROGRAMMER;
	setupUSB();
}

void programmer_poll() {
	//wdt_reset();
	usbPoll();
}
