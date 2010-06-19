#include <avr/eeprom.h>

PROGMEM char usbDescriptorConfigurationProgrammer[] = {
    /* HID USB configuration descriptor */
    9,          				/* sizeof(usbDescriptorConfiguration): length of descriptor in bytes */
    USBDESCR_CONFIG,   	 		/* descriptor type */
    34, 0,      				/* total length of data returned (including inlined descriptors) */
    1,          				/* number of interfaces in this configuration */
    1,          				/* index of this configuration */
    0,          				/* configuration name string index */
    (char)USBATTR_BUSPOWER, 	/* attributes */
    USB_CFG_MAX_BUS_POWER/2,	/* max USB current in 2mA units */
/* interface descriptor follows inline: */
    9,   			 	    	/* sizeof(usbDescrInterface): length of descriptor in bytes */
    USBDESCR_INTERFACE,			/* descriptor type */
    0,          				/* index of this interface */
    0,							/* alternate setting for this interface */
    1, 							/* endpoints excl 0: number of endpoint descriptors to follow */
    3,							/* USB interface class: HID */
    0,							/* USB interface subclass */
    0,							/* USB interface protocol */
    0,          				/* string index for interface */
    9,          				/* sizeof(usbDescrHID): length of descriptor in bytes */
    USBDESCR_HID,   			/* descriptor type: HID */
    0x01, 0x01,					/* BCD representation of HID version */
    0x00,       				/* target country code */
    0x01,       				/* number of HID Report (or other HID class) Descriptor infos to follow */
    0x22,       				/* descriptor type: report */
    33, 0,						/* total length of report descriptor */
    7,          				/* sizeof(usbDescrEndpoint) */
    USBDESCR_ENDPOINT,			/* descriptor type = endpoint */
    0x81,						/* IN endpoint number 1 */
    0x03,						/* attrib: Interrupt endpoint */
    8, 0,						/* maximum packet size */
    200, 							/* interrupt poll interval in ms */
};

#define EEPROM_SIZE_QUERY_REPORT_ID 3
#define EEPROM_PROGRAMMING_REPORT_ID 4

PROGMEM char usbHidReportDescriptorProgrammer[33] = {
    0x06, 0x00, 0xff,              		// USAGE_PAGE (Generic Desktop)
    0x09, 0x01,                    		// USAGE (Vendor Usage 1)
    0xa1, 0x01,                    		// COLLECTION (Application)
    0x15, 0x00,                    		//   LOGICAL_MINIMUM (0)
    0x26, 0xff, 0x00,              		//   LOGICAL_MAXIMUM (255)
    0x75, 0x08,                    		//   REPORT_SIZE (8)

    0x85, EEPROM_SIZE_QUERY_REPORT_ID,	//   REPORT_ID (EEPROM_SIZE_QUERY_REPORT_ID)
    0x95, 0x06,                    		//   REPORT_COUNT (6)
    0x09, 0x00,                    		//   USAGE (Undefined)
    0xb2, 0x02, 0x01,              		//   FEATURE (Data,Var,Abs,Buf)

    0x85, EEPROM_PROGRAMMING_REPORT_ID, //   REPORT_ID (EEPROM_PROGRAMMING_REPORT_ID)
    0x95, 0x83,                    		//   REPORT_COUNT (131)
    0x09, 0x00,                    		//   USAGE (Undefined)
    0xb2, 0x02, 0x01,              		//   FEATURE (Data,Var,Abs,Buf)

    0xc0                           		// END_COLLECTION
};

static uchar eepromOffset = 0xFF;
static uchar exitProgrammer = 0;
static uchar writeReportID = 0;

usbMsgLen_t usbFunctionSetupProgrammer(uchar receivedData[8]) {
	usbRequest_t* rq = (void *)receivedData;
	uchar reportType = rq->wValue.bytes[1];
	uchar reportID = rq->wValue.bytes[0];

	if((rq->bmRequestType & USBRQ_TYPE_MASK) == USBRQ_TYPE_CLASS) {
	    if(rq->bRequest == USBRQ_HID_SET_REPORT){
	        if(reportType == HID_REPORT_TYPE_FEATURE
			&& reportID == EEPROM_PROGRAMMING_REPORT_ID) {
	            eepromOffset = 0;
				writeReportID = EEPROM_PROGRAMMING_REPORT_ID;

	            return USB_NO_MSG; // use usbFunctionWrite()
	        }
			else
				exitProgrammer = 1;
	    }
		else if(rq->bRequest == USBRQ_HID_GET_REPORT) {
	        if(reportType == HID_REPORT_TYPE_FEATURE
			&& reportID == EEPROM_SIZE_QUERY_REPORT_ID) {
				data[0] = EEPROM_SIZE_QUERY_REPORT_ID;
				data[1] = E2PAGESIZE  & 0xff;
				data[2] = E2PAGESIZE >> 8;
				data[3] = ((long)E2END + 1) & 0xff;
				data[4] = (((long)E2END + 1) >> 8) & 0xff;
				data[5] = (((long)E2END + 1) >> 16) & 0xff;
				data[6] = (((long)E2END + 1) >> 24) & 0xff;
		        usbMsgPtr = data;

		        return 7;
	        }
			else
				exitProgrammer = 1;
	    }
	}

	return 0;
}

#ifndef ulong
#   define ulong    unsigned long
#endif
#ifndef uint
#   define uint     unsigned int
#endif

#define LONG_ADDRESSING (E2END) > 0xffff

#if LONG_ADDRESSING
#   define addr_t           ulong
#else
#   define addr_t           uint
#endif

static addr_t currentAddress; /* in bytes */

union {
    addr_t  l;
    uint    s[sizeof(addr_t)/2];
    uchar   c[sizeof(addr_t)];
} address;


// NOT FINISHED:
uchar usbFunctionWriteProgrammer(uchar *data, uchar len) {
	if(writeReportID == EEPROM_PROGRAMMING_REPORT_ID) {
		uchar   isLast;

	    address.l = currentAddress;

	    if(eepromOffset == 0) {
	        address.c[0] = data[1];
	        address.c[1] = data[2];
#if LONG_ADDRESSING
	        address.c[2] = data[3];
	        address.c[3] = 0;
#endif
	        data += 4;
	        len -= 4;
	    }

	    eepromOffset += len;
	    isLast = eepromOffset & 0x80; /* != 0 if last block received */

	    do {
	        addr_t prevAddr;

#if E2PAGESIZE > 256
	        uint pageAddr;
#else
	        uchar pageAddr;
#endif
	        pageAddr = address.s[0] & (SPM_PAGESIZE - 1);
	        cli();
	        eeprom_write_byte((uchar*)address.l, *data);
	        sei();
	        prevAddr = address.l;
	        address.l += 2;
	        data += 2;
	        /* write page when we cross page boundary */
	        pageAddr = address.s[0] & (SPM_PAGESIZE - 1);

	        if(pageAddr == 0) {
	            cli();
	            eeprom_write_byte((uchar*)address.l, *data);
	            sei();
	        }

	        len -= 2;
	    } while(len);

	    currentAddress = address.l;

	    return isLast;
	}
}


void programmer() {
	mode = USB_MODE_PROGRAMMER;
	usbPrepare();

    while(usbInterruptIsReady() && !exitProgrammer) { /* main event loop */
		usbPoll();
    }
}
