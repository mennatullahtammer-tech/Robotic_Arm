#ifndef HAL_GRIPPER_GRIPPER_CONFIG_H_
#define HAL_GRIPPER_GRIPPER_CONFIG_H_


/* =====================================================
 *                  GRIPPER SERVO
 * ===================================================== */

/*
 * Gripper Servo
 *
 * PB3 -> Servo Signal
 *
 * Timer0 will generate the servo signal.
 */

#define GRIPPER_SERVO_PORT       DIO_PORTB
#define GRIPPER_SERVO_PIN        DIO_PIN3


/* =====================================================
 *                  GRIPPER BUTTONS
 * ===================================================== */

/*
 * PB0 -> OPEN button
 * PB1 -> CLOSE button
 */

#define GRIPPER_OPEN_BUTTON_PORT     DIO_PORTB
#define GRIPPER_OPEN_BUTTON_PIN      DIO_PIN0

#define GRIPPER_CLOSE_BUTTON_PORT    DIO_PORTB
#define GRIPPER_CLOSE_BUTTON_PIN     DIO_PIN1


/* =====================================================
 *                  GRIPPER ANGLES
 * ===================================================== */

#define GRIPPER_OPEN_ANGLE       0
#define GRIPPER_CLOSE_ANGLE      90


/* =====================================================
 *              BUTTON ACTIVE STATE
 * ===================================================== */

#define GRIPPER_BUTTON_PRESSED   DIO_PIN_LOW


#endif
