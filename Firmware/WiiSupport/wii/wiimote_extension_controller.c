#include "wiimote_extension_controller.h"

#if F_CPU == NULL
#error "define your clock speed"
#endif

#include <avr/io.h>

#include "macros.h"
#include "wiimote.h"
#include "../DualStrike.h"

extern uint8_t config;

// most of this data is found on
// http://wiibrew.org/wiki/Wiimote/Extension_Controllers

// device id
const unsigned char deviceID[2] = {0x01, 0x01};

// calibration data
const unsigned char calibrationData[32] = {
	0x00, 0x00, 0x00, 0x00,
	0x00, 0x00, 0x00, 0x00,
	0x00, 0x00, 0x00, 0x00,
	0x00, 0x00, 0x00, 0x00,
	0x00, 0x00, 0x00, 0x00,
	0x00, 0x00, 0x00, 0x00,
	0x00, 0x00, 0x00, 0x00,
	0x00, 0x00, 0x00, 0x00
};

typedef struct {
	uchar	rx43Lx;
	uchar	rx21Ly;
	uchar   rx0Lt43Ry;	
	uchar   lt20Rt;
	uchar	rightDownBltMinusHomePlusBrtNA;
	uchar	bzlByBaBxBzrLeftUp;
} buttonData_t;

static buttonData_t buttonData;

void wm_timer_inc() {
	return;
}

void resetButtonData() {
	buttonData.rx43Lx = 
	buttonData.rx21Ly = 
	buttonData.rx0Lt43Ry = 
	buttonData.lt20Rt = 
	buttonData.rightDownBltMinusHomePlusBrtNA = 
	buttonData.bzlByBaBxBzrLeftUp = 0;
}

void readJoystickWii() {
	resetButtonData();
}

void wiimote_extension_controller() {
	wm_init(deviceID, (void*)&buttonData, calibrationData, wm_timer_inc);

	while(1) {
		readJoystickWii();
		wm_newaction((void*)&buttonData);
	}
}

