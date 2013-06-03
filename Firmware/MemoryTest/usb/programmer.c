#include <avr/wdt.h>

void programmer_setup() {
	setupUSB();
}

void programmer_poll() {
	//wdt_reset();
	usbPoll();
}
