#include "usb_controller.h"

#include "../DualStrike.h"

#include <avr/io.h>
#include <avr/interrupt.h>  /* for sei() */
#include <util/delay.h>     /* for _delay_ms() */
#include <avr/eeprom.h>

#include <avr/pgmspace.h>   /* required by usbdrv.h */
#include "usbdrv/usbdrv.h"

extern uint8_t config[2];

/* ------------------------------------------------------------------------- */

typedef struct {
	uchar	buttons[2];
	uchar	hatswitch;
	uchar	joystick_axes;
	uchar	analogue_buttons[12];
} ps3report_t;

typedef union {
	uchar array[20];
	ps3report_t ps3report;
} usb_data_t;

extern usb_data_t data;

#define HID_REPORT_TYPE_INPUT 1
#define HID_REPORT_TYPE_OUTPUT 2
#define HID_REPORT_TYPE_FEATURE 3

uchar usbMode = 0;

#define USB_MODE_PS3 1
#define USB_MODE_PROGRAMMER 2

#include "descriptors.c"

usbMsgLen_t usbFunctionSetup(uchar receivedData[8]) {
	usbRequest_t    *rq = (void *)receivedData;
	uchar reportID = rq->wValue.bytes[0];
	uchar reportType = rq->wValue.bytes[1];

	if(usbMode == USB_MODE_PS3) {
	    if((rq->bmRequestType & USBRQ_TYPE_MASK) == USBRQ_TYPE_CLASS) {    // class request
			// wValue: ReportType (highbyte), ReportID (lowbyte)
	        if(rq->bRequest == USBRQ_HID_GET_REPORT) {
				if(reportType == HID_REPORT_TYPE_FEATURE) {
					if(rq->wValue.bytes[0] == 0) {
						 // set buffer data
						data.array[0] = 0x21; // 0b00100001 0d33
						data.array[1] = 0x26; // 0b00100110 0d38

						for(int i = 2; i < 8; i++)
							data.array[i] = 0;

						usbMsgPtr = data.array;

						return 8;
					}
				}
	        }
	    }
	}
	else if(usbMode == USB_MODE_PROGRAMMER) {
		if((rq->bmRequestType & USBRQ_TYPE_MASK) == USBRQ_TYPE_CLASS) {
		    if(rq->bRequest == USBRQ_HID_SET_REPORT){
		        if(reportID == EEPROM_PROGRAMMING_REPORT_ID) {
		            eepromOffset = 0;
					writeReportID = EEPROM_PROGRAMMING_REPORT_ID;

		            return USB_NO_MSG; // use usbFunctionWrite()
		        }
		    }
			else if(rq->bRequest == USBRQ_HID_GET_REPORT) {
		        if(reportID == EEPROM_SIZE_QUERY_REPORT_ID) {
					data.array[0] = EEPROM_SIZE_QUERY_REPORT_ID;
					data.array[1] = E2PAGESIZE  & 0xff;
					data.array[2] = E2PAGESIZE >> 8;
					data.array[3] = ((long)E2END + 1) & 0xff;
					data.array[4] = (((long)E2END + 1) >> 8) & 0xff;
					data.array[5] = (((long)E2END + 1) >> 16) & 0xff;
					data.array[6] = (((long)E2END + 1) >> 24) & 0xff;
			        usbMsgPtr = data.array;

			        return 7;
		        }
		    }
		}
	}

    return 0;   /* default for not implemented requests: return no data back to host */
}

uchar usbFunctionWrite(uchar *receivedData, uchar len) {
	if(usbMode == USB_MODE_PROGRAMMER) {
		uchar   isLast;

		if(writeReportID == EEPROM_PROGRAMMING_REPORT_ID) {
			size_t eepromAddress;

		    if(eepromOffset == 0) {
				eepromAddress = receivedData[1];
		        receivedData += 4;
		        len -= 4;
		    }
			else {
			    eepromAddress = currentEEPROMAddress;
			}

		    eepromOffset += len;
		    isLast = eepromOffset & 0x80; /* != 0 if last block received */
	        //cli();
			eeprom_write_block(receivedData, (void*)eepromAddress, len);
	        //sei();
		    currentEEPROMAddress = eepromAddress + len;

		    return isLast;
		}
	}

	return 0;
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

#include "ps3.c"
#include "programmer.c"
