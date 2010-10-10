
#define USB_CFG_FTDI_VENDOR_ID 0x03, 0x04


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

// PS3

PROGMEM int  usbDescriptorStringDevicePS3[] = {
    USB_STRING_DESCRIPTOR_HEADER(USB_CFG_DEVICE_NAME_LEN + 6),
    USB_CFG_DEVICE_NAME, ' ', '(', 'P', 'S', '3', ')'
};

PROGMEM char usbHidReportDescriptorPS3[124/*+6*/] = {
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
	/*
	//---
	0x95, 0x23,                    //   REPORT_COUNT (35)
	0x09, 0x01,					   //   USAGE(POINTER)
	0x91, 0x02,					   //	OUTPUT (Data,Var,Abs)
	//---
	*/
    0x0a, 0x21, 0x26,              //   UNKNOWN
    0xb1, 0x02,                    //   FEATURE (Data,Var,Abs)
    0xc0                           // END_COLLECTION
};


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
    sizeof(usbHidReportDescriptorPS3), 0, /* total length of report descriptor */
    7,          				/* sizeof(usbDescrEndpoint) */
    USBDESCR_ENDPOINT,			/* descriptor type = endpoint */
    0x82,						/* IN endpoint number 2 */
    0x03,						/* attrib: Interrupt endpoint */
    16, 0,						/* maximum packet size */
    1 							/* interrupt poll interval in ms */
};

// PC

PROGMEM int  usbDescriptorStringDevicePC[] = {
    USB_STRING_DESCRIPTOR_HEADER(USB_CFG_DEVICE_NAME_LEN + 5),
    USB_CFG_DEVICE_NAME, ' ', '(', 'P', 'C', ')'
};

PROGMEM char usbHidReportDescriptorPC[56] = {
    0x05, 0x01,                    // USAGE_PAGE (Generic Desktop)
    0x09, 0x05,                    // USAGE (Game Pad)
    0xa1, 0x01,                    // COLLECTION (Application)
    0x15, 0x00,                    //   LOGICAL_MINIMUM (0)
    0x25, 0x01,                    //   LOGICAL_MAXIMUM (1)
    0x35, 0x00,                    //   PHYSICAL_MINIMUM (0)
    0x45, 0x01,                    //   PHYSICAL_MAXIMUM (1)
    0x75, 0x01,                    //   REPORT_SIZE (1)
    0x95, 0x0d,                    //   REPORT_COUNT (13)
    0x05, 0x09,                    //   USAGE_PAGE (Button)
    0x19, 0x01,                    //   USAGE_MINIMUM (Button 1)
    0x29, 0x0d,                    //   USAGE_MAXIMUM (Button 13)
    0x81, 0x02,                    //   INPUT (Data,Var,Abs)
    0x05, 0x01,                    //   USAGE_PAGE (Generic Desktop)
    0x09, 0x00,                    //   USAGE (Undefined)
    0x95, 0x03,                    //   REPORT_COUNT (3)
    0x81, 0x01,                    //   INPUT (Cnst,Ary,Abs)
    0x25, 0x07,                    //   LOGICAL_MAXIMUM (7)
    0x46, 0x3b, 0x01,              //   PHYSICAL_MAXIMUM (315)
    0x75, 0x04,                    //   REPORT_SIZE (4)
    0x95, 0x01,                    //   REPORT_COUNT (1)
    0x65, 0x14,                    //   UNIT (Eng Rot:Angular Pos)
    0x09, 0x39,                    //   USAGE (Hat switch)
    0x81, 0x42,                    //   INPUT (Data,Var,Abs,Null)
    0x09, 0x00,                    //   USAGE (Undefined)
    0x95, 0x01,                    //   REPORT_COUNT (1)
    0x81, 0x01,                    //   INPUT (Cnst,Ary,Abs)
    0xc0                           // END_COLLECTION
};

PROGMEM char usbDescriptorConfigurationPC[] = {
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
    sizeof(usbHidReportDescriptorPC), 0, /* total length of report descriptor */
    7,          				/* sizeof(usbDescrEndpoint) */
    USBDESCR_ENDPOINT,			/* descriptor type = endpoint */
    0x81,						/* IN endpoint number 1 */
    0x03,						/* attrib: Interrupt endpoint */
    8, 0,						/* maximum packet size */
    1 							/* interrupt poll interval in ms */
};

// EEPROM PROGRAMMER

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

#if ATMEGA_NO == 168
// MAME

PROGMEM char usbDescriptorDeviceMAME[] = {    /* USB device descriptor */
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
    USB_CFG_FTDI_VENDOR_ID,	/* 2 bytes */
    0x28, 0x8E,				/* product ID (2 bytes) */
    USB_CFG_DEVICE_VERSION, /* 2 bytes */
    1,          			/* manufacturer string index */
    2,         				/* product string index */
    0,          			/* serial number string index */
    1                       /* number of configurations */
};

PROGMEM int  usbDescriptorStringDeviceMAME[] = {
    USB_STRING_DESCRIPTOR_HEADER(USB_CFG_DEVICE_NAME_LEN + 7),
    USB_CFG_DEVICE_NAME, ' ', '(', 'M', 'A', 'M', 'E', ')'
};


PROGMEM char usbHidReportDescriptorMAME[110] = {
    0x05, 0x01,                    // USAGE_PAGE (Generic Desktop)
    0x09, 0x06,                    // USAGE (Keyboard)
    0x15, 0x00,                    // LOGICAL_MINIMUM (0)
    0x25, 0x01,                    // LOGICAL_MAXIMUM (1)
    0x35, 0x00,                    // PHYSICAL_MINIMUM (0)
    0x45, 0x01,                    // PHYSICAL_MAXIMUM (1)
    0x75, 0x01,                    // REPORT_SIZE (1)
    0xa1, 0x01,                    // COLLECTION (Application)
    0x85, 0x01,                    //   REPORT_ID (1)
    0x05, 0x07,                    //   USAGE_PAGE (Keyboard)
    0x09, 0x52,                    //   USAGE (Keyboard UpArrow)
    0x09, 0x4f,                    //   USAGE (Keyboard RightArrow)
    0x09, 0x51,                    //   USAGE (Keyboard DownArrow)
    0x09, 0x50,                    //   USAGE (Keyboard LeftArrow)
    0x09, 0xe0,                    //   USAGE (Keyboard LeftControl)
    0x09, 0xe2,                    //   USAGE (Keyboard LeftAlt)
    0x09, 0xe1,                    //   USAGE (Keyboard LeftShift)
    0x09, 0x28,                    //   USAGE (Keyboard Return (ENTER))
    0x09, 0x29,                    //   USAGE (Keyboard ESCAPE)
    0x09, 0x2c,                    //   USAGE (Keyboard Spacebar)
    0x09, 0x2b,                    //   USAGE (Keyboard Tab)
    0x09, 0x3b,                    //   USAGE (Keyboard F2)
    0x09, 0x3c,                    //   USAGE (Keyboard F3)
    0x09, 0x04,                    //   USAGE (Keyboard a and A)
    0x09, 0x07,                    //   USAGE (Keyboard d and D)
    0x09, 0x09,                    //   USAGE (Keyboard f and F)
    0x09, 0x0a,                    //   USAGE (Keyboard g and G)
    0x09, 0x0c,                    //   USAGE (Keyboard i and I)
    0x09, 0x0e,                    //   USAGE (Keyboard k and K)
    0x09, 0x13,                    //   USAGE (Keyboard p and P)
    0x09, 0x14,                    //   USAGE (Keyboard q and Q)
    0x09, 0x15,                    //   USAGE (Keyboard r and R)
    0x09, 0x16,                    //   USAGE (Keyboard s and S)
    0x09, 0x1a,                    //   USAGE (Keyboard w and W)
    0x09, 0x1b,                    //   USAGE (Keyboard x and X)
    0x09, 0x1d,                    //   USAGE (Keyboard z and Z)
    0x09, 0x1e,                    //   USAGE (Keyboard 1 and !)
    0x09, 0x1f,                    //   USAGE (Keyboard 2 and @)
    0x09, 0x22,                    //   USAGE (Keyboard 5 and %)
    0x09, 0x23,                    //   USAGE (Keyboard 6 and ^)
    0x09, 0x26,                    //   USAGE (Keyboard 9 and ()
    0x09, 0x01,                    //   USAGE (Keyboard ErrorRollOver)
    0x95, 0x20,                    //   REPORT_COUNT (32)
    0x81, 0x02,                    //   INPUT (Data,Var,Abs)
    0xc0,                          // END_COLLECTION
    0x05, 0x0c,                    // USAGE_PAGE (Consumer Devices)
    0x09, 0x01,                    // USAGE (Consumer Control)
    0xa1, 0x01,                    // COLLECTION (Application)
    0x85, 0x02,                    //   REPORT_ID (2)
    0x09, 0xe9,                    //   USAGE (Volume Up)
    0x09, 0xea,                    //   USAGE (Volume Down)
    0x95, 0x02,                    //   REPORT_COUNT (2)
    0x81, 0x02,                    //   INPUT (Data,Var,Abs)
    0x95, 0x06,                    //   REPORT_COUNT (6)
    0x81, 0x01,                    //   INPUT (Cnst,Ary,Abs)
    0xc0                           // END_COLLECTION
};


PROGMEM char usbDescriptorConfigurationMAME[] = {
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
    sizeof(usbHidReportDescriptorMAME), 0, /* total length of report descriptor */
    7,          				/* sizeof(usbDescrEndpoint) */
    USBDESCR_ENDPOINT,			/* descriptor type = endpoint */
    0x81,						/* IN endpoint number 1 */
    0x03,						/* attrib: Interrupt endpoint */
    8, 0,						/* maximum packet size */
    4 							/* interrupt poll interval in ms */
};

// XBOX

PROGMEM char usbDescriptorDeviceXBox[] = {    /* USB device descriptor */
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
    USB_CFG_FTDI_VENDOR_ID,	/* 2 bytes */
    0x29, 0x8E,				/* product ID (2 bytes) */
    USB_CFG_DEVICE_VERSION, /* 2 bytes */
    1,          /* manufacturer string index */
    2,          /* product string index */
    0,          /* serial number string index */
    1,          /* number of configurations */
};

PROGMEM int  usbDescriptorStringDeviceXBox[] = {
    USB_STRING_DESCRIPTOR_HEADER(USB_CFG_DEVICE_NAME_LEN + 7),
    USB_CFG_DEVICE_NAME, ' ', '(', 'X', 'B', 'o', 'x', ')'
};

PROGMEM const unsigned char usbDescriptorConfigurationXBox[] = {
    9,          				// sizeof(usbDescriptorConfiguration): length of descriptor in bytes 
    USBDESCR_CONFIG,   	 		// descriptor type 
    32, 0,      				// total length of data returned (including inlined descriptors) 
    1,          				// number of interfaces in this configuration 
    1,          				// index of this configuration 
    0,          				// configuration name string index

	0x80, 						//attributes:Default
	125,						//power consumption * 2mA
	/*
    (char)USBATTR_BUSPOWER, 	// attributes 
    USB_CFG_MAX_BUS_POWER/2,	// max USB current in 2mA units 
	*/
// interface descriptor follows inline: 
    9,   			 	    	// sizeof(usbDescrInterface): length of descriptor in bytes 
    USBDESCR_INTERFACE,			// descriptor type 
    0,          				// index of this interface 
    0,							// alternate setting for this interface 
    2, 							// endpoints excl 0: number of endpoint descriptors to follow  | ORIGINALLY: 2
    0x58,						// USB interface class: Unknown 
    0x42,						// USB interface subclass 
    0,							// USB interface protocol 
    0,          				// string index for interface 
    7,          				// sizeof(usbDescrEndpoint): length of descriptor in bytes 
    USBDESCR_ENDPOINT,			// descriptor type = endpoint 
    0x82,						// IN endpoint number 2
    //0x00,						// attrib: Control endpoint 
    //0x01,						// attrib: Isochronous endpoint 
    //0x02,						// attrib: Bulk endpoint 
    0x03,						// attrib: Interrupt endpoint 
    32, 0,						// maximum packet size, originally 32
    4, 							// interrupt poll interval in ms 
    7,          				// sizeof(usbDescrEndpoint): length of descriptor in bytes 
    USBDESCR_ENDPOINT,			// descriptor type = endpoint 
    0x02,						// OUT endpoint number 2
    0x03,						// attrib: Interrupt endpoint 
    8, 0,						// maximum packet size, originally 32
    4 							// interrupt poll interval in ms
};
#endif


// GENERIC

usbMsgLen_t usbFunctionDescriptor(struct usbRequest *rq) {
	usbMsgLen_t len = 0;

	switch(usbMode) {
	case USB_MODE_PS3:
	    switch(rq->wValue.bytes[1]) {
	    case USBDESCR_DEVICE:
			usbMsgPtr = (uchar *)(usbDescriptorDeviceDS);
			len = sizeof(usbDescriptorDeviceDS);
			break;

	    case USBDESCR_STRING:
			switch(rq->wValue.bytes[0]) {
			case 1: // Vendor
				usbMsgPtr = (uchar *)(usbDescriptorStringVendorDS);
				len = sizeof(usbDescriptorStringVendorDS);
				break;
			case 2: // Device
				usbMsgPtr = (uchar *)(usbDescriptorStringDevicePS3);
				len = sizeof(usbDescriptorStringDevicePS3);
				break;
			}
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
		break;

	case USB_MODE_PC:
	    switch(rq->wValue.bytes[1]) {
	    case USBDESCR_DEVICE:
			usbMsgPtr = (uchar *)(usbDescriptorDeviceDS);
			len = sizeof(usbDescriptorDeviceDS);
			break;

	    case USBDESCR_STRING:
			switch(rq->wValue.bytes[0]) {
			case 1: // Vendor
				usbMsgPtr = (uchar *)(usbDescriptorStringVendorDS);
				len = sizeof(usbDescriptorStringVendorDS);
				break;
			case 2: // Device
				usbMsgPtr = (uchar *)(usbDescriptorStringDevicePC);
				len = sizeof(usbDescriptorStringDevicePC);
				break;
			}
			break;

	    case USBDESCR_CONFIG:
			usbMsgPtr = (uchar*)usbDescriptorConfigurationPC;
			len = sizeof(usbDescriptorConfigurationPC);
			break;

	    case USBDESCR_HID:
			usbMsgPtr = (uchar *)(usbDescriptorConfigurationPC + 18);
			len = usbDescriptorConfigurationPC[18];
			break;

	    case USBDESCR_HID_REPORT:
			usbMsgPtr = (uchar*)usbHidReportDescriptorPC;
			len = sizeof(usbHidReportDescriptorPC);
			break;
		}
		break;

	case USB_MODE_PROGRAMMER:
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

#if ATMEGA_NO == 168
		break;

	case USB_MODE_MAME:
	    switch(rq->wValue.bytes[1]) {
	    case USBDESCR_DEVICE:
			usbMsgPtr = (uchar *)(usbDescriptorDeviceMAME);
			len = sizeof(usbDescriptorDeviceMAME);
			break;

	    case USBDESCR_STRING:
			switch(rq->wValue.bytes[0]) {
			case 1: // Vendor
				usbMsgPtr = (uchar *)(usbDescriptorStringVendorDS);
				len = sizeof(usbDescriptorStringVendorDS);
				break;
			case 2: // Device
				usbMsgPtr = (uchar *)(usbDescriptorStringDeviceMAME);
				len = sizeof(usbDescriptorStringDeviceMAME);
				break;
			}
			break;

	    case USBDESCR_CONFIG:
			usbMsgPtr = (uchar*)usbDescriptorConfigurationMAME;
			len = sizeof(usbDescriptorConfigurationMAME);
			break;

	    case USBDESCR_HID:
			usbMsgPtr = (uchar *)(usbDescriptorConfigurationMAME + 18);
			len = usbDescriptorConfigurationMAME[18];
			break;

	    case USBDESCR_HID_REPORT:
			usbMsgPtr = (uchar*)usbHidReportDescriptorMAME;
			len = sizeof(usbHidReportDescriptorMAME);
			break;
	    }
		break;

	case USB_MODE_XBOX:
	    switch(rq->wValue.bytes[1]) {
	    case USBDESCR_DEVICE:
			usbMsgPtr = (uchar *)(usbDescriptorDeviceXBox);
			len = sizeof(usbDescriptorDeviceXBox);
			break;

	    case USBDESCR_STRING:
			switch(rq->wValue.bytes[0]) {
			case 1: // Vendor
				usbMsgPtr = (uchar *)(usbDescriptorStringVendorDS);
				len = sizeof(usbDescriptorStringVendorDS);
				break;
			case 2: // Device
				usbMsgPtr = (uchar *)(usbDescriptorStringDeviceXBox);
				len = sizeof(usbDescriptorStringDeviceXBox);
				break;
			}
			break;

	    case USBDESCR_CONFIG:
			usbMsgPtr = (uchar*)usbDescriptorConfigurationXBox;
			len = sizeof(usbDescriptorConfigurationXBox);
			break;
		}
#endif
	}

    return len;
}

