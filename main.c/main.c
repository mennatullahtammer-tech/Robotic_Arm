#define F_CPU 8000000UL

#include <avr/io.h>
#include <avr/interrupt.h>
#include <util/delay.h>

#include "LIB/STD_Types.h"
#include "LIB/BIT_Math.h"

#include "MCAL/DIO/DIO_Interface.h"

#include "HAL/LCD/LCD_Interface.h"
#include "HAL/KPD/KPD_Interface.h"

#include "HAL/SERVO/SERVO_Interface.h"
#include "HAL/STEPPER/STEPPER_Interface.h"
#include "HAL/GRIPPER/GRIPPER_Interface.h"

#include "HAL/EEPROM_POSITION/EEPROM_Position.h"


/* =========================================================
 *                  CURRENT ARM STATE
 * ========================================================= */

static u8 Joint1_Angle = 0;
static u8 Joint2_Angle = 90;
static u8 Joint3_Angle = 180;


/* =========================================================
 *                  LCD HOME
 * ========================================================= */

static void LCD_ShowHome(void)
{
    LCD_Clear_Screen();

    LCD_GotoXY(
        CLCD_ROW_1,
        CLCD_COL_1
    );

    LCD_Send_String(
        (u8*)"1:J1 2:J2 3:J3"
    );

    LCD_GotoXY(
        CLCD_ROW_2,
        CLCD_COL_1
    );

    LCD_Send_String(
        (u8*)"4:B 5:G 6:S 7:SV"
    );
}


/* =========================================================
 *                  LCD JOINT
 * ========================================================= */

static void LCD_ShowJoint(
    u8 JointNumber,
    u8 Angle
)
{
    LCD_Clear_Screen();

    LCD_GotoXY(
        CLCD_ROW_1,
        CLCD_COL_1
    );

    LCD_Send_String(
        (u8*)"Joint "
    );

    LCD_Send_Number(
        JointNumber
    );


    LCD_GotoXY(
        CLCD_ROW_2,
        CLCD_COL_1
    );

    LCD_Send_String(
        (u8*)"Angle = "
    );

    LCD_Send_Number(
        Angle
    );
}


/* =========================================================
 *                  LCD BASE
 * ========================================================= */

static void LCD_ShowBase(void)
{
    LCD_Clear_Screen();

    LCD_GotoXY(
        CLCD_ROW_1,
        CLCD_COL_1
    );

    LCD_Send_String(
        (u8*)"BASE CONTROL"
    );


    LCD_GotoXY(
        CLCD_ROW_2,
        CLCD_COL_1
    );

    LCD_Send_String(
        (u8*)"2:CW 8:CCW =:Exit"
    );
}


/* =========================================================
 *                  LCD GRIPPER
 * ========================================================= */

static void LCD_ShowGripper(void)
{
    LCD_Clear_Screen();

    LCD_GotoXY(
        CLCD_ROW_1,
        CLCD_COL_1
    );

    LCD_Send_String(
        (u8*)"GRIPPER"
    );


    LCD_GotoXY(
        CLCD_ROW_2,
        CLCD_COL_1
    );

    LCD_Send_String(
        (u8*)"PB0 OPEN PB1 CLOSE"
    );
}


/* =========================================================
 *                  LCD STATUS
 * ========================================================= */

static void LCD_ShowArmStatus(void)
{
    u16 BaseAngle;


    BaseAngle =
        (u16)STEPPER_s16GetCurrentAngle();


    LCD_Clear_Screen();


    LCD_GotoXY(
        CLCD_ROW_1,
        CLCD_COL_1
    );

    LCD_Send_String(
        (u8*)"J1="
    );

    LCD_Send_Number(
        Joint1_Angle
    );


    LCD_Send_String(
        (u8*)" J2="
    );

    LCD_Send_Number(
        Joint2_Angle
    );


    LCD_GotoXY(
        CLCD_ROW_2,
        CLCD_COL_1
    );

    LCD_Send_String(
        (u8*)"J3="
    );

    LCD_Send_Number(
        Joint3_Angle
    );


    LCD_Send_String(
        (u8*)" B="
    );

    LCD_Send_Number(
        BaseAngle
    );


    _delay_ms(1500);
}


/* =========================================================
 *                  SAVE SCREEN
 * ========================================================= */

static void LCD_ShowSavePosition(u8 Position)
{
    LCD_Clear_Screen();


    LCD_GotoXY(
        CLCD_ROW_1,
        CLCD_COL_1
    );

    LCD_Send_String(
        (u8*)"SAVE POSITION"
    );


    LCD_GotoXY(
        CLCD_ROW_2,
        CLCD_COL_1
    );

    LCD_Send_String(
        (u8*)"POSITION: "
    );

    LCD_Send_Number(
        Position
    );
}


/* =========================================================
 *                  LOAD SCREEN
 * ========================================================= */

static void LCD_ShowLoadPosition(u8 Position)
{
    LCD_Clear_Screen();


    LCD_GotoXY(
        CLCD_ROW_1,
        CLCD_COL_1
    );

    LCD_Send_String(
        (u8*)"LOAD POSITION"
    );


    LCD_GotoXY(
        CLCD_ROW_2,
        CLCD_COL_1
    );

    LCD_Send_String(
        (u8*)"POSITION: "
    );

    LCD_Send_Number(
        Position
    );
}


/* =========================================================
 *              SHOW LOADED POSITION
 * ========================================================= */

static void LCD_ShowLoadedPosition(
    u8 Position,
    u8 J1,
    u8 J2,
    u8 J3,
    u16 Base
)
{
    LCD_Clear_Screen();


    LCD_GotoXY(
        CLCD_ROW_1,
        CLCD_COL_1
    );

    LCD_Send_String(
        (u8*)"POS "
    );

    LCD_Send_Number(
        Position
    );

    LCD_Send_String(
        (u8*)" LOADED"
    );


    LCD_GotoXY(
        CLCD_ROW_2,
        CLCD_COL_1
    );

    LCD_Send_String(
        (u8*)"J1:"
    );

    LCD_Send_Number(
        J1
    );

    LCD_Send_String(
        (u8*)" J2:"
    );

    LCD_Send_Number(
        J2
    );


    _delay_ms(1000);


    LCD_Clear_Screen();


    LCD_GotoXY(
        CLCD_ROW_1,
        CLCD_COL_1
    );

    LCD_Send_String(
        (u8*)"J3:"
    );

    LCD_Send_Number(
        J3
    );


    LCD_Send_String(
        (u8*)" B:"
    );

    LCD_Send_Number(
        Base
    );


    _delay_ms(1500);
}


/* =========================================================
 *                  JOINT 1 CONTROL
 * ========================================================= */

static void Control_Joint1(void)
{
    u8 Key;
    u8 Angle;


    Angle = Joint1_Angle;


    LCD_ShowJoint(
        1,
        Angle
    );


    while(1)
    {
        /* Gripper physical buttons remain active */

        GRIPPER_voidControl();


        Key =
            KPD_u8GetPressed();


        if(Key == '2')
        {
            if(Angle <= 170)
            {
                Angle += 10;

                SERVO_voidSetAngle1(
                    Angle
                );

                LCD_ShowJoint(
                    1,
                    Angle
                );
            }
        }


        else if(Key == '8')
        {
            if(Angle >= 10)
            {
                Angle -= 10;

                SERVO_voidSetAngle1(
                    Angle
                );

                LCD_ShowJoint(
                    1,
                    Angle
                );
            }
        }


        else if(Key == '=')
        {
            Joint1_Angle = Angle;

            break;
        }


        _delay_ms(150);
    }
}


/* =========================================================
 *                  JOINT 2 CONTROL
 * ========================================================= */

static void Control_Joint2(void)
{
    u8 Key;
    u8 Angle;


    Angle = Joint2_Angle;


    LCD_ShowJoint(
        2,
        Angle
    );


    while(1)
    {
        GRIPPER_voidControl();


        Key =
            KPD_u8GetPressed();


        if(Key == '2')
        {
            if(Angle <= 170)
            {
                Angle += 10;

                SERVO_voidSetAngle2(
                    Angle
                );

                LCD_ShowJoint(
                    2,
                    Angle
                );
            }
        }


        else if(Key == '8')
        {
            if(Angle >= 10)
            {
                Angle -= 10;

                SERVO_voidSetAngle2(
                    Angle
                );

                LCD_ShowJoint(
                    2,
                    Angle
                );
            }
        }


        else if(Key == '=')
        {
            Joint2_Angle = Angle;

            break;
        }


        _delay_ms(150);
    }
}


/* =========================================================
 *                  JOINT 3 CONTROL
 * ========================================================= */

static void Control_Joint3(void)
{
    u8 Key;
    u8 Angle;


    Angle = Joint3_Angle;


    LCD_ShowJoint(
        3,
        Angle
    );


    while(1)
    {
        GRIPPER_voidControl();


        Key =
            KPD_u8GetPressed();


        if(Key == '2')
        {
            if(Angle <= 170)
            {
                Angle += 10;

                SERVO_voidSetAngle3(
                    Angle
                );

                LCD_ShowJoint(
                    3,
                    Angle
                );
            }
        }


        else if(Key == '8')
        {
            if(Angle >= 10)
            {
                Angle -= 10;

                SERVO_voidSetAngle3(
                    Angle
                );

                LCD_ShowJoint(
                    3,
                    Angle
                );
            }
        }


        else if(Key == '=')
        {
            Joint3_Angle = Angle;

            break;
        }


        _delay_ms(150);
    }
}


/* =========================================================
 *                  BASE CONTROL
 * ========================================================= */

static void Control_Base(void)
{
    u8 Key;


    LCD_ShowBase();


    while(1)
    {
        GRIPPER_voidControl();


        Key =
            KPD_u8GetPressed();


        if(Key == '2')
        {
            STEPPER_voidMoveAngleCW(
                90
            );
        }


        else if(Key == '8')
        {
            STEPPER_voidMoveAngleCCW(
                90
            );
        }


        else if(Key == '=')
        {
            break;
        }


        _delay_ms(150);
    }
}


/* =========================================================
 *                  GRIPPER CONTROL
 *
 * Physical buttons only
 *
 * PB0 -> OPEN
 * PB1 -> CLOSE
 * ========================================================= */

static void Control_Gripper(void)
{
    u8 Key;


    LCD_ShowGripper();


    while(1)
    {
        GRIPPER_voidControl();


        Key =
            KPD_u8GetPressed();


        if(Key == '=')
        {
            break;
        }
    }
}


/* =========================================================
 *                  SAVE POSITION
 *
 * Choose:
 *
 * 1 -> Position 1
 * 2 -> Position 2
 * 3 -> Position 3
 * 4 -> Position 4
 * 5 -> Position 5
 *
 * = -> Cancel
 * ========================================================= */

static void Control_SavePosition(void)
{
    u8 Key;
    u8 Position;

    u16 BaseAngle;
    u8 GripperState;


    LCD_Clear_Screen();


    LCD_GotoXY(
        CLCD_ROW_1,
        CLCD_COL_1
    );

    LCD_Send_String(
        (u8*)"SAVE POS 1-5"
    );


    LCD_GotoXY(
        CLCD_ROW_2,
        CLCD_COL_1
    );

    LCD_Send_String(
        (u8*)"Choose Number:"
    );


    while(1)
    {
        GRIPPER_voidControl();


        Key =
            KPD_u8GetPressed();


        if(Key >= '1' && Key <= '5')
        {
            Position =
                Key - '0';


            BaseAngle =
                (u16)STEPPER_s16GetCurrentAngle();


            /*
             * IMPORTANT:
             *
             * Current Gripper angle is not directly
             * available in the interface.
             *
             * So here:
             *
             * 0 = OPEN
             * 1 = CLOSE
             *
             * Change this according to your current
             * gripper state if needed.
             */

            GripperState = 0;


            EEPROM_Position_Save(
                Position,
                Joint1_Angle,
                Joint2_Angle,
                Joint3_Angle,
                BaseAngle,
                GripperState
            );


            LCD_ShowSavePosition(
                Position
            );


            _delay_ms(700);


            LCD_Clear_Screen();


            LCD_GotoXY(
                CLCD_ROW_1,
                CLCD_COL_1
            );

            LCD_Send_String(
                (u8*)"POSITION SAVED"
            );


            LCD_GotoXY(
                CLCD_ROW_2,
                CLCD_COL_1
            );

            LCD_Send_String(
                (u8*)"POS: "
            );

            LCD_Send_Number(
                Position
            );


            _delay_ms(1000);


            break;
        }


        else if(Key == '=')
        {
            break;
        }
    }
}


/* =========================================================
 *                  LOAD POSITION
 *
 * Choose:
 *
 * 1 -> Position 1
 * 2 -> Position 2
 * 3 -> Position 3
 * 4 -> Position 4
 * 5 -> Position 5
 * ========================================================= */

static void Control_LoadPosition(void)
{
    u8 Key;
    u8 Position;

    u8 J1;
    u8 J2;
    u8 J3;

    u16 BaseAngle;

    u8 GripperState;


    LCD_Clear_Screen();


    LCD_GotoXY(
        CLCD_ROW_1,
        CLCD_COL_1
    );

    LCD_Send_String(
        (u8*)"LOAD POS 1-5"
    );


    LCD_GotoXY(
        CLCD_ROW_2,
        CLCD_COL_1
    );

    LCD_Send_String(
        (u8*)"Choose Number:"
    );


    while(1)
    {
        GRIPPER_voidControl();


        Key =
            KPD_u8GetPressed();


        if(Key >= '1' && Key <= '5')
        {
            Position =
                Key - '0';


            LCD_ShowLoadPosition(
                Position
            );


            _delay_ms(500);


            if(
                EEPROM_Position_Load(
                    Position,
                    &J1,
                    &J2,
                    &J3,
                    &BaseAngle,
                    &GripperState
                )
                == 1
            )
            {
                /*
                 * Update software state
                 */

                Joint1_Angle = J1;
                Joint2_Angle = J2;
                Joint3_Angle = J3;


                /*
                 * Show loaded values
                 */

                LCD_ShowLoadedPosition(
                    Position,
                    J1,
                    J2,
                    J3,
                    BaseAngle
                );
            }


            break;
        }


        else if(Key == '=')
        {
            break;
        }
    }
}


/* =========================================================
 *                       MAIN
 * ========================================================= */

int main(void)
{
    u8 Key;


    /* =====================================================
     * INITIALIZATION
     * ===================================================== */

    LCD_Init();

    KPD_Init();

    SERVO_voidInit();

    STEPPER_voidInit();

    GRIPPER_voidInit();


    /* Enable Interrupts */

    sei();


    /* =====================================================
     * INITIAL POSITIONS
     * ===================================================== */

    Joint1_Angle = 0;

    Joint2_Angle = 90;

    Joint3_Angle = 180;


    SERVO_voidSetAngle1(
        Joint1_Angle
    );


    SERVO_voidSetAngle2(
        Joint2_Angle
    );


    SERVO_voidSetAngle3(
        Joint3_Angle
    );


    /* Gripper initially OPEN */

    GRIPPER_voidOpen();


    /* =====================================================
     * WELCOME
     * ===================================================== */

    LCD_Clear_Screen();


    LCD_GotoXY(
        CLCD_ROW_1,
        CLCD_COL_1
    );

    LCD_Send_String(
        (u8*)"ROBOT ARM"
    );


    LCD_GotoXY(
        CLCD_ROW_2,
        CLCD_COL_1
    );

    LCD_Send_String(
        (u8*)"READY..."
    );


    _delay_ms(1500);


    LCD_ShowHome();


    /* =====================================================
     * MAIN LOOP
     * ===================================================== */

    while(1)
    {
        /*
         * Physical Gripper Buttons
         *
         * PB0 -> Open
         * PB1 -> Close
         */

        GRIPPER_voidControl();


        /*
         * Keypad
         */

        Key =
            KPD_u8GetPressed();


        /* J1 */

        if(Key == '1')
        {
            Control_Joint1();

            LCD_ShowHome();
        }


        /* J2 */

        else if(Key == '2')
        {
            Control_Joint2();

            LCD_ShowHome();
        }


        /* J3 */

        else if(Key == '3')
        {
            Control_Joint3();

            LCD_ShowHome();
        }


        /* Base */

        else if(Key == '4')
        {
            Control_Base();

            LCD_ShowHome();
        }


        /* Gripper menu */

        else if(Key == '5')
        {
            Control_Gripper();

            LCD_ShowHome();
        }


        /* Status */

        else if(Key == '6')
        {
            LCD_ShowArmStatus();

            LCD_ShowHome();
        }


        /* SAVE */

        else if(Key == '7')
        {
            Control_SavePosition();

            LCD_ShowHome();
        }


        /* LOAD */

        else if(Key == '8')
        {
            Control_LoadPosition();

            LCD_ShowHome();
        }


        _delay_ms(100);
    }


    return 0;
}
