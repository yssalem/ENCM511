/*
 * File:   timer.c
 * Author: ibrkh
 *
 * Created on October 24, 2025, 5:41 PM
 */

#include "timer.h"

void TM2init(void) {
    //T2CON config
    T2CONbits.T32 = 0;      // operate timer 2 as 16 bit timer
    T2CONbits.TCKPS = 3;    // set prescaler to 1:256
    
    T2CONbits.TCS = 0;      // use internal clock
    T2CONbits.TSIDL = 0;    // operate in idle mode
    IFS0bits.T2IF = 0;      // set flag 0
    IEC0bits.T2IE = 1;      // enable timer interrupt
    PR2 = 3906;             // set timer limit - overriden in delay function later
    TMR2 = 0;
    T2CONbits.TON = 0;      // disable timer initially
}

void TM3init(void) {
    //T3CON config
    T3CONbits.TCKPS = 3;    // set prescaler to 1:256
    
    T3CONbits.TCS = 0;      // use internal clock
    T3CONbits.TSIDL = 0;    // operate in idle mode
    IFS0bits.T3IF = 0;      // set flag 0
    IEC0bits.T3IE = 1;      // enable timer interrupt
    PR3 = 3906;             // set timer limit - overriden in delay function later
    TMR2 = 0;
    T3CONbits.TON = 0;      // disable timer initially
}

void delay_ms(uint16_t ms) {
    
    // set timer2 count limit
    // timer frequency of 15625 due to prescaler defined earlier
    // freq/1000 to convert ms to s
    PR2 = (float)(15625/1000) * (float)ms;  // floats used to prevent issues
                                            // with integer division
    
    TMR2 = 0;           // reset timer count value
    T2CONbits.TON = 1;  // turn timer ON
    Idle();             // wait for interrupt
}
void debounce(uint16_t ms) {
    
    // set timer3 count limit
    // timer frequency of 15625 due to prescaler defined earlier
    // freq/1000 to convert ms to s
    PR3 = (float)(15625/1000) * (float)ms;  // floats used to prevent issues
                                            // with integer division
    
    TMR3 = 0;           // reset timer count value
    T3CONbits.TON = 1;  // turn timer ON
    Idle();             // wait for interrupt
}
