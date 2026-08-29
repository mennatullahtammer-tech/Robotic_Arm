#include "STEPPER_Interface.h"
#include "STEPPER_Config.h"
#include "STEPPER_Private.h"

#include "../../MCAL/DIO/DIO_Interface.h"

#include <util/delay.h>


/* =====================================================
 *              CURRENT STEPPER POSITION
 * ===================================================== */

static s16 STEPPER_s16CurrentAngle = 0;


/* =====================================================
 *                  STEPPER INIT
 * ===================================================== */

void STEPPER_voidInit(void)
{
    /* ================================================
     * Set Stepper Pins as OUTPUT
     * ================================================ */

    DIO_enumSetPinDirection(
        STEPPER_PORT,
        STEPPER_PIN1,
        DIO_PIN_OUTPUT
    );

    DIO_enumSetPinDirection(
        STEPPER_PORT,
        STEPPER_PIN2,
        DIO_PIN_OUTPUT
    );

    DIO_enumSetPinDirection(
        STEPPER_PORT,
        STEPPER_PIN3,
        DIO_PIN_OUTPUT
    );

    DIO_enumSetPinDirection(
        STEPPER_PORT,
        STEPPER_PIN4,
        DIO_PIN_OUTPUT
    );


    /* ================================================
     * Turn OFF all coils
     * ================================================ */

    STEPPER_voidStop();


    /* ================================================
     * Initial position
     * ================================================ */

    STEPPER_s16CurrentAngle = 0;
}


/* =====================================================
 *                    SINGLE STEP
 * ===================================================== */

void STEPPER_voidStep(u8 Copy_u8Step)
{
    switch(Copy_u8Step)
    {

        /* =============================================
         * STEP 0
         *
         * 1100
         * ============================================= */

        case STEPPER_STEP_0:

            DIO_enumSetPinValue(
                STEPPER_PORT,
                STEPPER_PIN1,
                DIO_PIN_HIGH
            );

            DIO_enumSetPinValue(
                STEPPER_PORT,
                STEPPER_PIN2,
                DIO_PIN_HIGH
            );

            DIO_enumSetPinValue(
                STEPPER_PORT,
                STEPPER_PIN3,
                DIO_PIN_LOW
            );

            DIO_enumSetPinValue(
                STEPPER_PORT,
                STEPPER_PIN4,
                DIO_PIN_LOW
            );

            break;


        /* =============================================
         * STEP 1
         *
         * 0110
         * ============================================= */

        case STEPPER_STEP_1:

            DIO_enumSetPinValue(
                STEPPER_PORT,
                STEPPER_PIN1,
                DIO_PIN_LOW
            );

            DIO_enumSetPinValue(
                STEPPER_PORT,
                STEPPER_PIN2,
                DIO_PIN_HIGH
            );

            DIO_enumSetPinValue(
                STEPPER_PORT,
                STEPPER_PIN3,
                DIO_PIN_HIGH
            );

            DIO_enumSetPinValue(
                STEPPER_PORT,
                STEPPER_PIN4,
                DIO_PIN_LOW
            );

            break;


        /* =============================================
         * STEP 2
         *
         * 0011
         * ============================================= */

        case STEPPER_STEP_2:

            DIO_enumSetPinValue(
                STEPPER_PORT,
                STEPPER_PIN1,
                DIO_PIN_LOW
            );

            DIO_enumSetPinValue(
                STEPPER_PORT,
                STEPPER_PIN2,
                DIO_PIN_LOW
            );

            DIO_enumSetPinValue(
                STEPPER_PORT,
                STEPPER_PIN3,
                DIO_PIN_HIGH
            );

            DIO_enumSetPinValue(
                STEPPER_PORT,
                STEPPER_PIN4,
                DIO_PIN_HIGH
            );

            break;


        /* =============================================
         * STEP 3
         *
         * 1001
         * ============================================= */

        case STEPPER_STEP_3:

            DIO_enumSetPinValue(
                STEPPER_PORT,
                STEPPER_PIN1,
                DIO_PIN_HIGH
            );

            DIO_enumSetPinValue(
                STEPPER_PORT,
                STEPPER_PIN2,
                DIO_PIN_LOW
            );

            DIO_enumSetPinValue(
                STEPPER_PORT,
                STEPPER_PIN3,
                DIO_PIN_LOW
            );

            DIO_enumSetPinValue(
                STEPPER_PORT,
                STEPPER_PIN4,
                DIO_PIN_HIGH
            );

            break;


        default:

            STEPPER_voidStop();

            break;
    }
}


/* =====================================================
 *                    STOP STEPPER
 * ===================================================== */

void STEPPER_voidStop(void)
{
    DIO_enumSetPinValue(
        STEPPER_PORT,
        STEPPER_PIN1,
        DIO_PIN_LOW
    );

    DIO_enumSetPinValue(
        STEPPER_PORT,
        STEPPER_PIN2,
        DIO_PIN_LOW
    );

    DIO_enumSetPinValue(
        STEPPER_PORT,
        STEPPER_PIN3,
        DIO_PIN_LOW
    );

    DIO_enumSetPinValue(
        STEPPER_PORT,
        STEPPER_PIN4,
        DIO_PIN_LOW
    );
}


void STEPPER_voidMoveCW(u16 Copy_u16Steps)
{
    u16 i;

    for(i = 0; i < Copy_u16Steps; i++)
    {
        /* Step 1 */
        STEPPER_voidStep(0);
        _delay_ms(100);

        /* Step 2 */
        STEPPER_voidStep(1);
        _delay_ms(100);

        /* Step 3 */
        STEPPER_voidStep(2);
        _delay_ms(100);

        /* Step 4 */
        STEPPER_voidStep(3);
        _delay_ms(100);
    }

    STEPPER_voidStop();
}


void STEPPER_voidMoveCCW(u16 Copy_u16Steps)
{
    u16 i;

    for(i = 0; i < Copy_u16Steps; i++)
    {
        /* Step 4 */
        STEPPER_voidStep(3);
        _delay_ms(100);

        /* Step 3 */
        STEPPER_voidStep(2);
        _delay_ms(100);

        /* Step 2 */
        STEPPER_voidStep(1);
        _delay_ms(100);

        /* Step 1 */
        STEPPER_voidStep(0);
        _delay_ms(100);
    }

    STEPPER_voidStop();
}
/* =====================================================
 *              MOVE ANGLE CLOCKWISE
 * ===================================================== */

void STEPPER_voidMoveAngleCW(u16 Copy_u16Angle)
{
    u16 Local_u16Steps;


    /*
     * Step Angle = 90 degree
     *
     * Steps = Angle / 90
     */

    Local_u16Steps =
        Copy_u16Angle / STEPPER_STEP_ANGLE;


    STEPPER_voidMoveCW(Local_u16Steps);


    /* Update current angle */

    STEPPER_s16CurrentAngle +=
        Local_u16Steps * STEPPER_STEP_ANGLE;


    if(STEPPER_s16CurrentAngle >= 360)
    {
        STEPPER_s16CurrentAngle -= 360;
    }
}


/* =====================================================
 *            MOVE ANGLE COUNTER CLOCKWISE
 * ===================================================== */

void STEPPER_voidMoveAngleCCW(u16 Copy_u16Angle)
{
    u16 Local_u16Steps;


    /*
     * Steps = Angle / 90
     */

    Local_u16Steps =
        Copy_u16Angle / STEPPER_STEP_ANGLE;


    STEPPER_voidMoveCCW(Local_u16Steps);


    /* Update current angle */

    STEPPER_s16CurrentAngle -=
        Local_u16Steps * STEPPER_STEP_ANGLE;


    if(STEPPER_s16CurrentAngle < 0)
    {
        STEPPER_s16CurrentAngle += 360;
    }
}


/* =====================================================
 *                  MOVE TO ANGLE
 * ===================================================== */

void STEPPER_voidMoveToAngle(s16 Copy_s16TargetAngle)
{
    s16 Local_s16Difference;


    /* ================================================
     * Normalize target angle
     * ================================================ */

    while(Copy_s16TargetAngle >= 360)
    {
        Copy_s16TargetAngle -= 360;
    }


    while(Copy_s16TargetAngle < 0)
    {
        Copy_s16TargetAngle += 360;
    }


    /* ================================================
     * Calculate difference
     * ================================================ */

    Local_s16Difference =
        Copy_s16TargetAngle -
        STEPPER_s16CurrentAngle;


    /* ================================================
     * Clockwise
     * ================================================ */

    if(Local_s16Difference > 0)
    {
        STEPPER_voidMoveAngleCW(
            (u16)Local_s16Difference
        );
    }


    /* ================================================
     * Counter Clockwise
     * ================================================ */

    else if(Local_s16Difference < 0)
    {
        STEPPER_voidMoveAngleCCW(
            (u16)(-Local_s16Difference)
        );
    }


    /* ================================================
     * Already at target
     * ================================================ */

    else
    {
        STEPPER_voidStop();
    }
}


/* =====================================================
 *                GET CURRENT ANGLE
 * ===================================================== */

s16 STEPPER_s16GetCurrentAngle(void)
{
    return STEPPER_s16CurrentAngle;
}
