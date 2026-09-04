#include "EEPROM_Position.h"

#include "../../MCAL/EEPROM/EEPROM_Interface.h"

#include "../../HAL/SERVO/SERVO_Interface.h"
#include "../../HAL/STEPPER/STEPPER_Interface.h"
#include "../../HAL/GRIPPER/GRIPPER_Interface.h"


/* =====================================================
 *                  EEPROM SETTINGS
 * ===================================================== */

#define EEPROM_POSITION_SIZE      6
#define EEPROM_MAX_POSITIONS      5


/* =====================================================
 *                  GET ADDRESS
 * ===================================================== */

static u16 EEPROM_Position_GetAddress(u8 PositionNumber)
{
    if(PositionNumber < 1 ||
       PositionNumber > EEPROM_MAX_POSITIONS)
    {
        return 0;
    }

    return ((u16)(PositionNumber - 1) *
            EEPROM_POSITION_SIZE);
}


/* =====================================================
 *                  SAVE POSITION
 * ===================================================== */

void EEPROM_Position_Save(
    u8 PositionNumber,
    u8 Joint1Angle,
    u8 Joint2Angle,
    u8 Joint3Angle,
    u16 BaseAngle,
    u8 GripperState
)
{
    u16 Address;


    /* Check position number */

    if(PositionNumber < 1 ||
       PositionNumber > EEPROM_MAX_POSITIONS)
    {
        return;
    }


    /* Get EEPROM address */

    Address =
        EEPROM_Position_GetAddress(PositionNumber);


    /* =================================================
     * J1
     * ================================================= */

    EEPROM_voidWriteByte(
        Address + 0,
        Joint1Angle
    );


    /* =================================================
     * J2
     * ================================================= */

    EEPROM_voidWriteByte(
        Address + 1,
        Joint2Angle
    );


    /* =================================================
     * J3
     * ================================================= */

    EEPROM_voidWriteByte(
        Address + 2,
        Joint3Angle
    );


    /* =================================================
     * BASE LOW BYTE
     * ================================================= */

    EEPROM_voidWriteByte(
        Address + 3,
        (u8)(BaseAngle & 0xFF)
    );


    /* =================================================
     * BASE HIGH BYTE
     * ================================================= */

    EEPROM_voidWriteByte(
        Address + 4,
        (u8)((BaseAngle >> 8) & 0xFF)
    );


    /* =================================================
     * GRIPPER
     *
     * 0 = OPEN
     * 1 = CLOSED
     * ================================================= */

    EEPROM_voidWriteByte(
        Address + 5,
        GripperState
    );
}


/* =====================================================
 *                  LOAD POSITION
 * ===================================================== */

u8 EEPROM_Position_Load(
    u8 PositionNumber,
    u8 *Joint1Angle,
    u8 *Joint2Angle,
    u8 *Joint3Angle,
    u16 *BaseAngle,
    u8 *GripperState
)

    {
    u16 Address;

    u8 BaseLow;
    u8 BaseHigh;


    /* =================================================
     * Check position number
     * ================================================= */

    if(PositionNumber < 1 ||
       PositionNumber > EEPROM_MAX_POSITIONS)
    {
        return 0;
    }


    /* =================================================
     * Check pointers
     * ================================================= */

    if(Joint1Angle == 0 ||
       Joint2Angle == 0 ||
       Joint3Angle == 0 ||
       BaseAngle == 0 ||
       GripperState == 0)
    {
        return 0;
    }


    /* =================================================
     * Get EEPROM address
     * ================================================= */

    Address =
        EEPROM_Position_GetAddress(PositionNumber);


    /* =================================================
     * READ J1
     * ================================================= */

    *Joint1Angle =
        EEPROM_u8ReadByte(
            Address + 0
        );
/* =================================================
     * READ J2
     * ================================================= */

    *Joint2Angle =
        EEPROM_u8ReadByte(
            Address + 1
        );


    /* =================================================
     * READ J3
     * ================================================= */

    *Joint3Angle =
        EEPROM_u8ReadByte(
            Address + 2
        );


    /* =================================================
     * READ BASE
     * ================================================= */

    BaseLow =
        EEPROM_u8ReadByte(
            Address + 3
        );

    BaseHigh =
        EEPROM_u8ReadByte(
            Address + 4
        );


    *BaseAngle =
        ((u16)BaseHigh << 8) | BaseLow;


    /* =================================================
     * READ GRIPPER
     * ================================================= */

    *GripperState =
        EEPROM_u8ReadByte(
            Address + 5
        );


    /* =================================================
     *              MOVE SERVO 1
     * ================================================= */

    SERVO_voidSetAngle1(
        *Joint1Angle
    );


    /* =================================================
     *              MOVE SERVO 2
     * ================================================= */

    SERVO_voidSetAngle2(
        *Joint2Angle
    );


    /* =================================================
     *              MOVE SERVO 3
     * ================================================= */

    SERVO_voidSetAngle3(
        *Joint3Angle
    );


    /* =================================================
     *              MOVE BASE
     * ================================================= */

    STEPPER_voidMoveToAngle(
        (s16)(*BaseAngle)
    );


    /* =================================================
     *              MOVE GRIPPER
     * ================================================= */

    if(*GripperState == 0)
    {
        GRIPPER_voidOpen();
    }
    else
    {
        GRIPPER_voidClose();
    }


    return 1;
}
