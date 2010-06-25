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

PROGMEM int  usbDescriptorStringVendorDS[] = {
    USB_STRING_DESCRIPTOR_HEADER(USB_CFG_VENDOR_NAME_LEN),
    USB_CFG_VENDOR_NAME
};

PROGMEM int  usbDescriptorStringDeviceDS[] = {
    USB_STRING_DESCRIPTOR_HEADER(USB_CFG_DEVICE_NAME_LEN),
    USB_CFG_DEVICE_NAME
};


// PS3

PROGMEM char usbDescriptorConfigurationPS3[] = {
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
    124, 0,						/* total length of report descriptor */
    7,          				/* sizeof(usbDescrEndpoint) */
    USBDESCR_ENDPOINT,			/* descriptor type = endpoint */
    0x81,						/* IN endpoint number 1 */
    0x03,						/* attrib: Interrupt endpoint */
    8, 0,						/* maximum packet size */
    1, 							/* interrupt poll interval in ms */
};

PROGMEM char usbHidReportDescriptorPS3[124] = {
    0x05, 0x01,                    // USAGE_PAGE (Generic Desktop)
    0x09, 0x04,                    // USAGE (Joystick)
    0xa1, 0x01,                    // COLLECTION (Application)
    0xa1, 0x02,                    //   COLLECTION (Logical)
    0x75, 0x01,                    //     REPORT_SIZE (1)
    0x95, 0x0d,                    //     REPORT_COUNT (13)
    0x15, 0x00,                    //     LOGICAL_MINIMUM (0)
    0x25, 0x01,                    //     LOGICAL_MAXIMUM (1)
    0x35, 0x00,                    //     PHYSICAL_MINIMUM (0)
    0x45, 0x01,                    //     PHYSICAL_MAXIMUM (1)
    0x05, 0x09,                    //     USAGE_PAGE (Button)
    0x19, 0x01,                    //     USAGE_MINIMUM (Button 1)
    0x29, 0x0d,                    //     USAGE_MAXIMUM (Button 13)
    0x81, 0x02,                    //     INPUT (Data,Var,Abs)
    0x95, 0x03,                    //     REPORT_COUNT (3)
    0x81, 0x01,                    //     INPUT (Cnst,Ary,Abs)
    0x05, 0x01,                    //     USAGE_PAGE (Generic Desktop)
    0x75, 0x04,                    //     REPORT_SIZE (4)
    0x95, 0x01,                    //     REPORT_COUNT (1)
    0x25, 0x07,                    //     LOGICAL_MAXIMUM (7)
    0x46, 0x3b, 0x01,              //     PHYSICAL_MAXIMUM (315)
    0x65, 0x14,                    //     UNIT (Eng Rot:Angular Pos)
    0x09, 0x39,                    //     USAGE (Hat switch)
    0x81, 0x42,                    //     INPUT (Data,Var,Abs,Null)
    0x65, 0x00,                    //     UNIT (None)
    0x95, 0x01,                    //     REPORT_COUNT (1)
    0x81, 0x03,                    //     INPUT (Cnst,Var,Abs)
    0xa1, 0x00,                    //     COLLECTION (Physical)
    0x75, 0x02,                    //       REPORT_SIZE (2)
    0x95, 0x04,                    //       REPORT_COUNT (4)
    0x25, 0x02,                    //       LOGICAL_MAXIMUM (2)
    0x45, 0x02,                    //       PHYSICAL_MAXIMUM (2)
    0x09, 0x30,                    //       USAGE (X)
    0x09, 0x31,                    //       USAGE (Y)
    0x09, 0x32,                    //       USAGE (Z)
    0x09, 0x35,                    //       USAGE (Rz)
    0x81, 0x02,                    //       INPUT (Data,Var,Abs)
    0xc0,                          //     END_COLLECTION
    0xc0,                          //   END_COLLECTION
    0x75, 0x08,                    //   REPORT_SIZE (8)
    0x95, 0x08,                    //   REPORT_COUNT (8)
    0x26, 0xff, 0x00,              //   LOGICAL_MAXIMUM (255)
    0x46, 0xff, 0x00,              //   PHYSICAL_MAXIMUM (255)
    0x06, 0x00, 0xff,              //   USAGE_PAGE (Vendor Defined Page 1)
	0x09, 0x20,                    //   USAGE (Unknown)
	0x09, 0x21,                    //   USAGE (Unknown)
	0x09, 0x22,                    //   USAGE (Unknown)
	0x09, 0x23,                    //   USAGE (Unknown) 
	0x09, 0x24,                    //   USAGE (Unknown)
	0x09, 0x25,                    //   USAGE (Unknown)
	0x09, 0x26,                    //   USAGE (Unknown)
	0x09, 0x27,                    //   USAGE (Unknown)
	0x09, 0x28,                    //   USAGE (Unknown)
	0x09, 0x29,                    //   USAGE (Unknown)
	0x09, 0x2a,                    //   USAGE (Unknown)
	0x09, 0x2b,                    //   USAGE (Unknown)
	0x95, 0x0c,                    //   REPORT_COUNT (12)
    0x81, 0x02,                    //   INPUT (Data,Var,Abs)
    0x0a, 0x21, 0x26,              //   UNKNOWN
    0xb1, 0x02,                    //   FEATURE (Data,Var,Abs)
    0xc0                           // END_COLLECTION
};


// PROGRAMMER

PROGMEM char usbDescriptorDeviceHIDBoot[] = {    /* USB device descriptor */
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

PROGMEM int  usbDescriptorStringVendorHIDBoot[] = {
    USB_STRING_DESCRIPTOR_HEADER(8),
    'o', 'b', 'd', 'e', 'v', '.', 'a', 't'
};

PROGMEM int  usbDescriptorStringDeviceHIDBoot[] = {
    USB_STRING_DESCRIPTOR_HEADER(7),
    'H', 'I', 'D', 'B', 'o', 'o', 't'
};

#define EEPROM_SIZE_QUERY_REPORT_ID 3
#define EEPROM_PROGRAMMING_REPORT_ID 4
#define EEPROM_READING_REPORT_ID 5

PROGMEM char usbHidReportDescriptorProgrammer[42] = {
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

    0x85, EEPROM_READING_REPORT_ID,		//   REPORT_ID (EEPROM_READING_REPORT_ID)
    0x95, 0x83,                    		//   REPORT_COUNT (131)
    0x09, 0x00,                    		//   USAGE (Undefined)
    0xb2, 0x02, 0x01,              		//   FEATURE (Data,Var,Abs,Buf)

    0xc0                           		// END_COLLECTION
};

usbMsgLen_t usbFunctionDescriptor(struct usbRequest *rq) {
	usbMsgLen_t len = 0;

	if(usbMode == USB_MODE_PS3) {
	    switch(rq->wValue.bytes[1]) {
	    case USBDESCR_DEVICE:
			usbMsgPtr = (uchar *)(usbDescriptorDeviceDS);
			len = sizeof(usbDescriptorDeviceDS);
			break;

	    case USBDESCR_STRING:
			switch(rq->wValue.bytes[0]) {
			case 1: // Vendor
				usbMsgPtr = (uchar *)(usbDescriptorStringVendorDS);
				break;
			case 2: // Device
				usbMsgPtr = (uchar *)(usbDescriptorStringDeviceDS);
				break;
			}

	        len = usbMsgPtr[0];
			break;

	    case USBDESCR_CONFIG:
			usbMsgPtr = (uchar*)usbDescriptorConfigurationPS3;
			len = sizeof(usbDescriptorConfigurationPS3);

			break;

	    case USBDESCR_HID:
			usbMsgPtr = (uchar *)(usbDescriptorConfigurationPS3 + 18);
			len = usbDescriptorConfigurationPS3[18];

			break;

	    case USBDESCR_HID_REPORT:
			usbMsgPtr = (uchar*)usbHidReportDescriptorPS3;
			len = sizeof(usbHidReportDescriptorPS3);

			break;
	    }
	}
	else if(usbMode == USB_MODE_PROGRAMMER) {
	    switch(rq->wValue.bytes[1]) {
	    case USBDESCR_DEVICE:
	        usbMsgPtr = (uchar *)(usbDescriptorDeviceHIDBoot);
	        len = sizeof(usbDescriptorDeviceHIDBoot);
			break;

	    case USBDESCR_STRING:
	        switch(rq->wValue.bytes[0]) {
	        case 1: // Vendor
		        usbMsgPtr = (uchar *)(usbDescriptorStringVendorHIDBoot);
				break;
	        case 2: // Device
		        usbMsgPtr = (uchar *)(usbDescriptorStringDeviceHIDBoot);
				break;
			}

	        len = usbMsgPtr[0];
			break;

	    case USBDESCR_CONFIG:
			usbMsgPtr = (uchar*)usbDescriptorConfigurationProgrammer;
			len = sizeof(usbDescriptorConfigurationProgrammer);
			break;

	    case USBDESCR_HID:
        	usbMsgPtr = (uchar *)(usbDescriptorConfigurationProgrammer + 18);
			len = usbDescriptorConfigurationProgrammer[18];
			break;

	    case USBDESCR_HID_REPORT:
			usbMsgPtr = (uchar*)usbHidReportDescriptorProgrammer;
			len = sizeof(usbHidReportDescriptorProgrammer);
			break;
	    }
	}

    return len;
}

