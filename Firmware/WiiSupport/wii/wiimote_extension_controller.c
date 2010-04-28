// Wii support based on http://frank.circleofcurrent.com/cache/wii_extension_lib.htm

/*
Wii Support
-----------
The Dual Strike can act as a Wii Classic Controller when connected to a
Wiimote. You need a passive adapter from USB to the Wii extension port
connected like follows.

Wiimote extension port layout (as seen looking on the Wiimote bottom): 
+---___---+
| 1  2  3 |
| ======= |
| 4  5  6 |
+---------+
1: Ground [white]
2: No Connection
3: SCL [yellow]
4: SDA [green]
5: Device Detect [red]
6: VCC (3.3V) [red]

USB type B port layout (as seen looking at the port)
   --
 /    \
| 2  1 |
| ==== |
| 3  4 |
+------+
1: VCC (5V) [red]
2: D- [white]
3: D+ [green]
3: Ground [black]

Your adapter has to make the following connections:
USB D- | Wiimote SCL
USB D+ | Wiimote SDA
USB VCC | Wiimote VCC
USB Ground | Wiimote Ground
Wiimote VCC | Wiimote Device Detect

The adapter has to be connected to the USB port BEFORE PLUGGING
IT INTO THE WIIMOTE.

*/

#include "wiimote_extension_controller.h"

#if F_CPU == NULL
#error "define your clock speed"
#endif

#include <avr/io.h>

#include "macros.h"
#include "wiimote.h"
#include "../DualStrike.h"

extern uint8_t config[2];

// most of this data is found on
// http://wiibrew.org/wiki/Wiimote/Extension_Controllers

// device id
const uchar deviceID[2] = {0x01, 0x01};

// calibration data
const uchar calibrationData[32] = {
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
	uchar	rx4_3Lx;
	uchar	rx2_1Ly;
	uchar   rx0Lt4_3Ry;	
	uchar   lt2_0Rt;
	uchar	rightDownBltMinusHomePlusBrtNA;
	uchar	bzlBbByBaBxBzrLeftUp;
} buttonData_t;

static buttonData_t buttonData;

void wm_timer_inc() {
	return;
}

void resetButtonData() {
	buttonData.rx4_3Lx = 0b10100000;
	buttonData.rx2_1Ly = 0b00100000;
	buttonData.rx0Lt4_3Ry = 0b00010000;
	buttonData.lt2_0Rt = 
	buttonData.rightDownBltMinusHomePlusBrtNA = 
	buttonData.bzlBbByBaBxBzrLeftUp = 0;
}

void readInputWii() {
	resetButtonData();

	// Left Joystick Directions
	if(CFG_LEFT_STICK) {
		if (!Stick_Up)
			buttonData.rx2_1Ly &= 0b11000000;
		else if (!Stick_Down)
			buttonData.rx2_1Ly |= 0x00111111;

		if (!Stick_Left)
			buttonData.rx4_3Lx &= 0b11000000;
		else if (!Stick_Right)
			buttonData.rx4_3Lx |= 0b00111111;
	}

	// Right Joystick Directions
	if(CFG_RIGHT_STICK) {
		if (!Stick_Up)
			buttonData.rx0Lt4_3Ry &= 0b11100000;
		else if (!Stick_Down)
			buttonData.rx0Lt4_3Ry |= 0b00011111;
		
		if (!Stick_Left) {
			buttonData.rx4_3Lx &= 0b00111111;
			buttonData.rx2_1Ly &= 0b00111111;
			buttonData.rx0Lt4_3Ry &= ~(1<<7);
		}
		else if (!Stick_Right) {
			buttonData.rx4_3Lx |= 0b11000000;
			buttonData.rx2_1Ly |= 0b11000000;
			buttonData.rx0Lt4_3Ry |= (1<<7);
		}
	}

	// Digital Pad Directions
	if(CFG_DIGITAL_PAD) {
		if(!Stick_Up)
			buttonData.bzlBbByBaBxBzrLeftUp |= (1<<0);
		else if(!Stick_Down)
			buttonData.rightDownBltMinusHomePlusBrtNA |= (1<<6);

		if(!Stick_Right)
			buttonData.rightDownBltMinusHomePlusBrtNA |= (1<<7);
		else if(!Stick_Left) 
			buttonData.bzlBbByBaBxBzrLeftUp |= (1<<1);
	}

	// Buttons
	if(!Stick_Jab)
		buttonData.bzlBbByBaBxBzrLeftUp |= (1<<3);

	if(!Stick_Short)
		buttonData.bzlBbByBaBxBzrLeftUp |= (1<<5);

	if(!Stick_Strong) {
		buttonData.bzlBbByBaBxBzrLeftUp |= (1<<7);
	}

	if(!Stick_Forward)
		buttonData.bzlBbByBaBxBzrLeftUp |= (1<<4);

	if(!Stick_Fierce)
		buttonData.bzlBbByBaBxBzrLeftUp |= (1<<2);

	if(!Stick_Roundhouse) {
		buttonData.rightDownBltMinusHomePlusBrtNA |= (1<<1);
		buttonData.lt2_0Rt |= 0b00011111;
	}

#ifdef EXTRA_BUTTONS					
	if(!Stick_Extra0)
		buttonData.bzlBbByBaBxBzrLeftUp |= (1<<7);

	if(!Stick_Extra1) {
		buttonData.rightDownBltMinusHomePlusBrtNA |= (1<<5);
		buttonData.lt2_0Rt |= 0b11100000;
		buttonData.rx0Lt4_3Ry |= 0b01100000;
	}
#endif

	if(CFG_HOME_EMU && !Stick_Start && !Stick_Select)
		buttonData.rightDownBltMinusHomePlusBrtNA |= (1<<3);
	else {
		if(!Stick_Start)
			buttonData.rightDownBltMinusHomePlusBrtNA |= (1<<4);

		if(!Stick_Select)
			buttonData.rightDownBltMinusHomePlusBrtNA |= (1<<2);
	}

	if(!Stick_Home)
		buttonData.rightDownBltMinusHomePlusBrtNA |= (1<<3);
}

void wiimote_extension_controller() {
	wm_init((uchar *)deviceID, (uchar *)&buttonData, (uchar *)calibrationData, wm_timer_inc);

	while(1) {
		readJoystickSwitch();
		readInputWii();
		wm_newaction((void*)&buttonData);
	}
}

