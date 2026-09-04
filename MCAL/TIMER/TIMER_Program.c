#include "TIMER_Interface.h"
#include "TIMER_Config.h"
#include "TIMER_Private.h"

#include "../../LIB/BIT_Math.h"
#include "../../LIB/STD_TYPES.h"

#include <avr/io.h>
#include <avr/interrupt.h>


/* =========================================================
 *                      TIMER 2
 *                    SERVO 3 CONTROL
 *
 * F_CPU = 8 MHz
 * Prescaler = 8
 *
 * Timer Clock = 1 MHz
 * Timer Tick  = 1 us
 *
 * OCR2 = 99
 * Compare Match = 100 us
 *
 * 200 Compare Matches = 20 ms
 * Servo Frequency = 50 Hz
 *
 * Servo:
 * 0°   = 1.0 ms
 * 90°  = 1.5 ms
 * 180° = 2.0 ms
 *
 * Servo 3 → PD7
 * ========================================================= */


/* Number of 100 us ticks */
static volatile u8 TIMER2_u8Servo3PulseTicks = 10;


/* 0 → 199 = one 20 ms frame */
static volatile u8 TIMER2_u8Counter = 0;


/* =========================================================
 *                 TIMER 2 SERVO 3 INIT
 * ========================================================= */

void TIMER2_voidServo3Init(void)
{
    /* PD7 → OUTPUT */
    SET_BIT(DDRD, 7);

    /* Initial output LOW */
    CLR_BIT(PORTD, 7);


    /* =====================================================
     * Timer2 CTC Mode
     *
     * WGM22 = 0
     * WGM21 = 1
     * WGM20 = 0
     * ===================================================== */

    CLR_BIT(TCCR2_Register, 6);   /* WGM20 */
    SET_BIT(TCCR2_Register, 3);   /* WGM21 */


    /* Compare value = 99
     * 100 counts × 1 us = 100 us
     */
    OCR2_Register = 99;


    /* Reset counter */
    TCNT2_Register = 0;


    /* =====================================================
     * Prescaler = 8
     *
     * CS22 = 0
     * CS21 = 1
     * CS20 = 0
     * ===================================================== */

    CLR_BIT(TCCR2_Register, 2);
    SET_BIT(TCCR2_Register, 1);
    CLR_BIT(TCCR2_Register, 0);


    /* =====================================================
     * Enable Timer2 Compare Match Interrupt
     *
     * TIMSK bit 7 = OCIE2
     * ===================================================== */

    SET_BIT(TIMSK_Register, 7);


    /* Initial Servo Values */
    TIMER2_u8Servo3PulseTicks = 10;
    TIMER2_u8Counter = 0;
}


/* =========================================================
 *                SET SERVO 3 ANGLE
 * ========================================================= */

void TIMER2_voidSetServo3Pulse(u8 Copy_u8Angle)
{
    if(Copy_u8Angle <= 180)
    {
        /*
         * 0°   → 10 × 100 us = 1 ms
         * 90°  → 15 × 100 us = 1.5 ms
         * 180° → 20 × 100 us = 2 ms
         */

        TIMER2_u8Servo3PulseTicks =
                10 +
                (((u16)Copy_u8Angle * 10U) / 180U);
    }
}


/* =========================================================
 *                  TIMER 2 ISR
 * ========================================================= */

ISR(TIMER2_COMP_vect)
{
    /*
     * Start of new 20 ms frame
     *
     * Counter = 0
     * → PD7 HIGH
     */
    if(TIMER2_u8Counter == 0)
    {
        SET_BIT(PORTD, 7);
    }


    /*
     * End of Servo pulse
     *
     * Example:
     * 10 ticks → 1 ms
     * 15 ticks → 1.5 ms
     * 20 ticks → 2 ms
     */
    if(TIMER2_u8Counter == TIMER2_u8Servo3PulseTicks)
    {
        CLR_BIT(PORTD, 7);
    }


    /* Move to next 100 us period */
    TIMER2_u8Counter++;


    /* 200 × 100 us = 20 ms */
    if(TIMER2_u8Counter >= 200)
    {
        TIMER2_u8Counter = 0;
    }
}
