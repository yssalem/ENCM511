/* 
 * File:   IOinit.h
 * Author: youss
 *
 * Created on October 24, 2025, 5:48 PM
 */

#ifndef IOINIT_H
#define	IOINIT_H

#include <xc.h>

#define LED0    _LATB5
#define PB0     _RB8
#define PB1     _RA4
#define PB2     _RB4

void IOinit(void);

#ifdef	__cplusplus
extern "C" {
#endif




#ifdef	__cplusplus
}
#endif

#endif	/* IOINIT_H */
