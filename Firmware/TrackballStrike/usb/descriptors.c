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


// Trackball Controller

PROGMEM char usbHidReportDescriptorTrackball[50] = {
    0x05, 0x01,                    // USAGE_PAGE (Generic Desktop)
    0x09, 0x02,                    // USAGE (Mouse)
    0xa1, 0x01,                    // COLLECTION (Application)
    0x09, 0x01,                    //   USAGE (Pointer)
    0xa1, 0x00,                    //   COLLECTION (Physical)
    0x05, 0x09,                    //     USAGE_PAGE (Button)
    0x19, 0x01,                    //     USAGE_MINIMUM (Button 1)
    0x29, 0x03,                    //     USAGE_MAXIMUM (Button 3)
    0x15, 0x00,                    //     LOGICAL_MINIMUM (0)
    0x25, 0x01,                    //     LOGICAL_MAXIMUM (1)
    0x95, 0x03,                    //     REPORT_COUNT (3)
    0x75, 0x01,                    //     REPORT_SIZE (1)
    0x81, 0x02,                    //     INPUT (Data,Var,Abs)
    0x95, 0x01,                    //     REPORT_COUNT (1)
    0x75, 0x05,                    //     REPORT_SIZE (5)
    0x81, 0x03,                    //     INPUT (Cnst,Var,Abs)
    0x05, 0x01,                    //     USAGE_PAGE (Generic Desktop)
    0x09, 0x30,                    //     USAGE (X)
    0x09, 0x31,                    //     USAGE (Y)
    0x15, 0x81,                    //     LOGICAL_MINIMUM (-127)
    0x25, 0x7f,                    //     LOGICAL_MAXIMUM (127)
    0x75, 0x08,                    //     REPORT_SIZE (8)
    0x95, 0x02,                    //     REPORT_COUNT (2)
    0x81, 0x06,                    //     INPUT (Data,Var,Rel)
    0xc0,                          //   END_COLLECTION
    0xc0                           // END_COLLECTION
};

PROGMEM char usbDescriptorConfigurationTrackball[] = {
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
    sizeof(usbHidReportDescriptorTrackball), 0, /* total length of report descriptor */
    7,          				/* sizeof(usbDescrEndpoint) */
    USBDESCR_ENDPOINT,			/* descriptor type = endpoint */
    0x81,						/* IN endpoint number 1 */
    0x03,						/* attrib: Interrupt endpoint */
    8, 0,						/* maximum packet size */
    1 							/* interrupt poll interval in ms */
};

usbMsgLen_t usbFunctionDescriptor(struct usbRequest *rq) {
	usbMsgLen_t len = 0;

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
			usbMsgPtr = (uchar *)(usbDescriptorStringDeviceDS);
			len = sizeof(usbDescriptorStringDeviceDS);
			break;
		}

		break;

    case USBDESCR_CONFIG:
		usbMsgPtr = (uchar*)usbDescriptorConfigurationTrackball;
		len = sizeof(usbDescriptorConfigurationTrackball);

		break;

    case USBDESCR_HID:
		usbMsgPtr = (uchar *)(usbDescriptorConfigurationTrackball + 18);
		len = usbDescriptorConfigurationTrackball[18];

		break;

    case USBDESCR_HID_REPORT:
		usbMsgPtr = (uchar*)usbHidReportDescriptorTrackball;
		len = sizeof(usbHidReportDescriptorTrackball);

		break;
    }

    return len;
}

