#ifndef wiimote_h

#define twi_port PORTC
#define twi_ddr DDRC
#define twi_scl_pin 5
#define twi_sda_pin 4

/*
#define dev_detect_port PORTD
#define dev_detect_ddr DDRD
#define dev_detect_pin 4
*/

// initialize wiimote interface with id, starting data, and calibration data
void wm_init(unsigned char * id, unsigned char * buton_data, unsigned char * cal_data, unsigned int cal_data_length);

// set button data
void wm_newaction(unsigned char * button_data);

#define wiimote_h
#endif
