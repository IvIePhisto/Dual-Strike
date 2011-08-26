// DUAL STRIKE

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

PROGMEM char usbDescriptorConfiguration[] = {};


// MEMORY TEST

PROGMEM char usbDescriptorDeviceMT[] = {    /* USB device descriptor */
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

PROGMEM int  usbDescriptorStringVendorMT[] = {
    USB_STRING_DESCRIPTOR_HEADER(20),
    'p', 'o', 'h', 'l', '-', 'm', 'i', 'c', 'h', 'a', 'e', 'l', '@', 'g', 'm', 'x', '.', 'b', 'i', 'z'
};

PROGMEM int  usbDescriptorStringDeviceMT[] = {
    USB_STRING_DESCRIPTOR_HEADER(11),
    'M', 'e', 'm', 'o', 'r', 'y', ' ', 'T', 'e', 's', 't'
};

#define EEPROM_SIZE_QUERY_REPORT_ID 1
#define EEPROM_PROGRAMMING_REPORT_ID 2
#define EEPROM_SET_ADDRESS_REPORT_ID 3
#define EEPROM_READING_REPORT_ID 4
#define FLASH_SIZE_QUERY_REPORT_ID 5
#define FLASH_SET_ADDRESS_REPORT_ID 6
#define FLASH_READING_REPORT_ID 7

PROGMEM char usbHidReportDescriptorMT[78] = {
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

    0x85, FLASH_SIZE_QUERY_REPORT_ID,	//   REPORT_ID (FLASH_SIZE_QUERY_REPORT_ID)
    0x95, 0x06,                    		//   REPORT_COUNT (6)
    0x09, 0x00,                    		//   USAGE (Undefined)
    0xb2, 0x02, 0x01,              		//   FEATURE (Data,Var,Abs,Buf)

    0x85, FLASH_SET_ADDRESS_REPORT_ID,	//   REPORT_ID (FLASH_SET_ADDRESS_REPORT_ID)
    0x95, 0x03,                    		//   REPORT_COUNT (3)
    0x09, 0x00,                    		//   USAGE (Undefined)
    0xb2, 0x02, 0x01,              		//   FEATURE (Data,Var,Abs,Buf)

    0x85, FLASH_READING_REPORT_ID,		//   REPORT_ID (FLASH_READING_REPORT_ID)
    0x95, 0x83,                    		//   REPORT_COUNT (131)
    0x09, 0x00,                    		//   USAGE (Undefined)
    0xb2, 0x02, 0x01,              		//   FEATURE (Data,Var,Abs,Buf)

    0xc0                           		// END_COLLECTION
};

PROGMEM char usbDescriptorConfigurationMT[] = {
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
    sizeof(usbHidReportDescriptorMT), 0, /* total length of report descriptor */
    7,          				/* sizeof(usbDescrEndpoint) */
    USBDESCR_ENDPOINT,			/* descriptor type = endpoint */
    0x81,						/* IN endpoint number 1 */
    0x03,						/* attrib: Interrupt endpoint */
    8, 0,						/* maximum packet size */
    200 						/* interrupt poll interval in ms */
};


// GENERIC

usbMsgLen_t usbFunctionDescriptor(struct usbRequest *rq) {
	usbMsgLen_t len = 0;
	switch(rq->wValue.bytes[1]) {
    case USBDESCR_DEVICE:
        usbMsgPtr = (uchar *)(usbDescriptorDeviceMT);
        len = sizeof(usbDescriptorDeviceMT);
		break;

    case USBDESCR_STRING:
        switch(rq->wValue.bytes[0]) {
        case 1: // Vendor
	        usbMsgPtr = (uchar *)(usbDescriptorStringVendorMT);
			len = sizeof(usbDescriptorStringVendorMT);
			break;
        case 2: // Device
	        usbMsgPtr = (uchar *)(usbDescriptorStringDeviceMT);
			len = sizeof(usbDescriptorStringDeviceMT);
			break;
		}

		break;

    case USBDESCR_CONFIG:
		usbMsgPtr = (uchar*)usbDescriptorConfigurationMT;
		len = sizeof(usbDescriptorConfigurationMT);
		break;

    case USBDESCR_HID:
    	usbMsgPtr = (uchar *)(usbDescriptorConfigurationMT + 18);
		len = usbDescriptorConfigurationMT[18];
		break;

    case USBDESCR_HID_REPORT:
		usbMsgPtr = (uchar*)usbHidReportDescriptorMT;
		len = sizeof(usbHidReportDescriptorMT);
		break;
    }

    return len;
}

