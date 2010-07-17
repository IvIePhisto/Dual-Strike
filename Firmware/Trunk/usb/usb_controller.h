#ifndef __usb_controller_h_included__
#define __usb_controller_h_included__

void ps3_controller();
void mame_controller();
void xbox_controller();

void programmer_setup();
void programmer_poll();

void disconnectUSB();
#endif
