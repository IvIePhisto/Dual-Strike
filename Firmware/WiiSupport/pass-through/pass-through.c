#include "pass-through.h"

#include "../DualStrike.h"
#include <avr/io.h>

extern uint8_t config;

void pass_through() {
    if(CFG_JOYSTICK_SWITCH_EMU) {	
        if(CFG_LEFT_STICK) {
            PORTD &= ~(1<<4); // pin S3 is low	
            PORTC |= (1<<6); // pin S4 is high
        }
        else if(CFG_RIGHT_STICK) {
            PORTD |= (1<<4); // pin S3 is high	
            PORTC &= ~(1<<6); // pin S4 is low
        }
        else if(CFG_DIGITAL_PAD) {
            PORTD |= (1<<4); // pin S3 is high	
            PORTC |= (1<<6); // pin S4 is high
        }
    }

    while(1) { /* main event loop */
        if(CFG_HOME_EMU) {
            if((!Stick_Start) &&  (!Stick_Select))
                //if start and select are pressed then make guide line low
                SWITCH_HOME_LOW
            else
                //else guide line is high
                SWITCH_HOME_HIGH
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

