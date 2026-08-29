#ifndef HAL_STEPPER_STEPPER_CONFIG_H_
#define HAL_STEPPER_STEPPER_CONFIG_H_


/* =====================================================
 *                 STEPPER CONNECTION
 * ===================================================== */

/*
 * Proteus Stepper Motor
 *
 * PA0 -> Coil 1
 * PA1 -> Coil 2
 * PA2 -> Coil 3
 * PA3 -> Coil 4
 */

#define STEPPER_PORT        DIO_PORTA

#define STEPPER_PIN1        DIO_PIN0
#define STEPPER_PIN2        DIO_PIN1
#define STEPPER_PIN3        DIO_PIN2
#define STEPPER_PIN4        DIO_PIN3


/* =====================================================
 *                    STEP ANGLE
 * ===================================================== */

/*
 * Proteus motor Step Angle = 90 degree
 *
 * Every sequence = 90 degree
 */

#define STEPPER_STEP_ANGLE      90


/* =====================================================
 *                   STEP DELAY
 * ===================================================== */

/*
 * Delay between two steps
 */

#define STEPPER_STEP_DELAY      50


/* =====================================================
 *                  STEP SEQUENCE
 * ===================================================== */

#define STEPPER_STEP_0          0
#define STEPPER_STEP_1          1
#define STEPPER_STEP_2          2
#define STEPPER_STEP_3          3


#endif
