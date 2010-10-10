#ifndef __usb_controller_h_included__
#define __usb_controller_h_included__

void ps3_controller();
void pc_controller();

#if ATMEGA_NO == 168
void mame_controller();
void xbox_controller();
#endif

void programmer_setup();
void programmer_poll();

void disconnectUSB();
#endif
