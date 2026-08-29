#ifndef HAL_STEPPER_STEPPER_INTERFACE_H_
#define HAL_STEPPER_STEPPER_INTERFACE_H_

#include "../../LIB/STD_TYPES.h"


/* =====================================================
 *                    STEPPER INIT
 * ===================================================== */

void STEPPER_voidInit(void);


/* =====================================================
 *                  SINGLE STEP
 * ===================================================== */

void STEPPER_voidStep(u8 Copy_u8Step);


/* =====================================================
 *                     STOP
 * ===================================================== */

void STEPPER_voidStop(void);


/* =====================================================
 *                 MOVE CLOCKWISE
 * ===================================================== */

void STEPPER_voidMoveCW(u16 Copy_u16Steps);


/* =====================================================
 *              MOVE COUNTER CLOCKWISE
 * ===================================================== */

void STEPPER_voidMoveCCW(u16 Copy_u16Steps);


/* =====================================================
 *              MOVE ANGLE CLOCKWISE
 * ===================================================== */

void STEPPER_voidMoveAngleCW(u16 Copy_u16Angle);


/* =====================================================
 *           MOVE ANGLE COUNTER CLOCKWISE
 * ===================================================== */

void STEPPER_voidMoveAngleCCW(u16 Copy_u16Angle);


/* =====================================================
 *                   MOVE TO ANGLE
 * ===================================================== */

void STEPPER_voidMoveToAngle(s16 Copy_s16TargetAngle);


/* =====================================================
 *                GET CURRENT ANGLE
 * ===================================================== */

s16 STEPPER_s16GetCurrentAngle(void);


#endif
