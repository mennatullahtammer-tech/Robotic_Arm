#include "HAL/SERVO/SERVO_Interface.h"
#include "HAL/STEPPER/STEPPER_Interface.h"
#include "HAL/GRIPPER/GRIPPER_Interface.h"

#include <util/delay.h>
#include <avr/interrupt.h>


int main(void)
{
    /* =====================================================
     *                  INITIALIZATION
     * ===================================================== */

    /* Servo 1 + Servo 2 + Servo 3 */
    SERVO_voidInit();

    /* Stepper Motor */
    STEPPER_voidInit();

    /* Gripper Servo + Open/Close Buttons */
    GRIPPER_voidInit();

    /* Enable Global Interrupt */
    sei();


    /* =====================================================
     *                  INITIAL POSITIONS
     * ===================================================== */

    SERVO_voidSetAngle1(0);
    SERVO_voidSetAngle2(90);
    SERVO_voidSetAngle3(180);

    GRIPPER_voidOpen();

    _delay_ms(2000);


    /* =====================================================
     *                  SERVO TEST
     * ===================================================== */

    SERVO_voidSetAngle1(90);
    SERVO_voidSetAngle2(180);
    SERVO_voidSetAngle3(90);

    _delay_ms(2000);


    SERVO_voidSetAngle1(180);
    SERVO_voidSetAngle2(0);
    SERVO_voidSetAngle3(0);

    _delay_ms(2000);


    /* =====================================================
     *                  STEPPER TEST
     * ===================================================== */

    /* 90° Clockwise */
    STEPPER_voidMoveAngleCW(90);

    _delay_ms(2000);


    /* 90° Counter Clockwise */
    STEPPER_voidMoveAngleCCW(90);

    _delay_ms(2000);


    /* =====================================================
     *                  NORMAL OPERATION
     * ===================================================== */

    while(1)
    {
        /*
         * PB0:
         * OPEN GRIPPER
         *
         * PB1:
         * CLOSE GRIPPER
         */

        GRIPPER_voidControl();
    }
}
