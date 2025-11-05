/*
 * File:   IOinit.c
 * Author: youss
 *
 * Created on October 24, 2025, 5:46 PM
 */


#include "xc.h"
#include "IOinit.h"

void IOinit(void){
    
    // set output pin for LED0
    TRISBbits.TRISB5 = 0;
    
    LED0 = 0;
    
    // set input pins and pull ups for buttons
    //PB0
    TRISBbits.TRISB8 = 1;
    IOCPUBbits.CNPUB8 = 1;
    //PB1
    TRISAbits.TRISA4 = 1;
    IOCPUAbits.CNPUA4 = 1;
    //PB2
    TRISBbits.TRISB4 = 1;
    IOCPUBbits.CNPUB4 = 1;
    
    ANSAbits.ANSA5 = 1;
    
}
