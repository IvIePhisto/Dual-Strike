#include "usb_controller.h"

#include "../MemoryTest.h"

#include <avr/io.h>
#include <avr/interrupt.h>  /* for sei() */
#include <util/delay.h>     /* for _delay_ms() */
#include <avr/eeprom.h>

#include <avr/pgmspace.h>   /* required by usbdrv.h */
#include "usbdrv/usbdrv.h"

/* ------------------------------------------------------------------------- */

typedef union {
	uchar array[132];
} usb_data_t;

extern usb_data_t data;

#define HID_REPORT_TYPE_INPUT 1
#define HID_REPORT_TYPE_OUTPUT 2
#define HID_REPORT_TYPE_FEATURE 3

#include "descriptors.c"

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

uchar usbIsConfigured() {
	return (usbConfiguration == 1);
}

void setupUSB() {
    usbDeviceDisconnect(); /* enforce re-enumeration, do this while interrupts are disabled! */
    _delay_ms(300UL);/* fake USB disconnect for > 250 ms */
    usbDeviceConnect();
    usbInit();
    sei();
	usbPoll();
}

/* ------------------------------------------------------------------------- */

#if (E2END) > 0xffff /* we need long addressing */
#   define e2addr_t           ulong
#else
#   define e2addr_t           uint
#endif

#if (FLASHEND) > 0xffff /* we need long addressing */
#   define faddr_t           ulong
#else
#   define faddr_t           uint
#endif

typedef union {
    e2addr_t  l;
    uchar   c[sizeof(e2addr_t)];
} e2address_t;

typedef union {
    faddr_t  l;
    uchar   c[sizeof(faddr_t)];
} flashAddress_t;

static e2addr_t eepromOffset = -1;
static e2addr_t currentE2Address;

static faddr_t currentFlashAddress;

static uchar writeReportID = 0;


usbMsgLen_t usbFunctionSetup(uchar receivedData[8]) {
	usbRequest_t    *rq = (void *)receivedData;
	uchar reportID = rq->wValue.bytes[0];
	//uchar reportType = rq->wValue.bytes[1];

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
			else if(reportID == FLASH_SET_ADDRESS_REPORT_ID) {
				writeReportID = FLASH_SET_ADDRESS_REPORT_ID;

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
				if(currentE2Address > E2END)
					return -1;

				size_t length = 128;		            
				e2addr_t rest = E2END - currentE2Address + 1;

				if(rest < 128)
					length = rest;

				data.array[0] = EEPROM_READING_REPORT_ID;
				data.array[1] = length;
				data.array[2] = 0;
				data.array[3] = 0;
				eeprom_read_block(data.array + 4, (void*)currentE2Address, length);
				usbMsgPtr = data.array;

	            return 132;
			}
	        else if(reportID == FLASH_SIZE_QUERY_REPORT_ID) {
				data.array[0] = FLASH_SIZE_QUERY_REPORT_ID;
				data.array[1] = SPM_PAGESIZE & 0xff;
				data.array[2] = SPM_PAGESIZE >> 8;
				data.array[3] = ((long)FLASHEND + 1) & 0xff;
				data.array[4] = (((long)FLASHEND + 1) >> 8) & 0xff;
				data.array[5] = (((long)FLASHEND + 1) >> 16) & 0xff;
				data.array[6] = (((long)FLASHEND + 1) >> 24) & 0xff;
		        usbMsgPtr = data.array;

		        return 7;
	        }
			else if(reportID == FLASH_READING_REPORT_ID) {
				if(currentFlashAddress > FLASHEND)
					return -1;

				faddr_t length = 128;		            
				faddr_t rest = FLASHEND - currentFlashAddress + 1;

				if(rest < 128)
					length = rest;

				data.array[0] = FLASH_READING_REPORT_ID;
				data.array[1] = length;
				data.array[2] = 0;
				data.array[3] = 0;

				for(faddr_t i = 0; i < length; i++) {
					faddr_t address = currentFlashAddress + i;

#if (FLASHEND) > 0xffff /* we need long addressing */					
					data.array[i+4] = pgm_read_byte_far((void*)(address));
#else
					data.array[i+4] = pgm_read_byte((void*)(address));
#endif

				}

				usbMsgPtr = data.array;

	            return 132;
			}
	    }
	}

    return 0;   /* default for not implemented requests: return no data back to host */
}

static int remainingBytes;

uchar usbFunctionWrite(uchar *receivedData, uchar len) {
	e2address_t e2address;
	flashAddress_t flashAddress;

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
		    e2address.l = currentE2Address;
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
		    currentE2Address = e2address.l;
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
		currentE2Address = e2address.l;

		return 1;
	}
	else if(writeReportID == FLASH_SET_ADDRESS_REPORT_ID) {
		flashAddress.c[0] = receivedData[1];
		flashAddress.c[1] = receivedData[2];
#if (FLASHEND) > 0xffff /* we need long addressing */
        flashAddress.c[2] = receivedData[3];
#else
        flashAddress.c[2] = 0;
#endif
        flashAddress.c[3] = 0;
		currentFlashAddress = flashAddress.l;

		return 1;
	}

	return -1;
}

/* ------------------------------------------------------------------------- */

void disconnectUSB() {
    usbDeviceDisconnect(); // enforce re-enumeration, do this while interrupts are disabled!
    _delay_ms(300UL); // fake USB disconnect for > 250 ms
}

#include "programmer.c"

