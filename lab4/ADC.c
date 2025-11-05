/*
 * File:   ADC.c
 * Author: noors
 *
 * Created on November 4, 2025, 2:18 PM
 */

#include "xc.h"
#include "uart.h"
#include "timer.h"


void ADCinit() {
    AD1CON2bits.PVCFG = 00; // High ref = VDD
    AD1CON2bits.NVCFG0 = 0; // Low ref = VSS
    
    AD1CHSbits.CH0NA = 000; // Neg input = VSS
    AD1CHSbits.CH0SA = 0b00101; // Pos input = AN3 ???
    
    AD1CON3bits.ADCS = 0b11111111; // set analog conversion clock
    
    AD1CON3bits.SAMC = 0b11111; // set auto sample time
    AD1CON1bits.SSRC = 0b0111; // auto convert mode
    
    AD1CON1bits.FORM = 00; // unsigned decimal result
    
    AD1CON2bits.SMPI = 00000;
    
    AD1CON1bits.ADON = 1;
}


uint16_t do_ADC(void) {
    uint16_t result;
    AD1CON1bits.SAMP = 1;
    delay_ms(1);
    AD1CON1bits.SAMP = 0;
    while(!AD1CON1bits.DONE);
    result = ADC1BUF0;
    
    return result;

}
