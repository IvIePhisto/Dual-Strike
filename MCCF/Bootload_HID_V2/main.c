/* Name: main.c
 * Project: AVR bootloader HID
 * Author: Christian Starkjohann
 * Creation Date: 2007-03-19
 * Tabsize: 4
 * Copyright: (c) 2007 by OBJECTIVE DEVELOPMENT Software GmbH
 * License: GNU GPL v2 (see License.txt)
 * This Revision: $Id: main.c 693 2008-11-14 15:09:37Z cs $
 */

#include <avr/io.h>
#include <avr/interrupt.h>
#include <avr/pgmspace.h>
#include <avr/wdt.h>
#include <avr/boot.h>
#include <string.h>
#include <util/delay.h>

static void leaveBootloader() __attribute__((__noreturn__));

#include "bootloaderconfig.h"
#include "usbdrv.c"

/* ------------------------------------------------------------------------ */

#ifndef ulong
#   define ulong    unsigned long
#endif
#ifndef uint
#   define uint     unsigned int
#endif

#if (FLASHEND) > 0xffff /* we need long addressing */
#   define addr_t           ulong
#else
#   define addr_t           uint
#endif

static addr_t           currentAddress; /* in bytes */
static uchar            offset;         /* data already processed in current transfer */
#if BOOTLOADER_CAN_EXIT
static uchar            exitMainloop;
#endif

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

#define EEPROM_SIZE_QUERY_REPORT_ID 3
#define EEPROM_PROGRAMMING_REPORT_ID 4
#define EEPROM_SET_ADDRESS_REPORT_ID 5
#define EEPROM_READING_REPORT_ID 6

PROGMEM char usbHidReportDescriptor[69] = {
    0x06, 0x00, 0xff,              // USAGE_PAGE (Generic Desktop)
    0x09, 0x01,                    // USAGE (Vendor Usage 1)
    0xa1, 0x01,                    // COLLECTION (Application)
    0x15, 0x00,                    //   LOGICAL_MINIMUM (0)
    0x26, 0xff, 0x00,              //   LOGICAL_MAXIMUM (255)
    0x75, 0x08,                    //   REPORT_SIZE (8)

    0x85, 0x01,                    //   REPORT_ID (1)
    0x95, 0x06,                    //   REPORT_COUNT (6)
    0x09, 0x00,                    //   USAGE (Undefined)
    0xb2, 0x02, 0x01,              //   FEATURE (Data,Var,Abs,Buf)

    0x85, 0x02,                    //   REPORT_ID (2)
    0x95, 0x83,                    //   REPORT_COUNT (131)
    0x09, 0x00,                    //   USAGE (Undefined)
    0xb2, 0x02, 0x01,              //   FEATURE (Data,Var,Abs,Buf)

    0x85, EEPROM_SIZE_QUERY_REPORT_ID,	//   REPORT_ID (EEPROM_SIZE_QUERY_REPORT_ID)
    0x95, 0x06,                    		//   REPORT_COUNT (6)
    0x09, 0x00,                    		//   USAGE (Undefined)
    0xb2, 0x02, 0x01,              		//   FEATURE (Data,Var,Abs,Buf)

    0x85, EEPROM_PROGRAMMING_REPORT_ID, //   REPORT_ID (EEPROM_PROGRAMMING_REPORT_ID)
    0x95, 0x84,                    		//   REPORT_COUNT (132)
    0x09, 0x00,                    		//   USAGE (Undefined)
    0xb2, 0x02, 0x01,              		//   FEATURE (Data,Var,Abs,Buf)

    0x85, EEPROM_SET_ADDRESS_REPORT_ID,	//   REPORT_ID (EEPROM_SET_ADDRESS_REPORT_ID)
    0x95, 0x03,                    		//   REPORT_COUNT (3)
    0x09, 0x00,                    		//   USAGE (Undefined)
    0xb2, 0x02, 0x01,              		//   FEATURE (Data,Var,Abs,Buf)

    0x85, EEPROM_READING_REPORT_ID,		//   REPORT_ID (EEPROM_READING_REPORT_ID)
    0x95, 0x83,                    		//   REPORT_COUNT (131)
    0x09, 0x00,                    		//   USAGE (Undefined)
    0xb2, 0x02, 0x01,              		//   FEATURE (Data,Var,Abs,Buf)

    0xc0                           // END_COLLECTION
};

/* allow compatibility with avrusbboot's bootloaderconfig.h: */
#ifdef BOOTLOADER_INIT
#   define bootLoaderInit()         BOOTLOADER_INIT
#endif
#ifdef BOOTLOADER_CONDITION
#   define bootLoaderCondition()    BOOTLOADER_CONDITION
#endif

/* compatibility with ATMega88 and other new devices: */
#ifndef TCCR0
#define TCCR0   TCCR0B
#endif
#ifndef GICR
#define GICR    MCUCR
#endif

static void (*nullVector)(void) __attribute__((__noreturn__));

static void leaveBootloader()
{
    DBG1(0x01, 0, 0);
    cli();
    boot_rww_enable();
    USB_INTR_ENABLE = 0;
    USB_INTR_CFG = 0;       /* also reset config bits */
#if F_CPU == 12800000
    TCCR0 = 0;              /* default value */
#endif
    GICR = (1 << IVCE);     /* enable change of interrupt vectors */
    GICR = (0 << IVSEL);    /* move interrupts to application flash section */
/* We must go through a global function pointer variable instead of writing
 *  ((void (*)(void))0)();
 * because the compiler optimizes a constant 0 to "rcall 0" which is not
 * handled correctly by the assembler.
 */
    nullVector();
}

uchar   usbFunctionSetup(uchar data[8])
{
usbRequest_t    *rq = (void *)data;
static uchar    replyBuffer[132];

    if(rq->bRequest == USBRQ_HID_SET_REPORT){
        if(rq->wValue.bytes[0] == 2){
            offset = 0;
			writeReportID = 2;

            return USB_NO_MSG;
        }
        else if(rq->wValue.bytes[0] == EEPROM_PROGRAMMING_REPORT_ID) {
            eepromOffset = 0;
			writeReportID = EEPROM_PROGRAMMING_REPORT_ID;

            return USB_NO_MSG; // use usbFunctionWrite()
        }
		else if(rq->wValue.bytes[0] == EEPROM_SET_ADDRESS_REPORT_ID) {
			writeReportID = EEPROM_SET_ADDRESS_REPORT_ID;

            return USB_NO_MSG; // use usbFunctionWrite()
		}

#if BOOTLOADER_CAN_EXIT
        else{
            exitMainloop = 1;
        }
#endif
    }
	else if(rq->bRequest == USBRQ_HID_GET_REPORT){
		if(rq->wValue.bytes[0] == 1) {
			replyBuffer[0] = 1;
			replyBuffer[1] = SPM_PAGESIZE & 0xff;
			replyBuffer[2] = SPM_PAGESIZE >> 8;
			replyBuffer[3] = ((long)FLASHEND + 1) & 0xff;
			replyBuffer[4] = (((long)FLASHEND + 1) >> 8) & 0xff;
			replyBuffer[5] = (((long)FLASHEND + 1) >> 16) & 0xff;
			replyBuffer[6] = (((long)FLASHEND + 1) >> 24) & 0xff;
	        usbMsgPtr = replyBuffer;

	        return 7;
		}
	    else if(rq->wValue.bytes[0] == EEPROM_SIZE_QUERY_REPORT_ID) {
			replyBuffer[0] = EEPROM_SIZE_QUERY_REPORT_ID;
			replyBuffer[1] = E2PAGESIZE  & 0xff;
			replyBuffer[2] = E2PAGESIZE >> 8;
			replyBuffer[3] = ((long)E2END + 1) & 0xff;
			replyBuffer[4] = (((long)E2END + 1) >> 8) & 0xff;
			replyBuffer[5] = (((long)E2END + 1) >> 16) & 0xff;
			replyBuffer[6] = (((long)E2END + 1) >> 24) & 0xff;
	        usbMsgPtr = replyBuffer;

	        return 7;
        }
		else if(rq->wValue.bytes[0] == EEPROM_READING_REPORT_ID) {
			if(currentEEPROMAddress > E2END)
				return -1;

			size_t length = 128;		            
			e2addr_t rest = E2END - currentEEPROMAddress + 1;

			if(rest < 128)
				length = rest;

			replyBuffer[0] = EEPROM_READING_REPORT_ID;
			replyBuffer[1] = length;
			replyBuffer[2] = 0;
			replyBuffer[3] = 0;
			eeprom_read_block(replyBuffer + 4, (void*)currentEEPROMAddress, length);
			usbMsgPtr = replyBuffer;

            return 132;
		}

    }
    return 0;
}

static int remainingBytes;

uchar usbFunctionWrite(uchar *data, uchar len)
{
uchar   isLast;
	e2address_t e2address;

	if(writeReportID == 2) {
		union {
		    addr_t  l;
		    uint    s[sizeof(addr_t)/2];
		    uchar   c[sizeof(addr_t)];
		}       address;
	    address.l = currentAddress;
	    if(offset == 0){
	        DBG1(0x30, data, 3);
	        address.c[0] = data[1];
	        address.c[1] = data[2];
#if (FLASHEND) > 0xffff /* we need long addressing */
	        address.c[2] = data[3];
	        address.c[3] = 0;
#endif
	        data += 4;
	        len -= 4;
	    }
	    DBG1(0x31, (void *)&currentAddress, 4);
	    offset += len;
	    isLast = offset & 0x80; /* != 0 if last block received */
	    do{
	        addr_t prevAddr;
#if SPM_PAGESIZE > 256
	        uint pageAddr;
#else
	        uchar pageAddr;
#endif
	        DBG1(0x32, 0, 0);
	        pageAddr = address.s[0] & (SPM_PAGESIZE - 1);
	        if(pageAddr == 0){              /* if page start: erase */
	            DBG1(0x33, 0, 0);
#ifndef TEST_MODE
	            cli();
	            boot_page_erase(address.l); /* erase page */
	            sei();
	            boot_spm_busy_wait();       /* wait until page is erased */
#endif
	        }
	        cli();
	        boot_page_fill(address.l, *(short *)data);
	        sei();
	        prevAddr = address.l;
	        address.l += 2;
	        data += 2;
	        /* write page when we cross page boundary */
	        pageAddr = address.s[0] & (SPM_PAGESIZE - 1);
	        if(pageAddr == 0){
	            DBG1(0x34, 0, 0);
#ifndef TEST_MODE
	            cli();
	            boot_page_write(prevAddr);
	            sei();
	            boot_spm_busy_wait();
#endif
	        }
	        len -= 2;
	    }while(len);
	    currentAddress = address.l;
	    DBG1(0x35, (void *)&currentAddress, 4);
	    return isLast;
	}
	else if(writeReportID == EEPROM_PROGRAMMING_REPORT_ID) {
	    if(eepromOffset == 0) {
			e2address.c[0] = data[1];
			e2address.c[1] = data[2];
#if (E2END) > 0xffff /* we need long addressing */
	        e2address.c[2] = data[3];
	        e2address.c[3] = 0;
#endif
			remainingBytes = data[4];
	        data += 5;
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
			eeprom_write_block(data, (void*)e2address.l, len);
	        //sei();
			e2address.l += len;
		    currentEEPROMAddress = e2address.l;
			remainingBytes -= len;
		}

	    return isLast;
	}
	else if(writeReportID == EEPROM_SET_ADDRESS_REPORT_ID) {
		e2address.c[0] = data[1];
		e2address.c[1] = data[2];
#if (E2END) > 0xffff /* we need long addressing */
        e2address.c[2] = data[3];
#else
        e2address.c[2] = 0;
#endif
        e2address.c[3] = 0;
		currentEEPROMAddress = e2address.l;

		return 1;
	}

	return -1;
}

static void initForUsbConnectivity(void)
{
uchar   i = 0;

#if F_CPU == 12800000
    TCCR0 = 3;          /* 1/64 prescaler */
#endif
    usbInit();
    /* enforce USB re-enumerate: */
    usbDeviceDisconnect();  /* do this while interrupts are disabled */
    do{             /* fake USB disconnect for > 250 ms */
        wdt_reset();
        _delay_ms(1);
    }while(--i);
    usbDeviceConnect();
    sei();
}

int main(void)
{
    /* initialize hardware */
    bootLoaderInit();
    odDebugInit();
    DBG1(0x00, 0, 0);
    /* jump to application if jumper is set */
    if(enterBootLoaderCondition()){
        uchar i = 0, j = 0;
#ifndef TEST_MODE
        GICR = (1 << IVCE);  /* enable change of interrupt vectors */
        GICR = (1 << IVSEL); /* move interrupts to boot flash section */
#endif
        initForUsbConnectivity();
        do{ /* main event loop */
            wdt_reset();
            usbPoll();
#if BOOTLOADER_CAN_EXIT
            if(exitMainloop){
#if F_CPU == 12800000
                break;  /* memory is tight at 12.8 MHz, save exit delay below */
#endif
                if(--i == 0){
                    if(--j == 0)
                        break;
                }
            }
#endif
        }while(!exitBootLoaderCondition());
    }
    leaveBootloader();
    return 0;
}


