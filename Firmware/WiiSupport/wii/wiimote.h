#ifndef wiimote_h

#include <string.h>
#include <avr/io.h>
#include <util/delay.h>
#include <util/twi.h>
#include <avr/interrupt.h>

#define twi_port PORTD
#define twi_ddr DDRD
#define twi_scl_pin 1
#define twi_sda_pin 2

/*
#define dev_detect_port PORTD
#define dev_detect_ddr DDRD
#define dev_detect_pin 4
*/

// initialize wiimote interface with id, starting data, and calibration data
void wm_init(
	const unsigned char * id,
	const unsigned char * buton_data,
	const unsigned char * cal_data,
	void (*function)(void));

// set button data
void wm_newaction(const unsigned char * button_data);

#define wiimote_h
#endif
