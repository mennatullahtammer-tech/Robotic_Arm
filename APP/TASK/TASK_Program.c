#include "TASK_Interface.h"

#include "../../MCAL/EEPROM/EEPROM_Interface.h"

#include "../../HAL/SERVO/SERVO_Interface.h"
#include "../../HAL/STEPPER/STEPPER_Interface.h"
#include "../../HAL/GRIPPER/GRIPPER_Interface.h"

#include <util/delay.h>


/* =====================================================
 *                  EEPROM MEMORY MAP
 * ===================================================== */

#define TASK_COUNT_ADDRESS       0

#define TASK_START_ADDRESS       1

#define TASK_RECORD_SIZE         2

#define TASK_MAX_RECORDS         100


/* =====================================================
 *                  VARIABLES
 * ===================================================== */

static u8 TASK_u8Count = 0;

static u8 TASK_u8Recording = 0;


/* =====================================================
 *                  INIT
 * ===================================================== */

void TASK_voidInit(void)
{
    TASK_u8Count =
        EEPROM_u8ReadByte(TASK_COUNT_ADDRESS);


    /*
     * If EEPROM contains invalid value
     * start with empty memory
     */

    if(TASK_u8Count > TASK_MAX_RECORDS)
    {
        TASK_u8Count = 0;

        EEPROM_voidWriteByte(
            TASK_COUNT_ADDRESS,
            0
        );
    }

    TASK_u8Recording = 0;
}


/* =====================================================
 *                  START RECORD
 * ===================================================== */

void TASK_voidStartRecord(void)
{
    TASK_u8Count = 0;

    TASK_u8Recording = 1;

    EEPROM_voidWriteByte(
        TASK_COUNT_ADDRESS,
        0
    );
}


/* =====================================================
 *                  STOP RECORD
 * ===================================================== */

void TASK_voidStopRecord(void)
{
    TASK_u8Recording = 0;

    EEPROM_voidWriteByte(
        TASK_COUNT_ADDRESS,
        TASK_u8Count
    );
}


/* =====================================================
 *                  RECORD MOVEMENT
 * ===================================================== */

void TASK_voidRecord(
    u8 Copy_u8Type,
    u8 Copy_u8Value
)
{
    u16 Local_u16Address;


    if(TASK_u8Recording == 0)
    {
        return;
    }


    if(TASK_u8Count >= TASK_MAX_RECORDS)
    {
        return;
    }


    Local_u16Address =
        TASK_START_ADDRESS +
        ((u16)TASK_u8Count * TASK_RECORD_SIZE);


    /* Save movement type */

    EEPROM_voidWriteByte(
        Local_u16Address,
        Copy_u8Type
    );


    /* Save movement value */

    EEPROM_voidWriteByte(
        Local_u16Address + 1,
        Copy_u8Value
    );


    TASK_u8Count++;
}


/* =====================================================
 *                  PLAY TASK
 * ===================================================== */

void TASK_voidPlay(void)
{
    u8 i;

    u8 Local_u8Type;

    u8 Local_u8Value;

    u16 Local_u16Address;


    /* Stop recording during playback */

    TASK_u8Recording = 0;


    for(i = 0; i < TASK_u8Count; i++)
    {
        Local_u16Address =
            TASK_START_ADDRESS +
            ((u16)i * TASK_RECORD_SIZE);


        Local_u8Type =
            EEPROM_u8ReadByte(
                Local_u16Address
            );


        Local_u8Value =
            EEPROM_u8ReadByte(
                Local_u16Address + 1
            );


        /* =============================================
         * JOINT 1
         * ============================================= */

        if(Local_u8Type == TASK_JOINT1)
        {
            SERVO_voidSetAngle1(
                Local_u8Value
            );

            _delay_ms(500);
        }


        /* =============================================
         * JOINT 2
         * ============================================= */

        else if(Local_u8Type == TASK_JOINT2)
        {
            SERVO_voidSetAngle2(
                Local_u8Value
            );

            _delay_ms(500);
        }


        /* =============================================
         * JOINT 3
         * ============================================= */

        else if(Local_u8Type == TASK_JOINT3)
        {
            SERVO_voidSetAngle3(
                Local_u8Value
            );

            _delay_ms(500);
        }


        /* =============================================
         * BASE CLOCKWISE
         * ============================================= */

        else if(Local_u8Type == TASK_BASE_CW)
        {
            STEPPER_voidMoveAngleCW(
                Local_u8Value
            );

            _delay_ms(500);
        }


        /* =============================================
         * BASE COUNTER CLOCKWISE
         * ============================================= */

        else if(Local_u8Type == TASK_BASE_CCW)
        {
            STEPPER_voidMoveAngleCCW(
                Local_u8Value
            );

            _delay_ms(500);
        }


        /* =============================================
         * GRIPPER OPEN
         * ============================================= */

        else if(Local_u8Type == TASK_GRIPPER_OPEN)
        {
            GRIPPER_voidOpen();

            _delay_ms(500);
        }


        /* =============================================
         * GRIPPER CLOSE
         * ============================================= */

        else if(Local_u8Type == TASK_GRIPPER_CLOSE)
        {
            GRIPPER_voidClose();

            _delay_ms(500);
        }
    }
}


/* =====================================================
 *                  CLEAR TASK
 * ===================================================== */

void TASK_voidClear(void)
{
    TASK_u8Count = 0;

    TASK_u8Recording = 0;

    EEPROM_voidWriteByte(
        TASK_COUNT_ADDRESS,
        0
    );
}


/* =====================================================
 *                  GET COUNT
 * ===================================================== */

u8 TASK_u8GetCount(void)
{
    return TASK_u8Count;
}


/* =====================================================
 *                  IS RECORDING
 * ===================================================== */

u8 TASK_u8IsRecording(void)
{
    return TASK_u8Recording;
}
