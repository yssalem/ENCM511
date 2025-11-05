/*
 * File:   ADC.c
 * Author: noors
 *
 * Created on November 4, 2025, 2:18 PM
 */

#include "xc.h"
#include "uart.h"
#include "timer.h"


void ADCinit(void){
    
    //Setting reference voltages
    AD1CON2bits.PVCFG = 0; //Using AVdd as positive reference
    AD1CON2bits.NVCFG = 0; // Using AVss as negative reference
    
    //Setting inputs for each channel
    AD1CHSbits.CH0NB = 0;
    AD1CHSbits.CH0NB = 0b00101;
    
    //Setting clock to match system clock
    AD1CON3bits.ADRC = 0;
    
    //AD Conversion select TAD = 256 * TCY
    AD1CON3bits.ADCS = 0b11111111;
    
    //Auto-convert mode
    AD1CON1bits.SSRC = 0b0111;
    
    //Auto-sample time select 31 TAD
    AD1CON3bits.SAMC = 0b11111;
    
    //How will data be displayed (decimal)
    AD1CON1bits.FORM = 0;
    
    //Interrupt rate (every sample)
    AD1CON2bits.SMPI = 0;
    
    //Configure A/D interrupt 
    IFS0bits.AD1IF = 0; //clear bit
    IEC0bits.AD1IE = 1; //Enable AD interrupt
    IPC3bits.AD1IP = 4; //Set interrupt priority    
    
    AD1CON1bits.ADON = 1;

}


uint16_t do_ADC(void){
    uint16_t result;
    AD1CON1bits.SAMP = 1;
    delay_ms(1);
    AD1CON1bits.SAMP = 0;
    while(!AD1CON1bits.DONE){
    }
    uint16_t result = ADC1BUF0;
    return result;
}

