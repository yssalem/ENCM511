/*
 * File:   main.c
 * Author: ENTER GROUP MEMBER NAME(S) HERE
 *
 * Created FOR ENCM 511
 * PLEASE ADD DATE CREATED HERE: 2025-XX-XX
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


#define LED0    LATBbits.LATB5
#define LED1    LATBbits.LATB6

#define PB2     PORTBbits.RB9
#define PB1     PORTAbits.RA4
#define PB0     PORTBbits.RB8

/**
 * You might find it useful to add your own #defines to improve readability here
 */


// Different states for FSM
typedef enum {
    STATE_NONE, STATE_PB0, STATE_PB1, STATE_BOTH
} state_t;

state_t state = STATE_NONE; // Initial FSM state

uint16_t PB2_BLINK_RATE = 4000; // Initial value for LED1 blink rate in seconds



int main(void) {
    
    /** This is usually where you would add run-once code
     * e.g., peripheral initialization. For the first labs
     * you might be fine just having it here. For more complex
     * projects, you might consider having one or more initialize() functions
     */
    
    ANSELA = 0x0000; /* keep this line as it sets I/O pins that can also be analog to be digital */
    ANSELB = 0x0000; /* keep this line as it sets I/O pins that can also be analog to be digital */
    
    
    
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
    
    // IOC config
    // PB0
    IOCNBbits.IOCNB8 = 1; // IOC high - low
    IOCPBbits.IOCPB8 = 1; // IOC low - high
    //PB1
    IOCNAbits.IOCNA4 = 1;
    IOCPAbits.IOCPA4 = 1;
    //PB2
    IOCNBbits.IOCNB9 = 1;
    IOCPBbits.IOCPB9 = 1;
    
    PADCONbits.IOCON = 1; // enables IOC
    IOCSTATbits.IOCPBF = 0;
    
    IFS1bits.IOCIF = 0;
    IPC4bits.IOCIP = 3;
    IEC1bits.IOCIE = 1;
    
    // Initialize IOs
    IOinit();
    
    /* Let's clear some flags */
    
    while(1) {        
        
        switch (state) {
            case STATE_NONE: // No buttons pressed - all LEDs OFF
                LED0 = 0;
                LED1 = 0;
                Idle();
                break;
            case STATE_PB0: // PB0 pressed - LED0 flashes at 0.25s
                LED0 ^= 1;
                delay_ms(250);
                break;
            case STATE_BOTH: // Both pressed - LED0 flashes at 0.5s
                LED0 ^= 1;
                delay_ms(500);
                break;
            case STATE_PB1: // PB1 pressed - LED1 flashes at PB2_BLINK_RATE
                LED1 ^= 1;
                delay_ms(PB2_BLINK_RATE);
                break;
               
        }
    }
    
    return 0;
}


// Timer 2 interrupt subroutine
void __attribute__((interrupt, no_auto_psv)) _T2Interrupt(void){
    //Don't forget to clear the timer 2 interrupt flag!
    IFS0bits.T2IF = 0; // reset flag
    T2CONbits.TON = 0; // turn timer OFF
}

void __attribute__ ((interrupt, no_auto_psv)) _IOCInterrupt(void) {
    
    // set both LEDs OFF
    LED0 = 0; 
    LED1 = 0;
    
    // divide PB2_BLINK_RATE by 2 if PB2 pressed until .125s
    if (PB2 == 0) 
        PB2_BLINK_RATE = PB2_BLINK_RATE > 125 ? PB2_BLINK_RATE / 2 : 4000;
   
    
    if ((PB0 == 1) && (PB1 == 1))       state = STATE_NONE; // no buttons pressed
    else if ((PB0 == 0) && (PB1 == 0))  state = STATE_BOTH; // both buttons pressed
    else if ((PB0 == 0))                state = STATE_PB0;  // PB0 pressed
    else if ((PB1 == 0))                state = STATE_PB1;  // PB1 pressed
    
    IFS1bits.IOCIF = 0; // clear IOC flag
    
}

void IOinit() {
    // set pins for LED
    TRISBbits.TRISB5 = 0;
    TRISBbits.TRISB6 = 0;
    
    LED0 = 0;
    LED1 = 0;
    
    // set pin for button
    //PB0
    TRISBbits.TRISB8 = 1;
    IOCPUBbits.CNPUB8 = 1;
    //PB1
    TRISAbits.TRISA4 = 1;
    IOCPUAbits.CNPUA4 = 1;
    //PB2
    TRISBbits.TRISB9 = 1;
    IOCPUBbits.CNPUB9 = 1;
}

void delay_ms(uint16_t ms) {
    
    // set timer2 count limit
    // timer frequency of 15625 due to prescaler defined earlier
    // freq/1000 to convert ms to s
    PR2 = (15625/1000) * ms;
    
    TMR2 = 0;           // reset timer count value
    T2CONbits.TON = 1;  // turn timer ON
    Idle();             // wait for interrupt
}

  
