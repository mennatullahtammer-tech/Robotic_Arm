#include "PWM_Interface.h"
#include "PWM_Config.h"
#include "PWM_Private.h"

#include "../../LIB/BIT_Math.h"


/* =====================================================
 *                  PWM INITIALIZATION
 * ===================================================== */

void PWM_voidInit(void)
{
    /*
     * Timer1
     * Fast PWM Mode 14
     *
     * WGM13 = 1
     * WGM12 = 1
     * WGM11 = 1
     * WGM10 = 0
     */

    SET_BIT(TCCR1A_Register, 1);
    CLR_BIT(TCCR1A_Register, 0);

    SET_BIT(TCCR1B_Register, 4);
    SET_BIT(TCCR1B_Register, 3);


    /*
     * Channel A
     * Non-Inverting PWM
     */

    SET_BIT(TCCR1A_Register, 7);
    CLR_BIT(TCCR1A_Register, 6);


    /*
     * Channel B
     * Non-Inverting PWM
     */

    SET_BIT(TCCR1A_Register, 5);
    CLR_BIT(TCCR1A_Register, 4);


    /*
     * TOP = 19999
     *
     * Frequency = 50 Hz
     */

    ICR1_Register = PWM_TIMER1_TOP;


    /*
     * Initial Servo Position
     *
     * 1 ms = 0 degree
     */

    OCR1A_Register = 1000;
    OCR1B_Register = 1000;


    /*
     * Prescaler = 8
     */

    CLR_BIT(TCCR1B_Register, 2);
    SET_BIT(TCCR1B_Register, 1);
    CLR_BIT(TCCR1B_Register, 0);
}


/* =====================================================
 *                 SET COMPARE CHANNEL A
 * ===================================================== */

void PWM_voidSetCompareA(u16 Copy_u16Value)
{
    if(Copy_u16Value <= PWM_TIMER1_TOP)
    {
        OCR1A_Register = Copy_u16Value;
    }
}


/* =====================================================
 *                 SET COMPARE CHANNEL B
 * ===================================================== */

void PWM_voidSetCompareB(u16 Copy_u16Value)
{
    if(Copy_u16Value <= PWM_TIMER1_TOP)
    {
        OCR1B_Register = Copy_u16Value;
    }
}
