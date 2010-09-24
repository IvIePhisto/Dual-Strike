#include "pass-through.h"

#include "../DualStrike.h"
#include <avr/io.h>

extern uint8_t config[2];

void pass_through() {
    if(CFG_JOYSTICK_SWITCH_EMU) {	
        if(CFG_LEFT_STICK) {
            S4_PORT |=  (1<<S4_PIN);
            S3_PORT &= ~(1<<S3_PIN);
			S3_DDR	|=  (1<<S3_PIN);
			S4_DDR 	|=  (1<<S4_PIN);
        }
        else if(CFG_RIGHT_STICK) {
            S3_PORT |=  (1<<S3_PIN);
            S4_PORT &= ~(1<<S4_PIN);
			S3_DDR  |=  (1<<S3_PIN);
			S4_DDR  |=  (1<<S4_PIN);
        }
        else if(CFG_DIGITAL_PAD) {
			S3_DDR  &= ~(1<<S3_PIN);
			S4_DDR  &= ~(1<<S4_PIN);
            S3_PORT |=  (1<<S3_PIN);
            S4_PORT |=  (1<<S4_PIN);
	    }
    }

    while(1) {
        if(CFG_HOME_EMU) {
            if((!Stick_Start) &&  (!Stick_Select)) {
                HOME_PORT	&=  ~(1<<HOME_PIN);
				HOME_DDR	|=   (1<<HOME_PIN);
			}
            else {
				HOME_DDR	&=  ~(1<<HOME_PIN);
                HOME_PORT	|=   (1<<HOME_PIN);
			}
        }

        if(CFG_INVERTED_TRIGGERS) {
            // if LT is pressed then invert trigger and make it high (pressed)
            if(!Stick_Extra1)
                PORTD |= (1<<4);
            else
                PORTD &= ~(1<<4);
    
            // if RT is pressed then invert trigger and make it high (pressed)
            if(!Stick_Roundhouse)
                PORTC |= (1<<6);
            // else keep trigger low
            else
                PORTC &= ~(1<<6);
        }
    }
}

