#include "wiimote_extension_controller.h"

#if F_CPU == NULL
#error "define your clock speed"
#endif

#include <avr/io.h>

#include "wii_pindef.h"
#include "macros.h"


#include "wiimote.h"

// most of this data is found on
// http://wiibrew.org/wiki/Wiimote/Extension_Controllers

// drum id
const unsigned char drum_id[6] = {0x01, 0x00, 0xA4, 0x20, 0x01, 0x03};

// code for each pad
const unsigned char pad_tbl[8] = {
	0xFF,
	0xFF,
	0b10110110,
	0b10011110,
	0b10100100,
	0b10100010,
	0b10110010,
	0b10011100
};

// calibration data
const unsigned char cal_data[32] = {
	0x00, 0x00, 0x00, 0x00,
	0x00, 0x00, 0x00, 0x00,
	0x00, 0x00, 0x00, 0x00,
	0x00, 0x00, 0x00, 0x00,
	0x00, 0x00, 0x00, 0x00,
	0x00, 0x00, 0x00, 0x00,
	0x00, 0x00, 0x00, 0x00,
	0x00, 0x00, 0x00, 0x00
};

static volatile unsigned char hit_f_l; // hit flag, ensures pad is "hit" for minimum counts of time
static volatile unsigned char hit_f_h; // second flag, ensures pad is "released" for same amount of time
static volatile unsigned char hit_s[8]; // hit softness, 0 = hard, 7 = softest, do not go over 7
static volatile unsigned int hit_t[8]; // time associated with flags
static volatile unsigned char hit_last; // last pad hit
static volatile unsigned long wm_timer; // psuedo timer

// pin input comparison variables
static volatile unsigned char drum_in_preg;
static volatile unsigned char bass_in_preg;

void hit_pcint(unsigned char w)
{
	unsigned long t = wm_timer; // keep time
	
	#if GUARANTEE_RELEASE
	if(bit_is_set(hit_f_h, w))
	#else
	if(bit_is_set(hit_f_l, w))
	#endif
	{ // fresh hit
		cbi(hit_f_l, w);
		cbi(hit_f_h, w);
		hit_t[w] = t;
		hit_last = w;
		
		// TODO
		// modify this line to change hit softness
		// hit_s[w] = ?;
		// Note from Frank: I only play RB2 so I never bothered with the velocity stuff
	}
}

void check_for_hits()
{
	// compare before and after states

	#ifdef trig_on_fall

	unsigned char c = drum_in_reg;
	unsigned char s = drum_in_preg;

	#endif

	#ifdef trig_on_rise

	unsigned char s = drum_in_reg;
	unsigned char c = drum_in_preg;

	#endif

	if(bit_is_clear(c, green_pin) && bit_is_set(s, green_pin)) hit_pcint(green_bit);
	if(bit_is_clear(c, red_pin) && bit_is_set(s, red_pin)) hit_pcint(red_bit);
	if(bit_is_clear(c, yellow_pin) && bit_is_set(s, yellow_pin)) hit_pcint(yellow_bit);
	if(bit_is_clear(c, blue_pin) && bit_is_set(s, blue_pin)) hit_pcint(blue_bit);

	#ifdef GHWT

	if(bit_is_clear(c, orange_pin) && bit_is_set(s, orange_pin)) hit_pcint(orange_bit);

	#endif
	
	c = bass_in_reg;
	s = bass_in_preg;

	if(bit_is_clear(c, bass1_pin) && bit_is_set(s, bass1_pin)) hit_pcint(bass_bit);
	if(bit_is_clear(c, bass2_pin) && bit_is_set(s, bass2_pin)) hit_pcint(bass_bit);

	// previous = current
	drum_in_preg = drum_in_reg;
	bass_in_preg = bass_in_reg;
}

void check_hit_f_llags()
{
	unsigned long t = wm_timer;
	for(unsigned char i = 0; i < 8; i++)
	{
		if(bit_is_clear(hit_f_l, i) && ((t - hit_t[i]) > hit_min_time))
		{
			// clear flag if flag has been set for too long
			sbi(hit_f_l, i);
		}
		if(bit_is_clear(hit_f_h, i) && ((t - hit_t[i]) > (hit_min_time * 2)))
		{
			// clear flag if flag has been set for too long
			sbi(hit_f_h, i);
		}		
	}
}

void wm_timer_inc()
{
	wm_timer++;
	if(hit_f_h == 0xFF)
	{
		// if nothing is hit, then clear timer
		wm_timer = 0;
	}
}

void wiimote_extension_controller()
{
	#ifdef USE_SERPORT
	// start serial port
	sbi(uart_port, uart_rx_pin); // pull up
	serInit(38400);
	#endif

	// initialize ports

	// make power detect pin input
	cbi(power_detect_port, power_detect_pin);
	cbi(power_detect_ddr, power_detect_pin);

	sbi(LED_port, LED_pin);
	sbi(LED_ddr, LED_pin);

	#ifdef pull_up_res
	#ifdef trig_on_fall
	// setting port = pull ups on
	sbi(drum_port, green_pin);
	sbi(drum_port, red_pin);
	sbi(drum_port, yellow_pin);
	sbi(drum_port, blue_pin);
	sbi(drum_port, orange_pin);
	#endif
	#else
	cbi(drum_port, green_pin);
	cbi(drum_port, red_pin);
	cbi(drum_port, yellow_pin);
	cbi(drum_port, blue_pin);
	cbi(drum_port, orange_pin);
	#endif

	#ifdef trig_on_rise
	// clearing port = pull ups off
	cbi(drum_port, green_pin);
	cbi(drum_port, red_pin);
	cbi(drum_port, yellow_pin);
	cbi(drum_port, blue_pin);
	cbi(drum_port, orange_pin);
	#endif

	sbi(bass_port, bass1_pin);
	sbi(bass_port, bass2_pin);

	#ifdef GHWT
	sbi(plusminus_port, plus_pin);
	sbi(plusminus_port, minus_pin);
	sbi(stick_port, up_stick_pin);
	sbi(stick_port, down_stick_pin);
	sbi(stick_port, left_stick_pin);
	sbi(stick_port, right_stick_pin);
	#endif

	// all input
	cbi(drum_ddr, green_pin);
	cbi(drum_ddr, red_pin);
	cbi(drum_ddr, yellow_pin);
	cbi(drum_ddr, blue_pin);
	cbi(drum_ddr, orange_pin);
	cbi(bass_ddr, bass1_pin);
	cbi(bass_ddr, bass2_pin);

	#ifdef GHWT
	cbi(plusminus_ddr, plus_pin);
	cbi(plusminus_ddr, minus_pin);
	cbi(stick_ddr, up_stick_pin);
	cbi(stick_ddr, down_stick_pin);
	cbi(stick_ddr, left_stick_pin);
	cbi(stick_ddr, right_stick_pin);
	#endif

	// preinitialize comparison
	drum_in_preg = drum_in_reg;
	bass_in_preg = bass_in_reg;

	// initialize variables	
	wm_timer = 0;

	// initialize flags
	hit_f_l = 0xFF;
	hit_f_h = 0xFF;
	for(unsigned char i = 0; i < 8; i++)
	{
		hit_t[i] = 0;
		hit_s[i] = default_hit_softness;
	}

	unsigned char but_dat[6]; // struct containing button data
	but_dat[0] = 0b00011111;
	but_dat[1] = 0b00011111;
	but_dat[2] = 0b11111111;
	but_dat[3] = 0b11111111;
	but_dat[4] = 0b11111111;
	but_dat[5] = 0b11111111;

	// make wiimote think this is a drum
	wm_init(drum_id, but_dat, cal_data, wm_timer_inc);

	while(1)
	{
		// check if connected to wiimote
		if(bit_is_clear(power_detect_input, power_detect_pin))
		{
			// disconnected

			#ifdef USE_SERPORT
			// clear serial port buffer
			serFlush();
			#endif

			// handles reconnections
			wm_init(drum_id, but_dat, cal_data, wm_timer_inc);

			continue;
		}
		
		// check hardware
		check_for_hits();
		check_hit_f_llags();

		// apply hits
		but_dat[5] = hit_f_l;

		#ifdef USE_SERPORT
		unsigned char d; // serial port latest data
		unsigned char c; // number of char in serial port buffer
		d = serRx(&c); // check for serial command

		if(c > 0) // new command over serial port
		{
			but_dat[4] = 0xFF;
			but_dat[5] = 0xFF;

			if(bit_is_set(d, 0)) cbi(but_dat[5], green_bit);
			if(bit_is_set(d, 1)) cbi(but_dat[5], red_bit);
			if(bit_is_set(d, 2)) cbi(but_dat[5], yellow_bit);
			if(bit_is_set(d, 3)) cbi(but_dat[5], blue_bit);
			if(bit_is_set(d, 4)) cbi(but_dat[5], orange_bit);
			if(bit_is_set(d, 5)) cbi(but_dat[5], bass_bit);

			if(bit_is_set(d, 6)) cbi(but_dat[4], minus_bit);
			if(bit_is_set(d, 7)) cbi(but_dat[4], plus_bit);
		}
		#endif

		#ifdef GHWT

		but_dat[2] = 0xFF;
		but_dat[3] = 0xFF;

		if(but_dat[5] != 0xFF)
		{
			unsigned long t = wm_timer;
			
			// if any pads active, then send "softness"
			for(unsigned long i = hit_last + t; i < 16; i++)
			{
				unsigned char j = (unsigned char)(i % 8);
				if(bit_is_clear(but_dat[5], j))
				{
					// set pad
					but_dat[2] = pad_tbl[j];
					
					// set softness
					but_dat[3] = 0b00001100 | (hit_s[i] << 5);
					but_dat[4] &= 0b01111110;
					break;
				}
			}
		}

		// plus and minus buttons
		if(bit_is_clear(plusminus_in_reg, plus_pin)) cbi(but_dat[4], plus_bit);
		if(bit_is_clear(plusminus_in_reg, minus_pin)) cbi(but_dat[4], minus_bit);

		// simulate thumbstick with switches
		but_dat[0] = 0b00011111;
		but_dat[1] = 0b00011111;
		if(bit_is_clear(stick_in_reg, up_stick_pin)) but_dat[1] += thumbstick_speed;
		if(bit_is_clear(stick_in_reg, down_stick_pin)) but_dat[1] -= thumbstick_speed;
		if(bit_is_clear(stick_in_reg, left_stick_pin)) but_dat[0] -= thumbstick_speed;
		if(bit_is_clear(stick_in_reg, right_stick_pin)) but_dat[0] += thumbstick_speed;

		#endif

		wm_newaction(but_dat);
	}

	return 0;
}

#ifdef trig_on_fall
#ifdef trig_on_rise
#error "cannot define both trig_on_fall and trig_on_rise"
#endif
#endif
