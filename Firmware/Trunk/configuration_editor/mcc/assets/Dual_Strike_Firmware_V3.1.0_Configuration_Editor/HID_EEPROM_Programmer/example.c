// EEPROM PROGRAMMER example V-USB code

PROGMEM char usbDescriptorDeviceEP[] = {    /* USB device descriptor */
    18,         /* sizeof(usbDescriptorDevice): length of descriptor in bytes */
    USBDESCR_DEVICE,        /* descriptor type */
    0x10, 0x01,             /* USB version supported */
    USB_CFG_DEVICE_CLASS,
    USB_CFG_DEVICE_SUBCLASS,
    0,                      /* protocol */
    8,                      /* max packet size */
    0xc0, 0x16,				/* Objective Development */
    0xdf, 0x05,				/* Shared USB HID ID */
    USB_CFG_DEVICE_VERSION, /* 2 bytes */
    1,          			/* manufacturer string index */
    2,          			/* product string index */
    0,          			/* serial number string index */
    1,          			/* number of configurations */
};

PROGMEM int  usbDescriptorStringVendorEP[] = {
    USB_STRING_DESCRIPTOR_HEADER(20),
    'p', 'o', 'h', 'l', '-', 'm', 'i', 'c', 'h', 'a', 'e', 'l', '@', 'g', 'm', 'x', '.', 'b', 'i', 'z'
};

PROGMEM int  usbDescriptorStringDeviceEP[] = {
    USB_STRING_DESCRIPTOR_HEADER(6),
    'H', 'I', 'D', ' ', 'E', 'P', 
};

#define EEPROM_SIZE_QUERY_REPORT_ID 1
#define EEPROM_PROGRAMMING_REPORT_ID 2
#define EEPROM_SET_ADDRESS_REPORT_ID 3
#define EEPROM_READING_REPORT_ID 4

PROGMEM char usbHidReportDescriptorEP[51] = {
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

    0xc0                           		// END_COLLECTION
};

PROGMEM char usbDescriptorConfigurationEP[] = {
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
    USBDESCR_HID,				/* descriptor type: HID */
    0x01, 0x01,					/* BCD representation of HID version */
    0x00,       				/* target country code */
    0x01,       				/* number of HID Report (or other HID class) Descriptor infos to follow */
    0x22,       				/* descriptor type: report */
    sizeof(usbHidReportDescriptorEP), 0, /* total length of report descriptor */
    7,          				/* sizeof(usbDescrEndpoint) */
    USBDESCR_ENDPOINT,			/* descriptor type = endpoint */
    0x81,						/* IN endpoint number 1 */
    0x03,						/* attrib: Interrupt endpoint */
    8, 0,						/* maximum packet size */
    200 						/* interrupt poll interval in ms */
};

usbMsgLen_t usbFunctionDescriptor(struct usbRequest *rq) {
	usbMsgLen_t len = 0;


	switch(rq->wValue.bytes[1]) {
	case USBDESCR_DEVICE:
		usbMsgPtr = (uchar *)(usbDescriptorDeviceEP);
		len = sizeof(usbDescriptorDeviceEP);
		break;

	case USBDESCR_STRING:
		switch(rq->wValue.bytes[0]) {
		case 1: // Vendor
			usbMsgPtr = (uchar *)(usbDescriptorStringVendorEP);
			len = sizeof(usbDescriptorStringVendorEP);
			break;
		case 2: // Device
			usbMsgPtr = (uchar *)(usbDescriptorStringDeviceEP);
			len = sizeof(usbDescriptorStringDeviceEP);
			break;
		}

		break;

	case USBDESCR_CONFIG:
		usbMsgPtr = (uchar*)usbDescriptorConfigurationEP;
		len = sizeof(usbDescriptorConfigurationEP);
		break;

	case USBDESCR_HID:
		usbMsgPtr = (uchar *)(usbDescriptorConfigurationEP + 18);
		len = usbDescriptorConfigurationEP[18];
		break;

	case USBDESCR_HID_REPORT:
		usbMsgPtr = (uchar*)usbHidReportDescriptorEP;
		len = sizeof(usbHidReportDescriptorEP);
		break;
	}
	break;

    return len;
}

usbMsgLen_t usbFunctionSetup(uchar receivedData[8]) {
	usbRequest_t    *rq = (void *)receivedData;
	uchar reportID = rq->wValue.bytes[0];
	uchar reportType = rq->wValue.bytes[1];

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


    return 0;   /* default for not implemented requests: return no data back to host */
}

static int remainingBytes;

uchar usbFunctionWrite(uchar *receivedData, uchar len) {
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

	return -1;
}