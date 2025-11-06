
#include <xc.h>
#include "uart.h"
#include "ADC.h"
#include "IOinit.h"
#include "IOCinit.h"
#include "timer.h"

uint16_t conv = 0;
long width = 0;
volatile char bar_character = 'x';

typedef enum {HEX, DEC} state_t;
state_t state = DEC;



int main() {
    
    TM2init();
    IOinit();
    IOCinit();
    ADCinit();
    InitUART2();
    
    while(1) {
        
        if (RXFlag) {
            switch (received_char) {
                case 'x':
                    state = HEX;
                    break;
                case 'd':
                    state = DEC;
                    break;
                default:
                    break;
            }
            
            RXFlag = 0;
        }
        
        ClearTerminal();
        conv = do_ADC();
        width = ((float)conv / 1024) * 48;
        XmitUART2(bar_character, width);
        Disp2String(" ");
        
        switch (state) {
            case HEX:
                Disp2Hex(conv);
                break;
            case DEC:
                Disp2Dec(conv);
                break;
        }
        delay_ms(1000);
        
    }
}


void __attribute__((interrupt, no_auto_psv)) _T2Interrupt(void){
    //Don't forget to clear the timer 2 interrupt flag!
    IFS0bits.T2IF = 0; // reset flag
    T2CONbits.TON = 0; // turn timer OFF
}

void __attribute__((interrupt, no_auto_psv)) _T3Interrupt(void){
    //Don't forget to clear the timer 3 interrupt flag!

    IFS0bits.T3IF = 0; // reset flag
    T3CONbits.TON = 0; // turn timer OFF
}

void __attribute__ ((interrupt, no_auto_psv)) _IOCInterrupt(void) {

    
    for (uint16_t i = 0; i < 20000; i++);
    
    if ((!PB0 && !PB1) || (!PB0 && !PB2) || (!PB1 && !PB2));
    else if (!PB0) bar_character = 'x';
    else if (!PB1) bar_character = '-';
    else if (!PB2) bar_character = '=';
    
    IFS1bits.IOCIF = 0;
    


}
