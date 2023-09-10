#include <Arduino.h>
#include "Switch.hpp"

#define SW_IDX_1 0
#define SW_IDX_2 1
#define SW_IDX_3 2

Switch *sw_tbl[] = { NULL,NULL,NULL };

void interruptSwitch_1 () {
    if (NULL != sw_tbl[SW_IDX_1]) {
        sw_tbl[SW_IDX_1]->interrupt(sw_tbl[SW_IDX_1]->get_pin());
    }
}

void interruptSwitch_2 () {
    if (NULL != sw_tbl[SW_IDX_2]) {
        sw_tbl[SW_IDX_2]->interrupt(sw_tbl[SW_IDX_2]->get_pin());
    }
}

void interruptSwitch_3 () {
    if (NULL != sw_tbl[SW_IDX_3]) {
        sw_tbl[SW_IDX_3]->interrupt(sw_tbl[SW_IDX_3]->get_pin());
    }
}


/**
 * set switch press interrupt
 * @input (int) pin number
 */
Switch::Switch (int pin) {
    /* pin setting */
    pin_num = pin;
    pinMode(pin, INPUT_PULLUP);
    
    /* set interrupt */
    int i;
    for (i=0; i < (sizeof(sw_tbl)/sizeof(Switch *)) ;i++) {
        if (NULL == sw_tbl[i]) {
            /* set table */
            sw_tbl[i] = this;
            
            switch (i) {
                case SW_IDX_1:
                    attachInterrupt(digitalPinToInterrupt(pin), interruptSwitch_1, CHANGE);
                    break;
                case SW_IDX_2:
                    attachInterrupt(digitalPinToInterrupt(pin), interruptSwitch_2, CHANGE);
                    break;
                case SW_IDX_3:
                    attachInterrupt(digitalPinToInterrupt(pin), interruptSwitch_3, CHANGE);
                    break;
            }
            break;
        } 
    }
}

void Switch::interrupt (int pin) {
    int           time_out  = 2000;
    int           delay_val = 200;
    bool          press_end = false;
    unsigned long swtm      = millis();
    unsigned long diff_swtm = swtm - last_swtm;
    
    /* check flickering */
    if (0 != last_swtm) {
        if (50 > diff_swtm) {
            last_swtm = millis();
            return;
        }
    }
    last_swtm = swtm;  // update last switched time

//    Serial.print("diff_swtm:");
//    Serial.println(diff_swtm);
        
    if ((E_Switch_Status::On == sts) && (LOW == digitalRead(pin))) {
        /* error */
        sts = E_Switch_Status::Off;
        if (200 > diff_swtm) {
            Serial.println("press event");
        }
        //Serial.println("error");
        return;
    }
        
    /* update status */
    if (LOW == digitalRead(pin)) {
        sts = E_Switch_Status::On;  
    } else {
        if ( (E_Switch_Status::On == sts) &&
             (200 > diff_swtm) ) {
            Serial.println("press event");
        }
        sts = E_Switch_Status::Off;
    }
    
    //Serial.println(digitalRead(pin));
}


int Switch::get_pin (void) {
    return pin_num;
}

void Switch::chkLoop (void) {
    if (HIGH == digitalRead(pin_num)) {
        return;
    }
    unsigned long diff_swtm = millis() - last_swtm;
    
    if ((E_Switch_Status::On == sts) && (2000 < diff_swtm)) {
        Serial.println("long press event");
        sts = E_Switch_Status::Off;
    }
}
