#include "TIMER_Interface.h"
#include "TIMER_Config.h"
#include "TIMER_Private.h"

#include "../../LIB/BIT_Math.h"
#include "../../LIB/STD_TYPES.h"
#include <avr/io.h>
#include <avr/interrupt.h>


/* =========================================================
 *                      TIMER 2
 *                      SERVO 3
 * ========================================================= */

/*
 * F_CPU = 8 MHz
 * Prescaler = 8
 *
 * Timer tick = 1 us
 *
 * OCR2 = 99
 * Compare Match = 100 us
 *
 * 200 x 100 us = 20 ms
 * Frequency = 50 Hz
 */

static volatile u8 TIMER2_u8Servo3PulseTicks = 10;
static volatile u8 TIMER2_u8Counter = 0;


/* =========================================================
 *                 TIMER2 SERVO3 INIT
 * ========================================================= */

void TIMER2_voidServo3Init(void)
{
    /* PD7 = OUTPUT */

    SET_BIT(DDRD, 7);


    /* Timer2 CTC Mode */

    CLR_BIT(TCCR2_Register, 6);
    SET_BIT(TCCR2_Register, 3);


    /* Compare value = 99 */

    OCR2_Register = 99;


    /* Prescaler = 8 */

    CLR_BIT(TCCR2_Register, 2);
    SET_BIT(TCCR2_Register, 1);
    CLR_BIT(TCCR2_Register, 0);


    /* Enable Timer2 Compare Match Interrupt */

    SET_BIT(TIMSK_Register, 7);


    /* Initial values */

    TIMER2_u8Servo3PulseTicks = 10;
    TIMER2_u8Counter = 0;


    /* Servo 3 LOW initially */

    CLR_BIT(PORTD, 7);
}


/* =========================================================
 *                SET SERVO3 ANGLE
 * ========================================================= */

void TIMER2_voidSetServo3Pulse(u8 Copy_u8Angle)
{
    if(Copy_u8Angle <= 180)
    {
        /*
         * 0 degree   = 10 ticks = 1 ms
         * 180 degree = 20 ticks = 2 ms
         */

        TIMER2_u8Servo3PulseTicks =
            10 +
            (((u16)Copy_u8Angle * 10) / 180);
    }
}


/* =========================================================
 *                  TIMER2 ISR
 * ========================================================= */

ISR(TIMER2_COMP_vect)
{
    /* Start pulse */

    if(TIMER2_u8Counter == 0)
    {
        SET_BIT(PORTD, 7);
    }


    /* End pulse */

    if(TIMER2_u8Counter >= TIMER2_u8Servo3PulseTicks)
    {
        CLR_BIT(PORTD, 7);
    }


    /* Increment counter */

    TIMER2_u8Counter++;


    /* 200 x 100 us = 20 ms */

    if(TIMER2_u8Counter >= 200)
    {
        TIMER2_u8Counter = 0;
    }
}
