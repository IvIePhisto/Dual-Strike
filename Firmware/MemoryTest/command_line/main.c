/* Name: main.c
 * Project: AVR bootloader HID
 * Author: Christian Starkjohann
 * Creation Date: 2007-03-19
 * Tabsize: 4
 * Copyright: (c) 2007 by OBJECTIVE DEVELOPMENT Software GmbH
 * License: Proprietary, free under certain conditions. See Documentation.
 * This Revision: $Id: main.c 373 2007-07-04 08:59:36Z cs $
 */
 
/*
 * Changes by Michael Pohl for the MCCF project.
 */

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <errno.h>
#include "usbcalls.h"

#define IDENT_VENDOR_NUM        0x16c0
#define IDENT_VENDOR_STRING     "pohl-michael@gmx.biz"
#define IDENT_PRODUCT_NUM       0x05df
#define IDENT_PRODUCT_STRING    "MemoryTool"

/* ------------------------------------------------------------------------- */

static char dataBuffer[65536 + 256];    /* buffer for file data */
static int  startAddress, endAddress;

/* ------------------------------------------------------------------------- */

static int  parseUntilColon(FILE *fp)
{
int c;

    do{
        c = getc(fp);
    }while(c != ':' && c != EOF);
    return c;
}

static int  parseHex(FILE *fp, int numDigits)
{
int     i;
char    temp[9];

    for(i = 0; i < numDigits; i++)
        temp[i] = getc(fp);
    temp[i] = 0;
    return strtol(temp, NULL, 16);
}

/* ------------------------------------------------------------------------- */

static int  parseIntelHex(char *hexfile, char buffer[65536 + 256], int *startAddr, int *endAddr)
{
int     address, base, d, segment, i, lineLen, sum;
FILE    *input;

    input = fopen(hexfile, "r");
    if(input == NULL){
        fprintf(stderr, "error opening %s: %s\n", hexfile, strerror(errno));
        return 1;
    }
    while(parseUntilColon(input) == ':'){
        sum = 0;
        sum += lineLen = parseHex(input, 2);
        base = address = parseHex(input, 4);
        sum += address >> 8;
        sum += address;
        sum += segment = parseHex(input, 2);  /* segment value? */
        if(segment != 0)    /* ignore lines where this byte is not 0 */
            continue;
        for(i = 0; i < lineLen ; i++){
            d = parseHex(input, 2);
            buffer[address++] = d;
            sum += d;
        }
        sum += parseHex(input, 2);
        if((sum & 0xff) != 0){
            fprintf(stderr, "Warning: Checksum error between address 0x%x and 0x%x\n", base, address);
        }
        if(*startAddr > base)
            *startAddr = base;
        if(*endAddr < address)
            *endAddr = address;
    }
    fclose(input);
    return 0;
}

/* ------------------------------------------------------------------------- */

char    *usbErrorMessage(int errCode)
{
static char buffer[80];

    switch(errCode){
        case USB_ERROR_ACCESS:      return "Access to device denied";
        case USB_ERROR_NOTFOUND:    return "The specified device was not found";
        case USB_ERROR_BUSY:        return "The device is used by another application";
        case USB_ERROR_IO:          return "Communication error with device";
        default:
            sprintf(buffer, "Unknown USB error %d", errCode);
            return buffer;
    }
    return NULL;    /* not reached */
}

static int  getUsbInt(char *buffer, int numBytes)
{
int shift = 0, value = 0, i;

    for(i = 0; i < numBytes; i++){
        value |= ((int)*buffer & 0xff) << shift;
        shift += 8;
        buffer++;
    }
    return value;
}

static void setUsbInt(char *buffer, int value, int numBytes)
{
int i;

    for(i = 0; i < numBytes; i++){
        *buffer++ = value;
        value >>= 8;
    }
}

/* ------------------------------------------------------------------------- */

typedef struct deviceInfo {
    char    reportId;
    char    pageSize[2];
    char    memorySize[4];
}deviceInfo_t;

typedef struct {
    char    reportId;
    char    address[3];
	char	length;
    char    data[128];
}e2Data_t;

typedef struct {
    char    reportId;
    char    address[3];
}setAddressData_t;

typedef struct {
    char    reportId;
    unsigned char    length;
	char	unused[2];
    char    data[128];
}dumpData_t;

usbDevice_t *dev = NULL;

union {
    char            bytes[1];
	deviceInfo_t    info;
	e2Data_t		e2data;
	setAddressData_t setAddress;
	dumpData_t		dump;
} buffer;

int err = 0;
int len = sizeof(buffer);

int pageSize, deviceSize = 0;

#define EEPROM_SIZE_QUERY_REPORT_ID 1
#define EEPROM_PROGRAMMING_REPORT_ID 2
#define EEPROM_SET_ADDRESS_REPORT_ID 3
#define EEPROM_READING_REPORT_ID 4
#define FLASH_SIZE_QUERY_REPORT_ID 5
#define FLASH_SET_ADDRESS_REPORT_ID 6
#define FLASH_READING_REPORT_ID 7

static int readSizes(int reportID, int deviceSizeCorrection) {
    if((err = usbGetReport(dev, USB_HID_REPORT_TYPE_FEATURE, reportID, buffer.bytes, &len)) != 0) {
        fprintf(stderr, "Error reading sizes: %s\n", usbErrorMessage(err));			
		return err;
	}

    pageSize = getUsbInt(buffer.info.pageSize, 2);
    deviceSize = getUsbInt(buffer.info.memorySize, 4);
    printf("Page size   = %d (0x%x)\n", pageSize, pageSize);
    printf("Device size = %d (0x%x)\n", deviceSize, deviceSize + deviceSizeCorrection);
	
	return 0;
}

static int uploadEEPROMData(char *dataBuffer, int startAddr, int endAddr) {
	err = 0;
	printf("Dumping EEPROM data...\n");

	if(readSizes(EEPROM_SIZE_QUERY_REPORT_ID, 0) != 0) {
		if(dev != NULL)
			usbCloseDevice(dev);

		return 1;
	}
	
    if(len < sizeof(buffer.info)) {
        fprintf(stderr, "Not enough bytes in EEPROM for data (%d instead of %d)\n", len, (int)sizeof(buffer.info));
        err = -1;
        goto errorOccurred;
    }	
	
    if(endAddr > deviceSize) {
        fprintf(stderr, "Data (%d bytes) exceeds remaining EEPROM size!\n", endAddr);
        err = -1;
        goto errorOccurred;
    }
		
    printf("Uploading %d (0x%x) bytes starting at %d (0x%x)\n", endAddr - startAddr, endAddr - startAddr, startAddr, startAddr);
	
    while(startAddr < endAddr){
		int length = 128;
		
		if(endAddr - startAddr < 128)
			length = endAddr - startAddr;
			
        buffer.e2data.reportId = 2;
        memcpy(buffer.e2data.data, dataBuffer + startAddr, length);
        setUsbInt(buffer.e2data.address, startAddr, 3);
		buffer.e2data.length = length;
        printf("\r0x%05x ... 0x%05x", startAddr, startAddr + length);
        fflush(stdout);
        if((err = usbSetReport(dev, USB_HID_REPORT_TYPE_FEATURE, buffer.bytes, sizeof(buffer.e2data))) != 0){
            fprintf(stderr, "\nError uploading data block: %s\n", usbErrorMessage(err));
            goto errorOccurred;
        }
        startAddr += length;
    }
	
    printf("\n");
	return 0;

errorOccurred:
    if(dev != NULL)
        usbCloseDevice(dev);
    return err;
}

static int  dumpEEPROMData(char *dataBuffer, int dataBufferSize) {
	int address = 0;
	
	err = 0;
	printf("Dumping EEPROM data...\n");

	if(pageSize == 0 && deviceSize == 0)
		if(readSizes(EEPROM_SIZE_QUERY_REPORT_ID, 0) != 0) {
			if(dev != NULL)
				usbCloseDevice(dev);

			return 1;
		}
	
    if(deviceSize > dataBufferSize) {
        fprintf(stderr, "Not enough bytes in data buffer for EEPROM data (%d instead of %d)\n", dataBufferSize, deviceSize);
        err = -1;
        goto errorOccurred;
    }	
		
    printf("Dumping %d (0x%x) bytes starting at %d (0x%x)\n", deviceSize, deviceSize, 0, 0);
	
    while(address < deviceSize) {
		unsigned int length;
		
		buffer.setAddress.reportId = EEPROM_SET_ADDRESS_REPORT_ID;
        setUsbInt(buffer.setAddress.address, address, 3);

        if((err = usbSetReport(dev, USB_HID_REPORT_TYPE_FEATURE, buffer.bytes, 4)) != 0){
            fprintf(stderr, "\nError setting address: %s\n", usbErrorMessage(err));
            goto errorOccurred;
        }

		if((err = usbGetReport(dev, USB_HID_REPORT_TYPE_FEATURE, EEPROM_READING_REPORT_ID, buffer.bytes, &len)) != 0) {		
            fprintf(stderr, "\nError receiving dumped EEPROM data: %s\n", usbErrorMessage(err));
            goto errorOccurred;
		}

		length = buffer.dump.length;
        printf("\r0x%05x ... 0x%05x", address, address + length);
        fflush(stdout);
		
        memcpy(dataBuffer + address, buffer.dump.data, length);
        address += length;
    }
    printf("\n");
	
	return address;
	
errorOccurred:
    if(dev != NULL)
        usbCloseDevice(dev);
		
    return 0;
}

static int  dumpFlashData(char *dataBuffer, int dataBufferSize) {
	int address = 0;
	
	err = 0;
	printf("Dumping flash data...\n");

	if(pageSize == 0 && deviceSize == 0)
		if(readSizes(FLASH_SIZE_QUERY_REPORT_ID, 0) != 0) {
			if(dev != NULL)
				usbCloseDevice(dev);

			return 1;
		}
	
    if(deviceSize > dataBufferSize) {
        fprintf(stderr, "Not enough bytes in data buffer for flash data (%d instead of %d)\n", dataBufferSize, deviceSize);
        err = -1;
        goto errorOccurred;
    }	
		
    printf("Dumping %d (0x%x) bytes starting at %d (0x%x)\n", deviceSize, deviceSize, 0, 0);
	
    while(address < deviceSize) {
		unsigned int length;
		
		buffer.setAddress.reportId = FLASH_SET_ADDRESS_REPORT_ID;
        setUsbInt(buffer.setAddress.address, address, 3);

        if((err = usbSetReport(dev, USB_HID_REPORT_TYPE_FEATURE, buffer.bytes, 4)) != 0){
            fprintf(stderr, "\nError setting address: %s\n", usbErrorMessage(err));
            goto errorOccurred;
        }

		if((err = usbGetReport(dev, USB_HID_REPORT_TYPE_FEATURE, FLASH_READING_REPORT_ID, buffer.bytes, &len)) != 0) {		
            fprintf(stderr, "\nError receiving dumped flash data: %s\n", usbErrorMessage(err));
            goto errorOccurred;
		}

		length = buffer.dump.length;
        printf("\r0x%05x ... 0x%05x", address, address + length);
        fflush(stdout);
		
        memcpy(dataBuffer + address, buffer.dump.data, length);
        address += length;
    }
    printf("\n");
	
	return address;
	
errorOccurred:
    if(dev != NULL)
        usbCloseDevice(dev);
		
    return 0;
}

int writePlainHEX(char* plainHexFile, char* dataBuffer, int endAddress) {
	FILE* output;
	int address;
	
    output = fopen(plainHexFile, "w");
	
	if(output == NULL) {
        fprintf(stderr, "Error opening file \"%s\" for writing: %s\n", plainHexFile, strerror(errno));
        return 1;
	}
	
	for(address = 0; address < endAddress; address++) {
		if(address > 0 && (address % 8 == 0))
			fprintf(output, "\n");

		fprintf(output, "%02X ", dataBuffer[address] & 0xFF);
	}
	
    fflush(output);
	fclose(output);
	
	return 0;
}

static int  parseHexByte(FILE *fp) {
	int hexPairPosition = 0;
	char hexPair[3];
	char c = EOF;
	
    for(c = getc(fp); c != EOF && hexPairPosition < 2; c = getc(fp)) {
		if(c != ' ' && c != '\n' && c != '\r' && c != '\t') {
			hexPair[hexPairPosition] = c;
			hexPairPosition++;
		}
    }
	
	if(c == EOF)
		return -1;
		
	hexPair[hexPairPosition] = 0;

    return strtol(hexPair, NULL, 16);
}

int comparePlainHEXFiles(char* compareFileA, char* compareFileB) {
	FILE* fileA = fopen(compareFileA, "r");
	FILE* fileB = fopen(compareFileB, "r");
	int address = 0;
	int mismatchFound = 0;

	if(fileA == NULL) {
        fprintf(stderr, "Error opening file \"%s\" for comparison: %s\n", compareFileA, strerror(errno));
        return 1;
	}

	if(fileB == NULL) {
        fprintf(stderr, "Error opening file \"%s\" for comparison: %s\n", compareFileB, strerror(errno));
        return 1;
	}
	
	while(1) {
		int valueA = parseHexByte(fileA);
		int valueB = parseHexByte(fileB);
		
		if(valueA == -1 && valueB != -1) {
			fprintf(stdout, "The first file ends at address %04X.\n", address - 1);
			break;
		}
		else if(valueA != -1 && valueB == -1) {
			fprintf(stdout, "The second file ends at address %04X.\n", address - 1);
			break;
		}
		else if(valueA == -1 && valueB == -1) {
			break;
		}
		else if(valueA != valueB) {
			if(!mismatchFound) {
				mismatchFound = 1;
				fprintf(stdout, "Mismatches (<address>: <value A> <value B>):\n");			
			}
			
			fprintf(stdout, "%04X: %02X %02X\n", address, valueA, valueB);			
		}
		
		address++;
	}
	
	fclose(fileA);
	fclose(fileB);
	
	return 0;
}

/* ------------------------------------------------------------------------- */

static void printUsage(char *pname)
{
    fprintf(stderr,	"usage: %s ([-h|--help] | [-e <intel-hexfile>] [-de <plain-hexfile>] [-df <plain-hexfile>] [-c <plain-hexfile> <plain-hexfile>])\n", pname);
	fprintf(stderr,	" \"-h\" or \"--help\" prints this message.\n");
	fprintf(stderr,	" \"-e <intel-hexfile>\" specifies a file to write to the EEPROM.\n");
	fprintf(stderr,	" \"-de <plain-hexfile>\" specifies a file to dump the EEPROM to.\n");
	fprintf(stderr,	" \"-df <plain-hexfile>\" specifies a file to dump the flash to.\n");
	fprintf(stderr,	" \"-c <plain-hexfile> <plain-hexfile>\" compares the given files.\n");
	fprintf(stderr,	"At least one of the arguments must be used.\nIf no argument is given device availability is tested.\n");
}

static int testArgs(int argc, char **argv) {
	int currentIndex;
	
	if(argc > 10)
		return 1;

	if(argc == 1)
		return 0;

	currentIndex = 1;		
	
	while(currentIndex < argc) {
		switch(currentIndex) {
		case 1:
			if(strcmp(argv[currentIndex], "-h") != 0
			&& strcmp(argv[currentIndex], "--help") != 0
			&& strcmp(argv[currentIndex], "-e") != 0
			&& strcmp(argv[currentIndex], "-de") != 0
			&& strcmp(argv[currentIndex], "-df") != 0
			&& strcmp(argv[currentIndex], "-c") != 0)
				return 1;
			break;
		case 3:
			if(strcmp(argv[currentIndex], "-de") != 0
			&& strcmp(argv[currentIndex], "-df") != 0
			&& strcmp(argv[currentIndex], "-c") != 0)
				return 1;
			break;
			
		case 5:
			if(strcmp(argv[currentIndex], "-df") != 0
			&& strcmp(argv[currentIndex], "-c") != 0)
				return 1;
			break;
			
		case 7:
			if(strcmp(argv[currentIndex], "-df") != 0)
				return 1;
			break;

		default:
			return 1;
		}

		if(strcmp(argv[currentIndex], "-h") == 0
		|| strcmp(argv[currentIndex], "--help") == 0) {
			if(argc == 2)
				return 0;
			else
				return 1;
		}

		if(strcmp(argv[currentIndex], "-e") == 0
		|| strcmp(argv[currentIndex], "-de") == 0
		|| strcmp(argv[currentIndex], "-df") == 0)
			currentIndex += 2;

		if(currentIndex < argc && strcmp(argv[currentIndex], "-c") == 0)
			currentIndex += 3;
		
		if(currentIndex == argc)
			return 0;
	}
	
	return 1;
}

int main(int argc, char **argv) {
	int currentIndex;
	char *eepromFile = NULL;
	char *eepromDumpFile = NULL;
	char *flashDumpFile = NULL;
	char *compareFileA = NULL;
	char *compareFileB = NULL;

    if(testArgs(argc, argv)) {
        printUsage(argv[0]);
        return 1;
    }
	
    if(argc > 1 && (strcmp(argv[1], "-h") == 0 || strcmp(argv[1], "--help") == 0)) {
        printUsage(argv[0]);
        return 0;
    }

	currentIndex = 1;		
	
	if(argc >= currentIndex + 2 && strcmp(argv[currentIndex], "-e") == 0) {
		eepromFile = argv[currentIndex + 1];
		currentIndex += 2;
	}
	
	if(argc >= currentIndex + 2 && strcmp(argv[currentIndex], "-de") == 0) {
		eepromDumpFile = argv[currentIndex + 1];
		currentIndex += 2;
	}

	if(argc >= currentIndex + 2 && strcmp(argv[currentIndex], "-df") == 0) {
		flashDumpFile = argv[currentIndex + 1];
		currentIndex += 2;
	}

	if(argc >= currentIndex + 3 && strcmp(argv[currentIndex], "-c") == 0) {
		compareFileA = argv[currentIndex + 1];
		compareFileB = argv[currentIndex + 2];
		currentIndex += 3;
	}
	
	if(eepromFile != NULL)
		printf("EEPROM file: \"%s\"\n", eepromFile);

	if(eepromDumpFile != NULL)
		printf("EEPROM dump file: \"%s\"\n", eepromDumpFile);

	if(flashDumpFile != NULL)
		printf("Flash dump file: \"%s\"\n", flashDumpFile);

	if(compareFileA != NULL)
		printf("Compare files: \"%s\", \"%s\"\n", compareFileA, compareFileB);
		
    if((argc == 1 || eepromFile != NULL || eepromDumpFile != NULL || flashDumpFile != NULL) && ((err = usbOpenDevice(&dev, IDENT_VENDOR_NUM, IDENT_VENDOR_STRING, IDENT_PRODUCT_NUM, IDENT_PRODUCT_STRING, 1)) != 0)) {
		fprintf(stderr, "Error opening MemoryTool device: %s\n", usbErrorMessage(err));
		return 2;
	}
	else if(argc == 1) {
		printf("EEPROM Sizes\n");
		printf("------------\n");
		readSizes(EEPROM_SIZE_QUERY_REPORT_ID, 0);			
		printf("\n");
		printf("Flash Sizes\n");
		printf("-----------\n");
		readSizes(FLASH_SIZE_QUERY_REPORT_ID, 0);
		
		if(dev != NULL)
			usbCloseDevice(dev);

		printf("\nUse argument \"-h\" or \"--help\" for usage information.\n");
		return 0;
	}
	
	if(eepromFile != NULL) {
		startAddress = sizeof(dataBuffer);
		endAddress = 0;
		memset(dataBuffer, -1, sizeof(dataBuffer));

		if(parseIntelHex(eepromFile, dataBuffer, &startAddress, &endAddress))
			return 5;

		if(startAddress >= endAddress) {
			fprintf(stderr, "No data in EEPROM input file.\n");
			return 5;
		}
		else if(uploadEEPROMData(dataBuffer, startAddress, endAddress))
			return 5;
	}
	
	if(eepromDumpFile != NULL) {
		memset(dataBuffer, -1, sizeof(dataBuffer));
		endAddress = dumpEEPROMData(dataBuffer, sizeof(dataBuffer));
		
		if(endAddress == 0) {
			fprintf(stderr, "Error dumping EEPROM data.\n");
			return 6;
		}
		
		if(writePlainHEX(eepromDumpFile, dataBuffer, endAddress))
			return 7;
	}

	if(flashDumpFile != NULL) {
		memset(dataBuffer, -1, sizeof(dataBuffer));
		endAddress = dumpFlashData(dataBuffer, sizeof(dataBuffer));
		
		if(endAddress == 0) {
			fprintf(stderr, "Error dumping flash data.\n");
			return 8;
		}
		
		if(writePlainHEX(flashDumpFile, dataBuffer, endAddress))
			return 9;
	}

	if(compareFileA != NULL) {
		if(comparePlainHEXFiles(compareFileA, compareFileB))
			return 10;
	}
	
	if(dev != NULL)
        usbCloseDevice(dev);

    return 0;
}

/* ------------------------------------------------------------------------- */


