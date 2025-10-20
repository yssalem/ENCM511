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
#define PB2     _RB9
/*#define TESTSENDWW */

/**
 * You might find it useful to add your own #defines to improve readability here
 */

uint16_t slow = 0;
char received;

/*6 state FSM:*/
typedef enum {NONE, PB0P, PB1P, PB2P, BOTHP, THREEP} state_t;
/*2 state Flag --> Does NOT count towards the FSM, only a flag to be checked*/
typedef enum {PROG, FAST} mode_t;

/*start state in NONE*/
state_t state = NONE;
/*program should start in fast mode*/
mode_t mode = FAST;

int main(void) {
    /*used for receiving from the terminal*/
    char recvString[BUFMAX];
    
    // IOC config
    // PB0
    IOCNBbits.IOCNB8 = 1;
    IOCPBbits.IOCPB8 = 1;
    //PB1
    IOCNAbits.IOCNA4 = 1;
    IOCPAbits.IOCPA4 = 1;
    //PB2
    IOCNBbits.IOCNB9 = 1;
    IOCPBbits.IOCPB9 = 1;
    
    PADCONbits.IOCON = 1;
    IOCSTATbits.IOCPBF = 0;
    
    IFS1bits.IOCIF = 0;
    IPC4bits.IOCIP = 3;
    IEC1bits.IOCIE = 1;
    
    InitUART2();
    IOinit();
    ClearTerminal();
    while(1) {
        switch(state){
            case NONE:
                /*no buttons pressed*/

                LED0 = 0;
                LED1 = 0;
                LED2 = 0;
                /*PRINT {MODE} mode: IDLE*/
                if(mode == PROG){Disp2String("Prog Mode: IDLE");}
                else{Disp2String("Fast Mode: IDLE");}
                Idle();
                break;
            case PB0P:
                /**/
                if(mode == FAST){
                
                }else{
                
                }
                break;
            case PB1P:
                /**/
                if(mode == FAST){
                
                }else{
                
                }
                break;
            case PB2P:
                /**/
                if(mode == FAST){
                
                }else{
                
                }
                break;                
            case BOTHP:
                /**/
                if(mode == FAST){
                }else{
                
                }
                break;
            case THREEP:
                /**/
                LED0 = 0;
                LED1 = 0;
                LED2 = 0;
                mode ^= 1;
        }
    }
    
    return 0;
}

void IOinit(void){
    /* Configure Output Pins */
    _TRISB5 = 0; /*LED0*/
    _TRISB6 = 0; /*LED1*/
    _TRISB7 = 0; /*LED2*/
    
    /* Configure Input Pins */
    _TRISA4 = 1;
    _TRISB8 = 1;
    _TRISB9 = 1;
    
    IOCPUAbits.CNPUA4 = 1;
    IOCPUBbits.CNPUB8 = 1;
    IOCPUBbits.CNPUB9 = 1;
}

void __attribute__ ((interrupt, no_auto_psv)) _IOCInterrupt(void) {
    /*if (PB2 == 0) 
     * PB2_BLINK_RATE = PB2_BLINK_RATE > 125 ? PB2_BLINK_RATE / 2 : 4000;
     */
    uint8_t pressed = (~PB0 & 1) + (~PB1 & 1) + (~PB2 & 1);
    if (pressed == 0) {
        state = NONE;
    }   
    else if(pressed == 3);
    else if(pressed == 2){
        state = BOTHP;
    }
    else{
        /*implement change logic when only 1 is pressed*/
    }
    IFS1bits.IOCIF = 0;
    
}



//#ifdef TESTSEND
//        for(count = '0'; count <= '9'; count ++){
//            XmitUART2(count,5);
//            /*XmitUART2('\r',5);
//            XmitUART2('\n', 5);
//            */
//        }
//        for (slow = 0; slow < 4000; slow++) {
//            
//        }
//#endif
//        
//#ifndef TESTSEND
//        Disp2String("Enter a character (hit enter to receive): ");
//        RecvUart(recvString, BUFMAX);
//        Disp2String("\r\n you entered ");
//        Disp2String(recvString);
//        XmitUART2('\r',1);
//        XmitUART2('\n',1);
//        
//        
//#endif
