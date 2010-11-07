#include "EmuStrike.h"

#include <avr/eeprom.h> /* EEPROM functions */
#include <avr/pgmspace.h>   /* required by usbdrv.h */
#include <util/delay.h>     /* for _delay_ms() */

#define WORKING_MODE_PT		0
#define WORKING_MODE_GAME	1

#define enableUsbLines()	S1_PORT |=  (1<<S1_PIN);
#define disableUsbLines()	S1_PORT &= ~(1<<S1_PIN);

int setModeGame() {
	enableUsbLines();

	return WORKING_MODE_GAME;
}

int setModePT() {
	disconnectUSB();
	disableUsbLines();

	S2_PORT |= (1<<S2_PIN); // enable pass-through usb lines

	return WORKING_MODE_PT;
}

int hardwareInit() {
	DDRC	= 0b00000000;	// PINC inputs
	PORTC	= 0b00111111;	// PORTC with pull-ups except S4

	DDRB	= 0b00000000;	// PINB inputs
	PORTB	= 0b00111111;	// PORTB with pull-ups except clock
	

	DDRD	= 0b00001001;  // PIND inputs, except S1 and S2
	PORTD	= 0b11100000;  // PORTD with pull-ups except S1, S2, D+, D- and S3

	if(!Stick_Select) {
		while(Stick_Start) {
			enableUsbLines();
			programmer_setup();

			while(Stick_Start)
				programmer_poll();

			disableUsbLines();
		}
	}

	if(!Stick_LK
	|| !Stick_MK
	|| !Stick_LP
	|| !Stick_MP
	|| !Stick_HP
	|| !Stick_4P)
		return setModePT();
	else
		return setModeGame();
}

/* ------------------------------------------------------------------------- */

/* buffer for data */
/*
NOTE:
As V-USB conforms to the USB low speed standard, the maximum packet size is 8 bytes for one call of usbSetInterrupt()!
But it is possible to send longer reports by using
---
while(!usbInterruptIsReady()) usbPoll();
---
to wait until the data is sent, then you can send the next part like this:
---
usbSetInterrupt((uchar *)&data + 8, 1*sizeof(uchar));
---
*/
unsigned char* data[132];

int main(void) {
	switch(hardwareInit()) {
	case WORKING_MODE_PT:
	  pass_through();
	  break;

	case WORKING_MODE_GAME:
	  game_controller();
	  break;
	}

    return 0;
}
