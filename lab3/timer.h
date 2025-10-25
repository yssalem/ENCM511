/* 
 * File:   timer.h
 * Author: youss
 *
 * Created on October 24, 2025, 7:56 PM
 */

#ifndef TIMER_H
#define	TIMER_H

#include <xc.h>

void TM2init(void);
void TM3init(void);
void delay_ms(uint16_t ms);
void debounce(uint16_t ms);

#ifdef	__cplusplus
extern "C" {
#endif




#ifdef	__cplusplus
}
#endif

#endif	/* TIMER_H */

