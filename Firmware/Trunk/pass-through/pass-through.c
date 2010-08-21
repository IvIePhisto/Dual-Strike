#include "pass-through.h"

#include "../DualStrike.h"
#include <avr/io.h>

extern uint8_t config[2];

void pass_through() {

    while(1) {
	    if(CFG_JOYSTICK_SWITCH_EMU) {
			updateStartState();
			updateJoystickMode();

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

		if(CFG_EMU_4X) {
			if(!Stick_Start && !Stick_4K) {
				EXTRA_4K_PORT &= ~(1<<EXTRA_4K_PIN);
				EXTRA_4K_DDR  |=  (1<<EXTRA_4K_PIN);
			}
			else {
				EXTRA_4K_DDR  &= ~(1<<EXTRA_4K_PIN);
				EXTRA_4K_PORT |=  (1<<EXTRA_4K_PIN);
			}

			if(!Stick_Start && !Stick_4P) {
				EXTRA_4P_PORT &= ~(1<<EXTRA_4P_PIN);
				EXTRA_4P_DDR  |=  (1<<EXTRA_4P_PIN);
			}
			else {
				EXTRA_4P_DDR  &= ~(1<<EXTRA_4P_PIN);
				EXTRA_4P_PORT |=  (1<<EXTRA_4P_PIN);
			}
		}

        if(CFG_INVERTED_TRIGGERS) {
            if(!Stick_4K)
                S3_PORT |=  (1<<S3_PIN);
            else
                S3_PORT &= ~(1<<S3_PIN);
    
            if(!Stick_HK)
                S4_PORT |=  (1<<S4_PIN);
            else
                S4_PORT &= ~(1<<S4_PIN);
        }
    }
}

