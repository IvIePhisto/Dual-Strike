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

#if ATMEGA_NO == 168
typedef struct {
	uchar reportID;
	uchar data[4];
	uchar previousData[4];
} mame_keyboard_report_t;


typedef struct {
	uchar reportID;
	uchar data;
} mame_consumer_report_t;

typedef struct {
	mame_keyboard_report_t keyboard;
	mame_consumer_report_t consumer;
} mame_reports_t;

static uchar mameIdleRate = 125; // 500ms
#endif

typedef union {
	uchar array[132];
	ps3report_t ps3report;
#if ATMEGA_NO == 168
	mame_reports_t mame_reports;
#endif
} usb_data_t;

extern usb_data_t data;
START_STATE_VARIABLES

#define HID_REPORT_TYPE_INPUT 1
#define HID_REPORT_TYPE_OUTPUT 2
#define HID_REPORT_TYPE_FEATURE 3

static uchar usbMode = -1;

#define USB_MODE_PROGRAMMER 0
#define USB_MODE_PS3 1

#if ATMEGA_NO == 168
#define USB_MODE_MAME 2
#define USB_MODE_XBOX 3
#endif

#include "descriptors.c"

#if (E2END) > 0xffff /* we need long addressing */
#   define e2addr_t           ulong
#else
#   define e2addr_t           uint
#endif

typedef union {
    e2addr_t  l;
    uchar   c[sizeof(e2addr_t)];
} e2address_t;

static e2addr_t eepromOffset = -1;
static uchar writeReportID = 0;
static e2addr_t currentEEPROMAddress;
//static uint debugCount = 0; //DEBUG

usbMsgLen_t usbFunctionSetup(uchar receivedData[8]) {
	usbRequest_t    *rq = (void *)receivedData;
	uchar reportID = rq->wValue.bytes[0];
	uchar reportType = rq->wValue.bytes[1];

	switch(usbMode) {
	case USB_MODE_PS3:
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
		break;

	case USB_MODE_PROGRAMMER:
		if((rq->bmRequestType & USBRQ_TYPE_MASK) == USBRQ_TYPE_CLASS) {
		    if(rq->bRequest == USBRQ_HID_SET_REPORT){
		        if(reportID == EEPROM_PROGRAMMING_REPORT_ID) {
		            eepromOffset = 0;
					writeReportID = EEPROM_PROGRAMMING_REPORT_ID;

		            return USB_NO_MSG; // use usbFunctionWrite()
		        }
				else if(reportID == EEPROM_SET_ADDRESS_REPORT_ID) {
					writeReportID = EEPROM_SET_ADDRESS_REPORT_ID;

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
				else if(reportID == EEPROM_READING_REPORT_ID) {
					if(currentEEPROMAddress > E2END)
						return -1;

					size_t length = 128;		            
					e2addr_t rest = E2END - currentEEPROMAddress + 1;

					if(rest < 128)
						length = rest;

					data.array[0] = EEPROM_READING_REPORT_ID;
					data.array[1] = length;
					data.array[2] = 0;
					data.array[3] = 0;
					eeprom_read_block(data.array + 4, (void*)currentEEPROMAddress, length);
					usbMsgPtr = data.array;

		            return 132;
				}
		    }
		}
#if ATMEGA_NO == 168
		break;

	case USB_MODE_MAME:
		if ((rq-> bmRequestType & USBRQ_TYPE_MASK) == USBRQ_TYPE_CLASS) {
			if (rq->bRequest == USBRQ_HID_GET_IDLE) {
				if((reportID == 0) || (reportID == 1)) {
		    		usbMsgPtr = &mameIdleRate;
			    	return 1;
				}
		    }
			else if(rq->bRequest == USBRQ_HID_SET_IDLE) {
				if((reportID == 0) || (reportID == 1)) {	
					mameIdleRate = reportType;
				}
		    }
			else if(rq->bRequest == USBRQ_HID_GET_REPORT) {
				if(reportID == 1) {
					usbMsgPtr = (void*)&data.mame_reports.keyboard;

					return 5;
				}
				else if(reportID == 2) {
					usbMsgPtr = (void*)&data.mame_reports.consumer;

					return 2;
				}
			}
		}
		break;

	case USB_MODE_XBOX:
	    if((rq->bmRequestType & USBRQ_TYPE_MASK) == USBRQ_TYPE_CLASS) {
			if(rq->bRequest == USBRQ_HID_GET_REPORT) {
				// is called only once in my setup

				usbMsgPtr = data.array;
				//eeprom_write_word((void*)E2END-1, ++debugCount); // DEBUG

				return 20;
			}
		}
		else
		if ((rq-> bmRequestType & USBRQ_TYPE_MASK) == USBRQ_TYPE_VENDOR) {
			if(rq->bRequest == 0x06) {
				/*
				receivedData == { 0xc1, 0x06, 0x00, 0x42, 0x00, 0x00, 0x10, 0x00 }
				probably means:
				-bmRequestType:
				 +direction: device to host
				 +type:      vendor
				 +recipient: interface
				-request: 0x06
				-wValue:  0, 0x42
				-wIndex:  0, 0
				-wLength: 16
				*/
				usbMsgPtr = &data.array[20];

				return 16;
			}
		}
#endif
	}

    return 0;   /* default for not implemented requests: return no data back to host */
}

static int remainingBytes;

uchar usbFunctionWrite(uchar *receivedData, uchar len) {
	if(usbMode == USB_MODE_PROGRAMMER) {
		e2address_t e2address;

		if(writeReportID == EEPROM_PROGRAMMING_REPORT_ID) {
			uchar   isLast;

		    if(eepromOffset == 0) {
				e2address.c[0] = receivedData[1];
				e2address.c[1] = receivedData[2];
#if (E2END) > 0xffff /* we need long addressing */
		        e2address.c[2] = receivedData[3];
		        e2address.c[3] = 0;
#endif
				remainingBytes = receivedData[4];
		        receivedData += 5;
		        len -= 5;
		    }
			else {
			    e2address.l = currentEEPROMAddress;
			}


		    eepromOffset += len;
		    isLast = eepromOffset & 0x80; /* != 0 if last block received */

			if(remainingBytes > 0) {
				if(remainingBytes < 8)
					len = remainingBytes;

		        //cli();
				eeprom_write_block(receivedData, (void*)e2address.l, len);
		        //sei();
				e2address.l += len;
			    currentEEPROMAddress = e2address.l;
				remainingBytes -= len;
			}

		    return isLast;
		}
		else if(writeReportID == EEPROM_SET_ADDRESS_REPORT_ID) {
			e2address.c[0] = receivedData[1];
			e2address.c[1] = receivedData[2];
#if (E2END) > 0xffff /* we need long addressing */
	        e2address.c[2] = receivedData[3];
#else
	        e2address.c[2] = 0;
#endif
	        e2address.c[3] = 0;
			currentEEPROMAddress = e2address.l;

			return 1;
		}
	}

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

		while(!usbInterruptIsReady())
			usbPoll();

		usbSetInterrupt(data + currentByte, currentCount*sizeof(uchar));
		currentByte += currentCount;		
	}
}

void sendDataUSB3(uchar* data, unsigned int byteCount) {
	int currentByte;
	int currentCount;

	currentByte = 0;

	while(currentByte < byteCount) {
		currentCount = byteCount - currentByte;

		if(currentCount > 8)
			currentCount = 8;

		while(!usbInterruptIsReady3())
			usbPoll();

		usbSetInterrupt3(data + currentByte, currentCount*sizeof(uchar));
		currentByte += currentCount;
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

#include "programmer.c"
#include "ps3.c"

#if ATMEGA_NO == 168
#include "mame.c"
#include "xbox.c"
#endif
