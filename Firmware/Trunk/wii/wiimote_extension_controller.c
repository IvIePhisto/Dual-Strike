/* Wii support based on:
   http://frank.circleofcurrent.com/cache/wii_extension_lib.htm
   http://wiibrew.org/wiki/Wiimote/Extension_Controllers
*/

// README
/*
Wiimote Adapter
===============
The Dual Strike can act as a Wii Classic Controller when connected to a
Wiimote. You need a passive adapter from USB to the Wiimote extension port
for this working mode, its composition is detailed below.

If you get your Wiimote plug and cable of a working Wii extension (Nunchuck,
Classic Controller, ...), you can add a USB jack to the extension's side of
the cable. So you can continue using the extension by utilising your adapter.

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
5: Device Detect, in the connector wired to VCC [red]
6: VCC (3.3V) [red]

USB type B port layout (as seen looking at the port):
   --
 /    \
| 2  1 |
| ==== |
| 3  4 |
+------+
1: VCC (5V) [red]
2: D- [white]
3: D+ [green]
4: Ground [black]

Your adapter has to make the following connections:

USB Wires      | Wiimote Wires
---------------+---------------
D-     [white] | SCL    [yellow]
D+     [green] | SDA    [green]
VCC    [red]   | VCC    [red]
Ground [black] | Ground [white]

The adapter should be connected to the USB port before plugging it into
the Wiimote.
*/

#include "wiimote_extension_controller.h"

#if F_CPU == NULL
#error "define your clock speed"
#endif

#include <avr/io.h>
#include <avr/pgmspace.h>

#include "wiimote.h"
#include "../DualStrike.h"

#ifndef uchar
#define uchar   unsigned char
#endif

extern uint8_t config[2];

// device id starting at address 0xA400FA
PROGMEM const uchar deviceID[6] = {0x00, 0x00, 0xA4, 0x20, 0x01, 0x01};

// calibration data starting at address 0xA40020
PROGMEM const uchar calibrationData[16] = {
	// left stick X
	0b00000000, // -min
	0b00100000, // -mid
	0b00111111, // -max

	// left stick Y
	0b00000000, // -min
	0b00100000, // -mid
	0b00111111, // -max

	// right stick X
	0b00000000, // -min
	0b00010000, // -mid
	0b00011111, // -max

	// right stick Y
	0b00000000, // -min
	0b00010000, // -mid
	0b00011111, // -max

	// left trigger
	0b00000000, // -min
	0b00001111, // -max

	// right trigger
	0b00000000, // -min
	0b00001111, // -max
	/*	
    0xff, 0x00, 0x80, 0xff, 0x00, 0x80,
    0xff, 0x00, 0x80, 0xff, 0x00, 0x80,
    0x00, 0x00, 0x51, 0xa6
	*/
};

typedef struct {
	uchar	rx4_3Lx;
	uchar	rx2_1Ly;
	uchar   rx0Lt4_3Ry;	
	uchar   lt2_0Rt;
	uchar	rightDownBltMinusHomePlusBrtNA;
	uchar	bzlBbByBaBxBzrLeftUp;
} data_t;

extern data_t data;

#define WII_UP		data.bzlBbByBaBxBzrLeftUp &= ~(1<<0);
#define WII_DOWN	data.rightDownBltMinusHomePlusBrtNA &= ~(1<<6);
#define WII_RIGHT	data.rightDownBltMinusHomePlusBrtNA &= ~(1<<7);
#define WII_LEFT	data.bzlBbByBaBxBzrLeftUp &= ~(1<<1);
#define WII_Y 		data.bzlBbByBaBxBzrLeftUp &= ~(1<<5);
#define WII_X		data.bzlBbByBaBxBzrLeftUp &= ~(1<<3);
#define WII_ZR		data.bzlBbByBaBxBzrLeftUp &= ~(1<<2);
#define WII_RT		data.rightDownBltMinusHomePlusBrtNA &= ~(1<<1);
#define WII_B		data.bzlBbByBaBxBzrLeftUp &= ~(1<<6);
#define WII_A		data.bzlBbByBaBxBzrLeftUp &= ~(1<<4);
#define WII_ZL		data.bzlBbByBaBxBzrLeftUp &= ~(1<<7);
#define WII_LT		data.rightDownBltMinusHomePlusBrtNA &= ~(1<<5);
#define WII_HOME	data.rightDownBltMinusHomePlusBrtNA &= ~(1<<3);
#define WII_MINUS	data.rightDownBltMinusHomePlusBrtNA &= ~(1<<4);
#define WII_PLUS	data.rightDownBltMinusHomePlusBrtNA &= ~(1<<2);

void resetButtonData() {
	data.rx4_3Lx = 0b10100000;
	data.rx2_1Ly = 0b00100000;
	data.rx0Lt4_3Ry = 0b00010000;
	data.lt2_0Rt = 
	data.rightDownBltMinusHomePlusBrtNA = 
	data.bzlBbByBaBxBzrLeftUp = 0xFF;
}

void readInputWii() {
	resetButtonData();

	// Left Joystick Directions
	if(CFG_LEFT_STICK) {
		if (!Stick_Up)
			data.rx2_1Ly &= 0b11000000;
		else if (!Stick_Down)
			data.rx2_1Ly |= 0x00111111;

		if (!Stick_Left)
			data.rx4_3Lx &= 0b11000000;
		else if (!Stick_Right)
			data.rx4_3Lx |= 0b00111111;
	}

	// Right Joystick Directions
	if(CFG_RIGHT_STICK) {
		if (!Stick_Up)
			data.rx0Lt4_3Ry &= 0b11100000;
		else if (!Stick_Down)
			data.rx0Lt4_3Ry |= 0b00011111;
		
		if (!Stick_Left) {
			data.rx4_3Lx &= 0b00111111;
			data.rx2_1Ly &= 0b00111111;
			data.rx0Lt4_3Ry &= ~(1<<7);
		}
		else if (!Stick_Right) {
			data.rx4_3Lx |= 0b11000000;
			data.rx2_1Ly |= 0b11000000;
			data.rx0Lt4_3Ry |= (1<<7);
		}
	}

	// Digital Pad Directions
	if(CFG_DIGITAL_PAD) {
		if(!Stick_Up)
			WII_UP
		else if(!Stick_Down)
			WII_DOWN

		if(!Stick_Right)
			WII_RIGHT
		else if(!Stick_Left) 
			WII_LEFT
	}

	// Buttons
	if(!Stick_Jab)
		WII_UP

	if(!Stick_Short)
		WII_X

	if(!Stick_Strong)
		WII_ZL

	if(!Stick_Forward)
		WII_B

	if(!Stick_Fierce)
		WII_A

	if(!Stick_Roundhouse) {
		WII_RT
		data.lt2_0Rt &= ~0b00011111;
	}

#ifdef EXTRA_BUTTONS					
	if(!Stick_Extra0)
		WII_ZL

	if(!Stick_Extra1) {
		WII_LT
		data.rx0Lt4_3Ry &= ~0b01100000;
		data.lt2_0Rt &= ~0b11100000;
	}
#endif

	if(CFG_HOME_EMU && !Stick_Start && !Stick_Select)
		WII_HOME
	else {
		if(!Stick_Start)
			WII_MINUS

		if(!Stick_Select)
			WII_PLUS
	}

	if(!Stick_Home)
		WII_HOME
}

void wiimote_extension_controller() {
	resetButtonData();
	wm_init((uchar *)deviceID, (uchar *)&data, (uchar *)calibrationData, sizeof(calibrationData));
	wm_newaction((void*)&data);

	while(1) {
		readJoystickSwitch();
		readInputWii();
		wm_newaction((void*)&data);
	}
}

