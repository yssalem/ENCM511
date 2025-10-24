/*
 * File:   main.c
 * Author: Ibrahim Khalid, Noor Sheikh, Youssef Salem
 *
 * Created FOR ENCM 511
 * PLEASE ADD DATE CREATED HERE: 2025-10-14
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
#include "uart.h"
#define BUFMAX  63
#define LED0    _LATB5
#define LED1    _LATB6 
#define LED2    _LATB7
#define PB0     _RB8
#define PB1     _RA4
#define PB2     _RB4
/*#define TESTSENDWW */

/**
 * You might find it useful to add your own #defines to improve readability here
 */

uint16_t slow = 0;
char received;

/*6 state FSM:*/
typedef enum {IDLE, PB0P, PB1P, PB2P, BOTHP} state_t;
/*2 state Flag --> Does NOT count towards the FSM, only a flag to be checked*/
typedef enum {PROG, FAST} mode_t;

/*start state in NONE*/
state_t state = IDLE;
state_t previous_state;
/*program should start in fast mode*/
mode_t mode = FAST;

uint8_t PB0F = 0;
uint8_t PB1F = 0;
uint8_t PB2F = 0;

uint8_t STATE_CHANGE = 0;

uint8_t time_settings[3] = {250, 500, 1000};
uint8_t time_setting = 'X';


int main(void) {

    // IOC config
    
    ClearTerminal();
    TM2init();
    TM3init();
    IOCinit();
    InitUART2();
    IOinit();
    ClearTerminal();
    while(1) {
        
        ClearTerminal();
        
        if (STATE_CHANGE) {
            
            IEC1bits.IOCIE = 0;
            delay_ms_3(75);
            
            state_t previous_state = state;
            
            if ((state == IDLE) && (PB0F & PB1F & PB2F)) 
                mode ^= 1;
            else if (state == BOTHP) 
                state = IDLE;
            else {
                if ((PB0F & PB1F) | (PB0F & PB2F) | (PB1F & PB2F)) {
                    state = BOTHP;
                } else if (PB0F) {
                    state = PB0P;
                } else if (PB1F) {
                    state = PB1P;
                } else if (PB2F) {
                    state = PB2P;
                }

                if (state == previous_state) {
                    state = IDLE;
                }
            }
            
            STATE_CHANGE = 0;
            
            IEC1bits.IOCIE = 1;
        }
        
        switch (state) {
            case IDLE:
                
                if (mode == FAST) {

                    Disp2String("Fast Mode: IDLE");
                    Idle();
                    
                } else {

                    Disp2String("Prog Mode: IDLE");
                    Idle();
                }
                
                break;
            case PB0P:
                if(mode == FAST){
                    Disp2String("Fast Mode: PB0 was pressed");
                    LED0 ^= 1;
                    delay_ms(250);
                } else{
                    Disp2String("Prog Mode: PB0 was pressed");
                    LED0 ^= 1;
                    delay_ms(3000);
                }
                break;
            case PB1P:
                if(mode == FAST){
                    Disp2String("Fast Mode: PB1 was pressed");
                    LED0 ^= 1;
                    delay_ms(500);
                }else{
                    Disp2String("Prog Mode: PB1 was pressed, setting ");
                    XmitUART2(time_setting, 1);
                    LED0 ^= 1;
                    delay_ms(time_settings[time_setting - 48]);
                }
                break;
            case PB2P:
                if(mode == FAST){
                    Disp2String("Fast Mode: PB2 was pressed");
                    LED0 ^= 1;
                    delay_ms(1000);
                }else{
                    
                }
                break;
            case BOTHP:
                if(mode == FAST){
                    if (PB0F & PB1F) Disp2String("Fast Mode: PB0 and PB1 were pressed");
                    else if (PB0F & PB2F) Disp2String("Fast Mode: PB0 and PB2 were pressed");
                    else if (PB1F & PB2F) Disp2String("Fast Mode: PB1 and PB2 were pressed");

                    LED0 = 1;
                    Idle();
                }
        }
    }
    
    return 0;
}

void IOinit(void){
    TRISBbits.TRISB5 = 0;
    TRISBbits.TRISB6 = 0;
    
    LED0 = 0;
    
    // set pin for button
    //PB0
    TRISBbits.TRISB8 = 1;
    IOCPUBbits.CNPUB8 = 1;
    //PB1
    TRISAbits.TRISA4 = 1;
    IOCPUAbits.CNPUA4 = 1;
    //PB2
    TRISBbits.TRISB4 = 1;
    IOCPUBbits.CNPUB4 = 1;
}

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
    IOCSTATbits.IOCPBF = 0;
    
    IFS1bits.IOCIF = 0;
    IPC4bits.IOCIP = 3;
    IEC1bits.IOCIE = 1;
}

void TM2init() {
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

void TM3init() {
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

void __attribute__ ((interrupt, no_auto_psv)) _IOCInterrupt(void) {
    
//    PR3 = (float)(15625/1000) * 75.0;
//    
//    TMR3 = 0;           // reset timer count value
//    T3CONbits.TON = 1;  // turn timer ON
//    Idle();             // wait for interrupt
    
    STATE_CHANGE = 1;
    
    ClearTerminal();
    
    LED0 = 0;

    

    
//    PB0F = 0;
//    PB1F = 0;
//    PB2F = 0;
//    
    IFS1bits.IOCIF = 0;

    
}

void __attribute__((interrupt, no_auto_psv)) _T2Interrupt(void){
    //Don't forget to clear the timer 2 interrupt flag!
    IFS0bits.T2IF = 0; // reset flag
    T2CONbits.TON = 0; // turn timer OFF
}

void __attribute__((interrupt, no_auto_psv)) _T3Interrupt(void){
    //Don't forget to clear the timer 2 interrupt flag!
    
    PB0F = !PB0;
    PB1F = !PB1;
    PB2F = !PB2;
    
    IFS0bits.T3IF = 0; // reset flag
    T3CONbits.TON = 0; // turn timer OFF
}

void delay_ms(uint16_t ms) {
    
    // set timer2 count limit
    // timer frequency of 15625 due to prescaler defined earlier
    // freq/1000 to convert ms to s
    PR2 = (float)(15625/1000) * (float)ms;
    
    TMR2 = 0;           // reset timer count value
    T2CONbits.TON = 1;  // turn timer ON
    Idle();             // wait for interrupt
}

void delay_ms_3(uint16_t ms) {
    
    // set timer3 count limit
    // timer frequency of 15625 due to prescaler defined earlier
    // freq/1000 to convert ms to s
    PR3 = (float)(15625/1000) * (float)ms;
    
    TMR3 = 0;           // reset timer count value
    T3CONbits.TON = 1;  // turn timer ON
    Idle();             // wait for interrupt
}
