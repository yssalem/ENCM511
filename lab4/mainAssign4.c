/*
 * File:   main.c
 * Author: Ibrahim Khalid, Noor Sheikh, Youssef Salem
 *
 * Created FOR ENCM 511
 * PLEASE ADD DATE CREATED HERE: 2025-11-04
 *
 * FAILURE TO UPDATE THIS HEADER WITH YOUR GROUP MEMBER NAMES
 * MAY RESULT IN PENALTIES
 */

// FSEC
#pragma config BWRP = OFF    //Boot Segment Write-Protect bit->Boot Segment may be written
#pragma config BSS = DISABLED    //Boot Segment Code-Protect Level bits->No Protection (other than BWRP)
#pragma config BSEN = OFF    //Boot Segment Control bit->No Boot Segment
#pragma config GWRP = OFF    //General Segment Write-Protect bit->General Segment may be written
#pragma config GSS = DISABLED    //General Segment Code-Protect Level bits->No Protection (other than GWRP)
#pragma config CWRP = OFF    //Configuration Segment Write-Protect bit->Configuration Segment may be written
#pragma config CSS = DISABLED    //Configuration Segment Code-Protect Level bits->No Protection (other than CWRP)
#pragma config AIVTDIS = OFF    //Alternate Interrupt Vector Table bit->Disabled AIVT

// FBSLIM
#pragma config BSLIM = 8191    //Boot Segment Flash Page Address Limit bits->8191

// FOSCSEL
#pragma config FNOSC = FRC    //Oscillator Source Selection->Internal Fast RC (FRC)
#pragma config PLLMODE = PLL96DIV2    //PLL Mode Selection->96 MHz PLL. Oscillator input is divided by 2 (8 MHz input)
#pragma config IESO = OFF    //Two-speed Oscillator Start-up Enable bit->Start up with user-selected oscillator source

// FOSC
#pragma config POSCMD = NONE    //Primary Oscillator Mode Select bits->Primary Oscillator disabled
#pragma config OSCIOFCN = ON    //OSC2 Pin Function bit->OSC2 is general purpose digital I/O pin
#pragma config SOSCSEL = OFF    //SOSC Power Selection Configuration bits->Digital (SCLKI) mode
#pragma config PLLSS = PLL_FRC    //PLL Secondary Selection Configuration bit->PLL is fed by the on-chip Fast RC (FRC) oscillator
#pragma config IOL1WAY = ON    //Peripheral pin select configuration bit->Allow only one reconfiguration
#pragma config FCKSM = CSECMD    //Clock Switching Mode bits->Clock switching is enabled,Fail-safe Clock Monitor is disabled

// FWDT
#pragma config WDTPS = PS32768    //Watchdog Timer Postscaler bits->1:32768
#pragma config FWPSA = PR128    //Watchdog Timer Prescaler bit->1:128
#pragma config FWDTEN = ON_SWDTEN    //Watchdog Timer Enable bits->WDT Enabled/Disabled (controlled using SWDTEN bit)
#pragma config WINDIS = OFF    //Watchdog Timer Window Enable bit->Watchdog Timer in Non-Window mode
#pragma config WDTWIN = WIN25    //Watchdog Timer Window Select bits->WDT Window is 25% of WDT period
#pragma config WDTCMX = WDTCLK    //WDT MUX Source Select bits->WDT clock source is determined by the WDTCLK Configuration bits
#pragma config WDTCLK = LPRC    //WDT Clock Source Select bits->WDT uses LPRC

// FPOR
#pragma config BOREN = ON    //Brown Out Enable bit->Brown Out Enable Bit  
#pragma config LPCFG = OFF    //Low power regulator control->No Retention Sleep
#pragma config DNVPEN = ENABLE    //Downside Voltage Protection Enable bit->Downside protection enabled using ZPBOR when BOR is inactive

// FICD
#pragma config ICS = PGD1    //ICD Communication Channel Select bits->Communicate on PGEC1 and PGED1
#pragma config JTAGEN = OFF    //JTAG Enable bit->JTAG is disabled

// FDEVOPT1
#pragma config ALTCMPI = DISABLE    //Alternate Comparator Input Enable bit->C1INC, C2INC, and C3INC are on their standard pin locations
#pragma config TMPRPIN = OFF    //Tamper Pin Enable bit->TMPRN pin function is disabled
#pragma config SOSCHP = ON    //SOSC High Power Enable bit (valid only when SOSCSEL = 1->Enable SOSC high power mode (default)
#pragma config ALTI2C1 = ALTI2CEN    //Alternate I2C pin Location->SDA1 and SCL1 on RB9 and RB8



#include "xc.h"
#include "IOCInit.h"
#include "IOInit.h"
#include "timer.h"
#include "ADC.h"
#include "uart.h"


#define DEBOUNCE_MS 50
#define NUM_BUTTONS 3

uint8_t button_state[NUM_BUTTONS] = {0};     // stable state (debounced)
uint8_t button_pressed[NUM_BUTTONS] = {0};   // flag for new press
uint8_t debounce_counter[NUM_BUTTONS] = {0}; // stability counter

uint8_t PB0F = 0;
uint8_t PB1F = 0;
uint8_t PB2F = 0;
uint8_t STATE_CHANGE = 0;
uint8_t raw0, raw1, raw2;

int main(void) {
   
    IOinit();
    IOCinit();
    TM2init();
    TM3init();
    InitUART2();
    ClearTerminal();
   
    while(1){
    }
   
   
}


void __attribute__ ((interrupt, no_auto_psv)) _IOCInterrupt(void) {
   
    // set flag for while(1) loop to execute next state logic
    STATE_CHANGE = 1;
   
    LED0 = 0;
   
    // CNflag used to exit RecvUartChar while(1) loop
    CNflag = 1;
    IFS1bits.IOCIF = 0;

   
}

void __attribute__((interrupt, no_auto_psv)) _T2Interrupt(void){
    //Don't forget to clear the timer 2 interrupt flag!
    IFS0bits.T2IF = 0; // reset flag
    T2CONbits.TON = 0; // turn timer OFF
}


void __attribute__((interrupt, no_auto_psv)) _T3Interrupt(void) {
    IFS0bits.T3IF = 0;

    // Sample each button directly
    raw0 = PB0;
    raw1 = PB1;
    raw2 = PB2;

    // Button 0
    if (raw0 == button_state[0]) {
        if (debounce_counter[0] < DEBOUNCE_MS) debounce_counter[0]++;
    } else {
        debounce_counter[0] = 0;
        button_state[0] = raw0;
    }
    if (debounce_counter[0] >= DEBOUNCE_MS && raw0 == 1)
        button_pressed[0] = 1;

    // Button 1
    if (raw1 == button_state[1]) {
        if (debounce_counter[1] < DEBOUNCE_MS) debounce_counter[1]++;
    } else {
        debounce_counter[1] = 0;
        button_state[1] = raw1;
    }
    if (debounce_counter[1] >= DEBOUNCE_MS && raw1 == 1)
        button_pressed[1] = 1;

    // Button 2
    if (raw2 == button_state[2]) {
        if (debounce_counter[2] < DEBOUNCE_MS) debounce_counter[2]++;
    } else {
        debounce_counter[2] = 0;
        button_state[2] = raw2;
    }
    if (debounce_counter[2] >= DEBOUNCE_MS && raw2 == 1)
        button_pressed[2] = 1;
    T3CONbits.TON = 1;
}

///// ADC interrupt subroutine
void __attribute__((interrupt, no_auto_psv)) _ADC1Interrupt(void)
{
IFS0bits.AD1IF = 0; // Clear the ADC1 Interrupt Flag
}
