#ifndef MCAL_TIMER_TIMER_PRIVATE_H_
#define MCAL_TIMER_TIMER_PRIVATE_H_

#include "../../LIB/STD_TYPES.h"

/* Timer0 Registers */
#define TCCR0_Register    *((volatile u8*)0x53)
#define TCNT0_Register    *((volatile u8*)0x52)
#define OCR0_Register     *((volatile u8*)0x5C)

/* Timer1 Registers */
#define TCCR1A_Register   *((volatile u8*)0x4F)
#define TCCR1B_Register   *((volatile u8*)0x4E)

#define TCNT1_Register    *((volatile u16*)0x4C)
#define OCR1A_Register    *((volatile u16*)0x4A)
#define OCR1B_Register    *((volatile u16*)0x48)
#define ICR1_Register     *((volatile u16*)0x46)

/* Interrupt Registers */
#define TIMSK_Register    *((volatile u8*)0x59)
#define TIFR_Register     *((volatile u8*)0x58)

#endif
