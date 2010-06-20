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
#define USB_MODE_PROGRAMMER 3

uchar mode = USB_MODE_PS3;

#define HID_REPORT_TYPE_INPUT 1
#define HID_REPORT_TYPE_OUTPUT 2
#define HID_REPORT_TYPE_FEATURE 3

PROGMEM char usbDescriptorDeviceDS[] = {    /* USB device descriptor */
    18,         /* sizeof(usbDescriptorDevice): length of descriptor in bytes */
    USBDESCR_DEVICE,        /* descriptor type */
    0x10, 0x01,             /* USB version supported */
    USB_CFG_DEVICE_CLASS,
    USB_CFG_DEVICE_SUBCLASS,
    0,                      /* protocol */
    8,                      /* max packet size */
    /* the following two casts affect the first byte of the constant only, but
     * that's sufficient to avoid a warning with the default values.
     */
    (char)USB_CFG_VENDOR_ID,/* 2 bytes */
    (char)USB_CFG_DEVICE_ID,/* 2 bytes */
    USB_CFG_DEVICE_VERSION, /* 2 bytes */
    1,          /* manufacturer string index */
    2,          /* product string index */
    0,          /* serial number string index */
    1,          /* number of configurations */
};

PROGMEM int  usbDescriptorStringVendorDS[] = {
    USB_STRING_DESCRIPTOR_HEADER(USB_CFG_VENDOR_NAME_LEN),
    USB_CFG_VENDOR_NAME
};

PROGMEM int  usbDescriptorStringDeviceDS[] = {
    USB_STRING_DESCRIPTOR_HEADER(USB_CFG_DEVICE_NAME_LEN),
    USB_CFG_DEVICE_NAME
};

PROGMEM int usbDescriptorStringSerialNumberDS[] = { 2, 8 };

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
#include "programmer.c"

usbMsgLen_t usbFunctionSetup(uchar receivedData[8]) {

	if(mode == USB_MODE_PS3) {
		return usbFunctionSetupPS3(receivedData);
	}
	else if(mode == USB_MODE_PROGRAMMER) {
		return usbFunctionSetupProgrammer(receivedData);
	}

    return 0;   /* default for not implemented requests: return no data back to host */
}

usbMsgLen_t usbFunctionDescriptor(struct usbRequest *rq)
{
	usbMsgLen_t len = 0;

    switch(rq->wValue.bytes[1]) {
    case USBDESCR_DEVICE:
		if(mode == USB_MODE_PROGRAMMER) {
	        usbMsgPtr = (uchar *)(usbDescriptorDeviceHIDBoot);
	        len = sizeof(usbDescriptorDeviceHIDBoot);
		}
		else {
	        usbMsgPtr = (uchar *)(usbDescriptorDeviceDS);
	        len = sizeof(usbDescriptorDeviceDS);
		}
		break;

    case USBDESCR_STRING:
        switch(rq->wValue.bytes[0]) {
        case 1: // Vendor
			if(mode == USB_MODE_PROGRAMMER)
		        usbMsgPtr = (uchar *)(usbDescriptorStringVendorHIDBoot);
			else
		        usbMsgPtr = (uchar *)(usbDescriptorStringVendorDS);

	        len = usbMsgPtr[0];
			break;
        case 2: // Device
			if(mode == USB_MODE_PROGRAMMER)
		        usbMsgPtr = (uchar *)(usbDescriptorStringDeviceHIDBoot);
			else
		        usbMsgPtr = (uchar *)(usbDescriptorStringDeviceDS);
	        len = usbMsgPtr[0];
			break;
		}

		break;

    case USBDESCR_CONFIG:
		if(mode == USB_MODE_PS3) {
        	usbMsgPtr = (uchar*)usbDescriptorConfigurationPS3;
        	len = sizeof(usbDescriptorConfigurationPS3);
		}
		else if(mode == USB_MODE_XBOX) {
        	usbMsgPtr = (uchar*)usbDescriptorConfigurationXBox;
        	len = sizeof(usbDescriptorConfigurationXBox);
		}
		else if(mode == USB_MODE_PROGRAMMER) {
			usbMsgPtr = (uchar*)usbDescriptorConfigurationProgrammer;
			len = sizeof(usbDescriptorConfigurationProgrammer);
		}
		else
			return 0;

		break;

    case USBDESCR_HID:
		if(mode == USB_MODE_PS3) {
        	usbMsgPtr = (uchar *)(usbDescriptorConfigurationPS3 + 18);
			len = usbDescriptorConfigurationPS3[18];
		}
		else if(mode == USB_MODE_PROGRAMMER) {
        	usbMsgPtr = (uchar *)(usbDescriptorConfigurationProgrammer + 18);
			len = usbDescriptorConfigurationProgrammer[18];
		}
		else
			return 0;

		break;

    case USBDESCR_HID_REPORT:
		if(mode == USB_MODE_PS3) {
			usbMsgPtr = (uchar*)usbHidReportDescriptorPS3;
			len = sizeof(usbHidReportDescriptorPS3);
		}
		else if(mode == USB_MODE_PROGRAMMER) {
			usbMsgPtr = (uchar*)usbHidReportDescriptorProgrammer;
			len = sizeof(usbHidReportDescriptorProgrammer);
		}
		else
			return 0;

		break;
    }

    return len;
}

uchar usbFunctionWrite(uchar *data, uchar len) {
	if(mode == USB_MODE_PROGRAMMER) {
		return usbFunctionWriteProgrammer(data, len);
	}

	return 0;
}

PROGMEM char usbDescriptorConfiguration[0] = {};

