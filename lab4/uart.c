/*
 * File:   uart.c
 * Author: psbta
 *
 * Created on September 27, 2025, 8:07 PM
 */


#include "uart.h"
#include "xc.h"
#include "timer.h"

uint8_t CNflag = 0;
uint8_t received_char = 0;
uint8_t RXFlag = 0;

void ClearTerminal(void) {
    Disp2String("\033[2J\033[H");
}

void InitUART2(void) 
{

    RPINR19bits.U2RXR = 11; // configures pin 22 as uart receive pin
    RPOR5bits.RP10R = 5;    // configures pin 21 as uart transmit pin

    U2MODE = 0b0000000010001000;

    U2BRG = 103;            // set baud rate to 9600
                            // Fcy / (4 * 9600) - 1 = 103 
                            // 4 used instead of 16 
                            // since BRGH = 1 in U2MODE above
    
	U2STAbits.UTXISEL0 = 0;
    U2STAbits.UTXISEL1 = 0;
    U2STAbits.URXEN = 1;
    U2STAbits.UTXEN = 1;
    U2STAbits.URXISEL = 0b00;

	IFS1bits.U2TXIF = 0;	
    IPC7bits.U2TXIP = 3; 
    
	IEC1bits.U2TXIE = 1; 
	IFS1bits.U2RXIF = 0; 
	IPC7bits.U2RXIP = 4; 
    IEC1bits.U2RXIE = 1;

	U2MODEbits.UARTEN = 1;	

	U2STAbits.UTXEN = 1;
	return;
}

void Disp2String(char *str) //Displays String of characters
{
    unsigned int i;
    for (i=0; i<= strlen(str); i++)
    {
        XmitUART2(str[i],1);
    }

    return;
}

void XmitUART2(char CharNum, unsigned int repeatNo)
{	
	U2STAbits.UTXEN = 1;
	while(repeatNo!=0) 
	{
		while(U2STAbits.UTXBF==1)   // We loop here because.... 
		{
		}	
		U2TXREG=CharNum;            // We put XXXXXX here because .....
		repeatNo--;
	}
	while(U2STAbits.TRMT==0)        // We check XXXXX because .........
	{
	}

    U2STAbits.UTXEN = 0;
}

/************************************************************************
 * Receive a buf_size number of characters over UART
 * Description: This function allows you to receive buf_size number of characters from UART,
 * provided the UART module has been initialized correctly. The function currently returns
 * if the "enter" key (ASCII 0x0D) is received. The function does not handle receiving
 * the DELETE or BACKSPACE keys meaningfully. 
 * 
 * Note: there is commented-out skeleton code that could be (re)implemented to allow the function
 * return early without the ENTER key given an interrupt-set global flag. 
 ************************************************************************/
void RecvUart(char* input, uint8_t buf_size)
{	
    uint16_t i = 0;
    char last_char;
    // wait for enter key
    while (last_char != 0x0D) {
        if (RXFlag == 1) {
            // only store alphanumeric characters
            if (received_char >= 32 && received_char <= 126) {
                if (i > buf_size-2) {
                    Disp2String("\ntoo long\n\r");
                    RXFlag = 0;
                    input[i] = '\0'; 
                    return;
                }
                input[i] = received_char;
                i++;
                XmitUART2(received_char,1); // loop back display
                U2STAbits.OERR = 0;
            }
            last_char = received_char;
            
            RXFlag = 0;
        }
        // wait for next character
        
        // if (CNflag == 1) { // this allows breaking out of the busy wait if CN interrupts are enabled...
        //     add logic here
        // }
    }
    input[i] = '\0'; 
}

/************************************************************************
 * Receive a single (alphanumeric) character over UART
 * Description: This function allows you to receive a single character, which will only be 
 * "accepted" (returned) after receiving the ENTER key (0x0D). 
 * While receiving characters, the program is designed to send back the received character.
 * To display this, it sends a BACKSPACE (0x08) to clear the previous character from the 
 * receiving terminal, before sending the new current character. 
 * 
 * Note: there is commented-out skeleton code that could be (re)implemented to allow the function
 * return early without the ENTER key given an interrupt-set global flag. 
 ************************************************************************/
char RecvUartChar()
{	
    char last_char;
    // wait for enter key
    while (1) { // changed from waiting for enter key to running indefinitely
        

        if (RXFlag == 1) {
            
            if (received_char == 0x0D) {
                RXFlag = 0;
                return last_char;
            }
            
            // bounds set to only allow '0' - '2'
            if (received_char >= 48 && received_char <= 50) {
                XmitUART2(0x08,1); // send backspace
                last_char = received_char;
                XmitUART2(received_char,1); // loop back display
            }
           
            U2STAbits.OERR = 0;
            RXFlag = 0;
        }
        
        // CNflag set by IOC ISR
        // used to exit infinite loop
        if (CNflag == 1) { // this allows breaking out of the busy wait if CN interrupts are enabled...
            CNflag = 0;
            break;
        }
        
        // flash LED at .125s on
        _LATB5 ^= 1;
        delay_ms(125);
    }
    
    // returns last character that was inputted
    return last_char;
}

void __attribute__ ((interrupt, no_auto_psv)) _U2RXInterrupt(void) {

	IFS1bits.U2RXIF = 0;
    
    received_char = U2RXREG;
    
    RXFlag = 1;
    
    _LATB5 ^= 1;
}

void __attribute__ ((interrupt, no_auto_psv)) _U2TXInterrupt(void) {
	IFS1bits.U2TXIF = 0;

}

void Disp2Hex(unsigned int DispData) {
    char i;
    char nib = 0x00;
    XmitUART2(' ', 1); // Disp Gap
    XmitUART2('0', 1); // Disp Hex notation 0x
    XmitUART2('x', 1);
    for (i = 3; i >= 0; i--) {
        nib = ((DispData >> (4 * i)) & 0x000F);
        if (nib >= 0x0A) {
            nib = nib + 0x37; //For Hex values A-F
        } else {
            nib = nib + 0x30; //For hex values 0-9
        }
        XmitUART2(nib, 1);
    }
    XmitUART2(' ', 1);
    DispData = 0x0000; // Clear DispData
    return;
}
// Displays 16 bit unsigned in in decimal form

void Disp2Dec(uint16_t Dec_num) {
    uint8_t rem; //remainder in div by 10
    uint16_t quot;
    uint8_t ctr = 0; //counter
    XmitUART2(' ', 1); // Disp Gap
    while (ctr < 5) {
        quot = Dec_num / (pow(10, (4 - ctr)));
        rem = quot % 10;
        XmitUART2(rem + 0x30, 1);
        ctr = ctr + 1;
    }
    XmitUART2(' ', 1); // Disp Gap
    return;
}
