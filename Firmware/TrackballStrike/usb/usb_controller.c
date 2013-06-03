#include "usb_controller.h"

#include "../TrackballStrike.h"

#include <avr/io.h>
#include <avr/interrupt.h>  /* for sei() */
#include <util/delay.h>     /* for _delay_ms() */
#include <avr/eeprom.h>

#include <avr/pgmspace.h>   /* required by usbdrv.h */
#include "usbdrv/usbdrv.h"

/* ------------------------------------------------------------------------- */

typedef struct {
	unsigned char buttons;
	char	trackballX;
	char	trackballY;
} trackball_controller_report_t;

typedef union {
	uchar array[132];
	trackball_controller_report_t trackball_report;
} usb_data_t;

extern usb_data_t data;

#define HID_REPORT_TYPE_INPUT 1
#define HID_REPORT_TYPE_OUTPUT 2
#define HID_REPORT_TYPE_FEATURE 3

#include "descriptors.c"

usbMsgLen_t usbFunctionSetup(uchar receivedData[8]) {
	/*
	usbRequest_t    *rq = (void *)receivedData;
	uchar reportID = rq->wValue.bytes[0];
	uchar reportType = rq->wValue.bytes[1];
	*/

    return 0;   /* default for not implemented requests: return no data back to host */
}

uchar usbFunctionWrite(uchar *receivedData, uchar len) {
	return -1;
}

/* ------------------------------------------------------------------------- */

void sendDataUSB(uchar* data, unsigned int byteCount) {
	int currentByte;
	int currentCount;

	currentByte = 0;

	while(currentByte < byteCount) {
		currentCount = byteCount - currentByte;

		if(currentCount > 8)
			currentCount = 8;

		usbSetInterrupt(data + currentByte, currentCount*sizeof(uchar));
		currentByte += currentCount;
		
		while(!usbInterruptIsReady())
			usbPoll();
	}
}

void setupUSB() {
    usbDeviceDisconnect(); /* enforce re-enumeration, do this while interrupts are disabled! */
    _delay_ms(300UL);/* fake USB disconnect for > 250 ms */
    usbDeviceConnect();
    usbInit();
    sei();
	usbPoll();
}

void disconnectUSB() {
    usbDeviceDisconnect(); /* enforce re-enumeration, do this while interrupts are disabled! */
    _delay_ms(300UL);/* fake USB disconnect for > 250 ms */
}

#include "trackball_controller.c"
