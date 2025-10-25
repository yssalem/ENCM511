/*
 * File:   IOCinit.c
 * Author: youss
 *
 * Created on October 24, 2025, 5:54 PM
 */

#include "IOCinit.h"
#include "xc.h"

void IOCinit(void) {
    // PB0
    IOCNBbits.IOCNB8 = 1; // IOC high - low
//    IOCPBbits.IOCPB8 = 1; // IOC low - high
    //PB1
    IOCNAbits.IOCNA4 = 1;
//    IOCPAbits.IOCPA4 = 1;
    //PB2
    IOCNBbits.IOCNB4 = 1;
//    IOCPBbits.IOCPB9 = 1;
    
    PADCONbits.IOCON = 1; // enables IOC
    IOCSTATbits.IOCPBF = 0; // clear PORTB change flag bit
    
    IFS1bits.IOCIF = 0; // clear IOC flag
    IPC4bits.IOCIP = 3; // set interrupt priority
    IEC1bits.IOCIE = 1; // enable interrupts
}
