#include "usb_controller.h"

#include "../DualStrike.h"

#include <avr/io.h>
#include <avr/interrupt.h>  /* for sei() */
#include <util/delay.h>     /* for _delay_ms() */

#include <avr/pgmspace.h>   /* required by usbdrv.h */
#include "usbdrv/usbdrv.h"


extern uint8_t config[2];
extern uchar* data;

#define USB_MODE_PS3 1
#define USB_MODE_XBOX 2

uchar mode = USB_MODE_PS3;

#define HID_REPORT_TYPE_INPUT 1
#define HID_REPORT_TYPE_OUTPUT 2
#define HID_REPORT_TYPE_FEATURE 3

usbMsgLen_t usbFunctionSetup(uchar receivedData[8])
{
	if(mode != USB_MODE_PS3)
		return 0;

	usbRequest_t* rq = (void *)receivedData;

    if((rq->bmRequestType & USBRQ_TYPE_MASK) == USBRQ_TYPE_CLASS) {    // class request
		// wValue: ReportType (highbyte), ReportID (lowbyte)
        if(rq->bRequest == USBRQ_HID_GET_REPORT) {
			if(rq->wValue.bytes[1] == HID_REPORT_TYPE_FEATURE) {
				if(rq->wValue.bytes[0] == 0) {
					 // set buffer data
					data[0] = 0x21; // 0b00100001 0d33
					data[1] = 0x26; // 0b00100110 0d38

					for(int i = 2; i < 8; i++)
						data[i] = 0;

					usbMsgPtr = data;

					return 8;
				}
			}
        }
    }

    return 0;   /* default for not implemented requests: return no data back to host */
}

void sendUSBData(uchar* data, unsigned int byteCount) {
	int currentByte;
	int currentCount;

	currentByte = 0;

	while(currentByte < byteCount) {
		currentCount = byteCount - currentByte;

		if(currentCount > 8)
			currentCount = 8;

		usbSetInterrupt(data + currentByte, currentCount*sizeof(uchar));
		currentByte += currentCount;

		while(!usbInterruptIsReady()) usbPoll();
	}
}

void usbPrepare() {
    usbDeviceDisconnect(); /* enforce re-enumeration, do this while interrupts are disabled! */
    _delay_ms(300UL);/* fake USB disconnect for > 250 ms */
    usbDeviceConnect();
    usbInit();
    sei();
	usbPoll();
}

#include "ps3.c"
#include "xbox.c"

usbMsgLen_t usbFunctionDescriptor(struct usbRequest *rq)
{
	usbMsgLen_t len = 0;

    switch(rq->wValue.bytes[1]) {
    case USBDESCR_CONFIG:
		if(mode == USB_MODE_PS3) {
        	usbMsgPtr = (uchar *)(usbDescriptorConfigurationPS3);
        	len = sizeof(usbDescriptorConfigurationPS3);
		}
		else if(mode == USB_MODE_XBOX) {
        	usbMsgPtr = (uchar *)(usbDescriptorConfigurationXBox);
        	len = sizeof(usbDescriptorConfigurationXBox);
		}
		else
			return 0;

		break;
    case USBDESCR_HID:
		// TODO
		break;
    case USBDESCR_HID_REPORT:
		// TODO
		break;
    }

    return len;
}

PROGMEM char usbDescriptorConfiguration[0] = {};

