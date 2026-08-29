#ifndef MCAL_TIMER_TIMER_INTERFACE_H_
#define MCAL_TIMER_TIMER_INTERFACE_H_

#include "../../LIB/STD_TYPES.h"

/* ================= Timer1 - Servo 1 & Servo 2 ================= */

void TIMER1_voidInit(void);

void TIMER1_voidSetOCR1A(u16 Copy_u16Value);

void TIMER1_voidSetOCR1B(u16 Copy_u16Value);


/* ================= Timer2 - Servo 3 ================= */

void TIMER2_voidServo3Init(void);

void TIMER2_voidSetServo3Pulse(u8 Copy_u8Angle);

#endif
