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
3: Ground [black]

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
PROGMEM const uchar deviceID[6] = {0, 0, 0, 0, 0x01, 0x01};

// calibration data starting at address 0xA40020
PROGMEM const uchar calibrationData[6] = { 0,0,0,0,0,0 };

typedef struct {
	uchar	rx4_3Lx;
	uchar	rx2_1Ly;
	uchar   rx0Lt4_3Ry;	
	uchar   lt2_0Rt;
	uchar	rightDownBltMinusHomePlusBrtNA;
	uchar	bzlBbByBaBxBzrLeftUp;
} data_t;

extern data_t data;

void wm_timer_inc() {}

void resetButtonData() {
	data.rx4_3Lx = 0b10100000;
	data.rx2_1Ly = 0b00100000;
	data.rx0Lt4_3Ry = 0b00010000;
	data.lt2_0Rt = 
	data.rightDownBltMinusHomePlusBrtNA = 
	data.bzlBbByBaBxBzrLeftUp = 0;
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
			data.bzlBbByBaBxBzrLeftUp |= (1<<0);
		else if(!Stick_Down)
			data.rightDownBltMinusHomePlusBrtNA |= (1<<6);

		if(!Stick_Right)
			data.rightDownBltMinusHomePlusBrtNA |= (1<<7);
		else if(!Stick_Left) 
			data.bzlBbByBaBxBzrLeftUp |= (1<<1);
	}

	// Buttons
	if(!Stick_Jab)
		data.bzlBbByBaBxBzrLeftUp |= (1<<5);

	if(!Stick_Short)
		data.bzlBbByBaBxBzrLeftUp |= (1<<3);

	if(!Stick_Strong) {
		data.bzlBbByBaBxBzrLeftUp |= (1<<7);
	}

	if(!Stick_Forward)
		data.bzlBbByBaBxBzrLeftUp |= (1<<2);

	if(!Stick_Fierce)
		data.bzlBbByBaBxBzrLeftUp |= (1<<4);

	if(!Stick_Roundhouse) {
		data.rightDownBltMinusHomePlusBrtNA |= (1<<1);
		data.lt2_0Rt |= 0b00011111;
	}

#ifdef EXTRA_BUTTONS					
	if(!Stick_Extra0)
		data.bzlBbByBaBxBzrLeftUp |= (1<<7);

	if(!Stick_Extra1) {
		data.rightDownBltMinusHomePlusBrtNA |= (1<<5);
		data.lt2_0Rt |= 0b11100000;
		data.rx0Lt4_3Ry |= 0b01100000;
	}
#endif

	if(CFG_HOME_EMU && !Stick_Start && !Stick_Select)
		data.rightDownBltMinusHomePlusBrtNA |= (1<<3);
	else {
		if(!Stick_Start)
			data.rightDownBltMinusHomePlusBrtNA |= (1<<4);

		if(!Stick_Select)
			data.rightDownBltMinusHomePlusBrtNA |= (1<<2);
	}

	if(!Stick_Home)
		data.rightDownBltMinusHomePlusBrtNA |= (1<<3);
}

void wiimote_extension_controller() {
	wm_init((uchar *)deviceID, (uchar *)&data, (uchar *)calibrationData, wm_timer_inc);

	while(1) {
		readJoystickSwitch();
		readInputWii();
		wm_newaction((void*)&data);
	}
}

