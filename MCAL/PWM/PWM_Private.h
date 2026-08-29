#ifndef MCAL_PWM_PWM_PRIVATE_H_
#define MCAL_PWM_PWM_PRIVATE_H_

#include "../../LIB/STD_TYPES.h"

/* ================= Timer1 Registers ================= */

#define TCCR1A_Register   *((volatile u8*)0x4F)
#define TCCR1B_Register   *((volatile u8*)0x4E)

#define TCNT1_Register    *((volatile u16*)0x4C)

#define OCR1A_Register    *((volatile u16*)0x4A)
#define OCR1B_Register    *((volatile u16*)0x48)

#define ICR1_Register     *((volatile u16*)0x46)

#endif
