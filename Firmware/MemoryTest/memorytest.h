#ifndef __DualStrike_h_included__
#define __DualStrike_h_included__

#include "usb/usb_controller.h"

#ifndef ulong
#   define ulong    unsigned long
#endif
#ifndef uint
#   define uint     unsigned int
#endif
#ifndef uchar
#define uchar   unsigned char
#endif

// BUTTON DEFINITIONS

#ifndef ATMEGA_NO
#error ATmega number has to be defined.
#endif

#if ((ATMEGA_NO != 8) && (ATMEGA_NO != 168))
#error ATmega with unsupported number used.
#endif

// set to 1 to use L1 and L2
#ifndef EXTRA_BUTTONS
#define EXTRA_BUTTONS 1
#endif

#define S1_PORT	PORTD
#define S1_PIN	0

#define S2_PORT	PORTD
#define S2_PIN	3

#endif
