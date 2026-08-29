#include "SERVO_Interface.h"

#include "../../MCAL/PWM/PWM_Interface.h"
#include "../../MCAL/TIMER/TIMER_Interface.h"

#include "../../MCAL/DIO/DIO_Interface.h"

#include "../../LIB/STD_TYPES.h"


/* =====================================================
 *                  SERVO INITIALIZATION
 * ===================================================== */

void SERVO_voidInit(void)
{
    /* Servo 1 -> PD5 -> OC1A */

    DIO_enumSetPinDirection(
        DIO_PORTD,
        DIO_PIN5,
        DIO_PIN_OUTPUT
    );


    /* Servo 2 -> PD4 -> OC1B */

    DIO_enumSetPinDirection(
        DIO_PORTD,
        DIO_PIN4,
        DIO_PIN_OUTPUT
    );


    /* Servo 3 -> PD7 */

    DIO_enumSetPinDirection(
        DIO_PORTD,
        DIO_PIN7,
        DIO_PIN_OUTPUT
    );


    /* Initialize PWM Timer1 */

    PWM_voidInit();


    /* Initialize Timer2 for Servo 3 */

    TIMER2_voidServo3Init();
}


/* =====================================================
 *                     SERVO 1
 * ===================================================== */

void SERVO_voidSetAngle1(u8 Copy_u8Angle)
{
    u16 Local_u16OCRValue;


    if(Copy_u8Angle <= 180)
    {
        /*
         * 0 degree   -> 1000
         * 180 degree -> 2000
         */

        Local_u16OCRValue =
            1000 +
            (((u32)Copy_u8Angle * 1000UL) / 180UL);


        PWM_voidSetCompareA(Local_u16OCRValue);
    }
}


/* =====================================================
 *                     SERVO 2
 * ===================================================== */

void SERVO_voidSetAngle2(u8 Copy_u8Angle)
{
    u16 Local_u16OCRValue;


    if(Copy_u8Angle <= 180)
    {
        /*
         * 0 degree   -> 1000
         * 180 degree -> 2000
         */

        Local_u16OCRValue =
            1000 +
            (((u32)Copy_u8Angle * 1000UL) / 180UL);


        PWM_voidSetCompareB(Local_u16OCRValue);
    }
}


/* =====================================================
 *                     SERVO 3
 * ===================================================== */

void SERVO_voidSetAngle3(u8 Copy_u8Angle)
{
    if(Copy_u8Angle <= 180)
    {
        TIMER2_voidSetServo3Pulse(Copy_u8Angle);
    }
}
