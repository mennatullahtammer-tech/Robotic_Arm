#include "GRIPPER_Interface.h"
#include "GRIPPER_Config.h"
#include "GRIPPER_Private.h"

#include "../../MCAL/DIO/DIO_Interface.h"

#include <avr/io.h>
#include <avr/interrupt.h>
#include <util/delay.h>


/* =====================================================
 *              CURRENT GRIPPER ANGLE
 * ===================================================== */

static volatile u8 GRIPPER_u8CurrentAngle = 0;


/* =====================================================
 *              TIMER0 VARIABLES
 * ===================================================== */

/*
 * Timer0:
 *
 * F_CPU = 8 MHz
 * Prescaler = 8
 *
 * Timer tick = 1 us
 *
 * OCR0 = 99
 *
 * Interrupt every 100 us
 *
 * 200 interrupts = 20 ms
 */

static volatile u8 GRIPPER_u8Counter = 0;

static volatile u8 GRIPPER_u8PulseTicks = 10;


/* =====================================================
 *                  GRIPPER INIT
 * ===================================================== */

void GRIPPER_voidInit(void)
{
    /* ================================================
     * Servo pin PB3 as OUTPUT
     * ================================================ */

    DIO_enumSetPinDirection(
        GRIPPER_SERVO_PORT,
        GRIPPER_SERVO_PIN,
        DIO_PIN_OUTPUT
    );


    /* ================================================
     * Open button as INPUT
     * ================================================ */

    DIO_enumSetPinDirection(
        GRIPPER_OPEN_BUTTON_PORT,
        GRIPPER_OPEN_BUTTON_PIN,
        DIO_PIN_INPUT
    );


    /* ================================================
     * Close button as INPUT
     * ================================================ */

    DIO_enumSetPinDirection(
        GRIPPER_CLOSE_BUTTON_PORT,
        GRIPPER_CLOSE_BUTTON_PIN,
        DIO_PIN_INPUT
    );


    /* ================================================
     * Enable Pull-Up resistors
     * ================================================ */

    DIO_enumSetPinValue(
        GRIPPER_OPEN_BUTTON_PORT,
        GRIPPER_OPEN_BUTTON_PIN,
        DIO_PIN_HIGH
    );

    DIO_enumSetPinValue(
        GRIPPER_CLOSE_BUTTON_PORT,
        GRIPPER_CLOSE_BUTTON_PIN,
        DIO_PIN_HIGH
    );


    /* ================================================
     * Timer0 CTC Mode
     * ================================================ */

    CLR_BIT(TCCR0, 6);
    SET_BIT(TCCR0, 3);


    /* ================================================
     * Compare value
     * ================================================ */

    OCR0 = 99;


    /* ================================================
     * Prescaler = 8
     * ================================================ */

    CLR_BIT(TCCR0, 2);
    SET_BIT(TCCR0, 1);
    CLR_BIT(TCCR0, 0);


    /* ================================================
     * Enable Timer0 Compare Interrupt
     *
     * OCIE0 = Bit 1
     * ================================================ */

    SET_BIT(TIMSK, 1);


    /* ================================================
     * Initial position = OPEN
     * ================================================ */

    GRIPPER_u8CurrentAngle = GRIPPER_OPEN_ANGLE;

    GRIPPER_u8PulseTicks = 10;

    GRIPPER_u8Counter = 0;


    /* Servo initially LOW */

    CLR_BIT(PORTB, 3);
}


/* =====================================================
 *                SET GRIPPER ANGLE
 * ===================================================== */

void GRIPPER_voidSetAngle(u8 Copy_u8Angle)
{
    u16 Local_u16Pulse;


    /* Limit angle */

    if(Copy_u8Angle > 180)
    {
        Copy_u8Angle = 180;
    }


    /*
     * 0 degree  -> 1 ms
     * 180 degree -> 2 ms
     *
     * Timer tick = 100 us
     *
     * 1 ms = 10 ticks
     * 2 ms = 20 ticks
     */

    Local_u16Pulse =
        10 +
        (((u16)Copy_u8Angle * 10) / 180);


    GRIPPER_u8PulseTicks =
        (u8)Local_u16Pulse;


    GRIPPER_u8CurrentAngle =
        Copy_u8Angle;
}


/* =====================================================
 *                    OPEN GRIPPER
 * ===================================================== */
void GRIPPER_voidOpen(void)
{
    GRIPPER_voidSetAngle(
        GRIPPER_OPEN_ANGLE
    );
}


/* =====================================================
 *                   CLOSE GRIPPER
 * ===================================================== */

void GRIPPER_voidClose(void)
{
    GRIPPER_voidSetAngle(
        GRIPPER_CLOSE_ANGLE
    );
}


/* =====================================================
 *                 BUTTON CONTROL
 * ===================================================== */

void GRIPPER_voidControl(void)
{
    u8 Local_u8OpenButton;
    u8 Local_u8CloseButton;


    /* Read buttons */

    DIO_enumGetPinValue(
        GRIPPER_OPEN_BUTTON_PORT,
        GRIPPER_OPEN_BUTTON_PIN,
        &Local_u8OpenButton
    );


    DIO_enumGetPinValue(
        GRIPPER_CLOSE_BUTTON_PORT,
        GRIPPER_CLOSE_BUTTON_PIN,
        &Local_u8CloseButton
    );


    /* ================================================
     * OPEN button
     * ================================================ */

    if(Local_u8OpenButton == GRIPPER_BUTTON_PRESSED)
    {
        GRIPPER_voidOpen();

        _delay_ms(200);
    }


    /* ================================================
     * CLOSE button
     * ================================================ */

    if(Local_u8CloseButton == GRIPPER_BUTTON_PRESSED)
    {
        GRIPPER_voidClose();

        _delay_ms(200);
    }
}


/* =====================================================
 *                  TIMER0 ISR
 * ===================================================== */

ISR(TIMER0_COMP_vect)
{
    /*
     * Start 20 ms period
     */

    if(GRIPPER_u8Counter == 0)
    {
        SET_BIT(PORTB, 3);
    }


    /*
     * End servo pulse
     */

    if(GRIPPER_u8Counter >= GRIPPER_u8PulseTicks)
    {
        CLR_BIT(PORTB, 3);
    }


    /*
     * Next 100 us
     */

    GRIPPER_u8Counter++;


    /*
     * 200 x 100 us = 20 ms
     */

    if(GRIPPER_u8Counter >= 200)
    {
        GRIPPER_u8Counter = 0;
    }
}
