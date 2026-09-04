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
 *                 CURRENT ARM STATE
 * ========================================================= */

static u8 Joint1_Angle = 0;
static u8 Joint2_Angle = 90;
static u8 Joint3_Angle = 180;

static u8 Gripper_State = 0;


/* =========================================================
 *                    LCD HOME
 * ========================================================= */

static void LCD_ShowHome(void)
{
    LCD_Clear_Screen();

    LCD_GotoXY(CLCD_ROW_1, CLCD_COL_1);
    LCD_Send_String((u8*)"1:J1 2:J2 3:J3");

    LCD_GotoXY(CLCD_ROW_2, CLCD_COL_1);
    LCD_Send_String((u8*)"4:B 5:G 6:S");
}


/* =========================================================
 *                  LCD JOINT SCREEN
 * ========================================================= */

static void LCD_ShowJoint(u8 JointNumber, u8 Angle)
{
    LCD_Clear_Screen();

    LCD_GotoXY(CLCD_ROW_1, CLCD_COL_1);

    LCD_Send_String((u8*)"JOINT ");

    LCD_Send_Number(JointNumber);

    LCD_GotoXY(CLCD_ROW_2, CLCD_COL_1);

    LCD_Send_String((u8*)"ANGLE:");

    LCD_Send_Number(Angle);

    LCD_Send_String((u8*)" +/-");
}


/* =========================================================
 *                   LCD BASE SCREEN
 * ========================================================= */

static void LCD_ShowBase(void)
{
    u16 BaseAngle;

    BaseAngle =
        (u16)STEPPER_s16GetCurrentAngle();

    LCD_Clear_Screen();

    LCD_GotoXY(CLCD_ROW_1, CLCD_COL_1);

    LCD_Send_String((u8*)"BASE:");

    LCD_Send_Number(BaseAngle);

    LCD_GotoXY(CLCD_ROW_2, CLCD_COL_1);

    LCD_Send_String((u8*)"+/- MOVE  = EXIT");
}


/* =========================================================
 *                 LCD GRIPPER SCREEN
 * ========================================================= */

static void LCD_ShowGripper(void)
{
    LCD_Clear_Screen();

    LCD_GotoXY(CLCD_ROW_1, CLCD_COL_1);

    LCD_Send_String((u8*)"GRIPPER");

    LCD_GotoXY(CLCD_ROW_2, CLCD_COL_1);

    if(Gripper_State == 0)
    {
        LCD_Send_String((u8*)"OPEN");
    }
    else
    {
        LCD_Send_String((u8*)"CLOSED");
    }
}


/* =========================================================
 *                  LCD ARM STATUS
 * ========================================================= */

static void LCD_ShowArmStatus(void)
{
    u16 BaseAngle;

    BaseAngle =
        (u16)STEPPER_s16GetCurrentAngle();

    LCD_Clear_Screen();

    LCD_GotoXY(CLCD_ROW_1, CLCD_COL_1);

    LCD_Send_String((u8*)"J1:");
    LCD_Send_Number(Joint1_Angle);

    LCD_Send_String((u8*)" J2:");
    LCD_Send_Number(Joint2_Angle);

    LCD_GotoXY(CLCD_ROW_2, CLCD_COL_1);

    LCD_Send_String((u8*)"J3:");
    LCD_Send_Number(Joint3_Angle);

    LCD_Send_String((u8*)" B:");
    LCD_Send_Number(BaseAngle);

    _delay_ms(1500);
}


/* =========================================================
 *                  JOINT 1 CONTROL
 *
 *                  +  -> INCREASE
 *                  -  -> DECREASE
 *                  =  -> SAVE / EXIT
 * ========================================================= */

static void Control_Joint1(void)
{
    u8 Key;
    u8 Angle;

    Angle = Joint1_Angle;

    LCD_ShowJoint(1, Angle);

    while(1)
    {
        GRIPPER_voidControl();

        Key = KPD_u8GetPressed();

        if(Key == '+')
        {
            if(Angle <= 170)
            {
                Angle += 10;

                SERVO_voidSetAngle1(Angle);

                Joint1_Angle = Angle;

                LCD_ShowJoint(1, Angle);
            }

            _delay_ms(200);
        }

        else if(Key == '-')
        {
            if(Angle >= 10)
            {
                Angle -= 10;

                SERVO_voidSetAngle1(Angle);

                Joint1_Angle = Angle;

                LCD_ShowJoint(1, Angle);
            }

            _delay_ms(200);
        }

        else if(Key == '=')
        {
            Joint1_Angle = Angle;

            break;
        }
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

    LCD_ShowJoint(2, Angle);

    while(1)
    {
        GRIPPER_voidControl();

        Key = KPD_u8GetPressed();

        if(Key == '+')
        {
            if(Angle <= 170)
            {
                Angle += 10;

                SERVO_voidSetAngle2(Angle);

                Joint2_Angle = Angle;

                LCD_ShowJoint(2, Angle);
            }

            _delay_ms(200);
        }

        else if(Key == '-')
        {
            if(Angle >= 10)
            {
                Angle -= 10;

                SERVO_voidSetAngle2(Angle);

                Joint2_Angle = Angle;

                LCD_ShowJoint(2, Angle);
            }

            _delay_ms(200);
        }

        else if(Key == '=')
        {
            Joint2_Angle = Angle;

            break;
        }
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

    LCD_ShowJoint(3, Angle);

    while(1)
    {
        GRIPPER_voidControl();

        Key = KPD_u8GetPressed();

        if(Key == '+')
        {
            if(Angle <= 170)
            {
                Angle += 10;

                SERVO_voidSetAngle3(Angle);

                Joint3_Angle = Angle;

                LCD_ShowJoint(3, Angle);
            }

            _delay_ms(200);
        }

        else if(Key == '-')
        {
            if(Angle >= 10)
            {
                Angle -= 10;

                SERVO_voidSetAngle3(Angle);

                Joint3_Angle = Angle;

                LCD_ShowJoint(3, Angle);
            }

            _delay_ms(200);
        }

        else if(Key == '=')
        {
            Joint3_Angle = Angle;

            break;
        }
    }
}


/* =========================================================
 *                     BASE CONTROL
 *
 *                  +  -> CW 90
 *                  -  -> CCW 90
 *                  =  -> EXIT
 * ========================================================= */

static void Control_Base(void)
{
    u8 Key;
    u16 BaseAngle;

    LCD_ShowBase();

    while(1)
    {
        GRIPPER_voidControl();

        Key = KPD_u8GetPressed();

        if(Key == '+')
        {
            STEPPER_voidMoveAngleCW(90);

            BaseAngle =
                (u16)STEPPER_s16GetCurrentAngle();

            LCD_ShowBase();

            _delay_ms(200);
        }

        else if(Key == '-')
        {
            STEPPER_voidMoveAngleCCW(90);

            BaseAngle =
                (u16)STEPPER_s16GetCurrentAngle();

            LCD_ShowBase();

            _delay_ms(200);
        }

        else if(Key == '=')
        {
            break;
        }
    }
}


/* =========================================================
 *                    GRIPPER CONTROL
 *
 * Hardware buttons:
 *
 * PB0 -> OPEN
 * PB1 -> CLOSE
 *
 * Keypad:
 *
 * = -> EXIT
 * ========================================================= */

static void Control_Gripper(void)
{
    u8 Key;

    LCD_ShowGripper();

    while(1)
    {
        GRIPPER_voidControl();

        Key = KPD_u8GetPressed();

        if(Key == '=')
        {
            break;
        }

        LCD_ShowGripper();

        _delay_ms(100);
    }
}


/* =========================================================
 *                  SAVE POSITION
 *
 * Position number:
 *
 * 1 -> Position 1
 * 2 -> Position 2
 * 3 -> Position 3
 * 4 -> Position 4
 * 5 -> Position 5
 * ========================================================= */

static void Save_Position(void)
{
    u8 Key;
    u8 PositionNumber;
    u16 BaseAngle;

    LCD_Clear_Screen();

    LCD_GotoXY(CLCD_ROW_1, CLCD_COL_1);
    LCD_Send_String((u8*)"SAVE POSITION");

    LCD_GotoXY(CLCD_ROW_2, CLCD_COL_1);
    LCD_Send_String((u8*)"1-5:");

    while(1)
    {
        Key = KPD_u8GetPressed();

        if(Key >= '1' && Key <= '5')
        {
            PositionNumber =
                Key - '0';

            BaseAngle =
                (u16)STEPPER_s16GetCurrentAngle();

            EEPROM_Position_Save(
                PositionNumber,
                Joint1_Angle,
                Joint2_Angle,
                Joint3_Angle,
                BaseAngle,
                Gripper_State
            );

            LCD_Clear_Screen();

            LCD_GotoXY(CLCD_ROW_1, CLCD_COL_1);

            LCD_Send_String((u8*)"POSITION ");

            LCD_Send_Number(PositionNumber);

            LCD_GotoXY(CLCD_ROW_2, CLCD_COL_1);

            LCD_Send_String((u8*)"SAVED!");

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
 * Select:
 *
 * 1 -> Position 1
 * 2 -> Position 2
 * 3 -> Position 3
 * 4 -> Position 4
 * 5 -> Position 5
 * ========================================================= */

static void Load_Position(void)
{
    u8 Key;
    u8 PositionNumber;

    u8 J1;
    u8 J2;
    u8 J3;

    u16 Base;

    u8 Grip;


    LCD_Clear_Screen();

    LCD_GotoXY(CLCD_ROW_1, CLCD_COL_1);
    LCD_Send_String((u8*)"LOAD POSITION");

    LCD_GotoXY(CLCD_ROW_2, CLCD_COL_1);
    LCD_Send_String((u8*)"1-5:");

    while(1)
    {
        Key = KPD_u8GetPressed();

        if(Key >= '1' && Key <= '5')
        {
            PositionNumber =
                Key - '0';

            /* =========================================
             * READ POSITION FROM EEPROM
             * ========================================= */

            if(
                EEPROM_Position_Load(
                    PositionNumber,
                    &J1,
                    &J2,
                    &J3,
                    &Base,
                    &Grip
                )
            )
            {
                /* =====================================
                 * Update current variables
                 * ===================================== */

                Joint1_Angle = J1;
                Joint2_Angle = J2;
                Joint3_Angle = J3;

                Gripper_State = Grip;


                /* =====================================
                 * Move Servos
                 * ===================================== */

                SERVO_voidSetAngle1(J1);

                SERVO_voidSetAngle2(J2);

                SERVO_voidSetAngle3(J3);


                /* =====================================
                 * Move Base
                 * ===================================== */

                STEPPER_voidMoveToAngle(
                    (s16)Base
                );


                /* =====================================
                 * Move Gripper
                 * ===================================== */

                if(Grip == 0)
                {
                    GRIPPER_voidOpen();
                }
                else
                {
                    GRIPPER_voidClose();
                }


                /* =====================================
                 * Show Loaded Position
                 * ===================================== */

                LCD_Clear_Screen();

                LCD_GotoXY(
                    CLCD_ROW_1,
                    CLCD_COL_1
                );

                LCD_Send_String((u8*)"LOADED P");

                LCD_Send_Number(
                    PositionNumber
                );

                LCD_GotoXY(
                    CLCD_ROW_2,
                    CLCD_COL_1
                );

                LCD_Send_String((u8*)"J1:");

                LCD_Send_Number(J1);

                LCD_Send_String((u8*)" J2:");

                LCD_Send_Number(J2);

                _delay_ms(1200);


                LCD_Clear_Screen();

                LCD_GotoXY(
                    CLCD_ROW_1,
                    CLCD_COL_1
                );

                LCD_Send_String((u8*)"J3:");

                LCD_Send_Number(J3);

                LCD_Send_String((u8*)" B:");

                LCD_Send_Number(Base);

                LCD_GotoXY(
                    CLCD_ROW_2,
                    CLCD_COL_1
                );

                if(Grip == 0)
                {
                    LCD_Send_String((u8*)"GRIP:OPEN");
                }
                else
                {
                    LCD_Send_String((u8*)"GRIP:CLOSE");
                }

                _delay_ms(1500);
            }

            else
            {
                LCD_Clear_Screen();

                LCD_GotoXY(
                    CLCD_ROW_1,
                    CLCD_COL_1
                );

                LCD_Send_String((u8*)"LOAD ERROR");

                LCD_GotoXY(
                    CLCD_ROW_2,
                    CLCD_COL_1
                );

                LCD_Send_String((u8*)"POSITION ");

                LCD_Send_Number(
                    PositionNumber
                );

                _delay_ms(1000);
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
 *                         MAIN
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


    /* Enable Global Interrupt */

    sei();


    /* =====================================================
     * INITIAL POSITIONS
     * ===================================================== */

    Joint1_Angle = 0;

    Joint2_Angle = 90;

    Joint3_Angle = 180;

    Gripper_State = 0;


    SERVO_voidSetAngle1(
        Joint1_Angle
    );

    SERVO_voidSetAngle2(
        Joint2_Angle
    );

    SERVO_voidSetAngle3(
        Joint3_Angle
    );


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
        (u8*)"SYSTEM READY"
    );

    _delay_ms(1500);


    /* =====================================================
     * MAIN MENU
     *
     * 1 -> J1
     * 2 -> J2
     * 3 -> J3
     * 4 -> BASE
     * 5 -> GRIPPER
     * 6 -> STATUS
     * 7 -> SAVE
     * 8 -> LOAD
     * ===================================================== */

    while(1)
    {
        GRIPPER_voidControl();

        LCD_ShowHome();

        Key =
            KPD_u8GetPressed();


        /* =================================================
         * JOINT 1
         * ================================================= */

        if(Key == '1')
        {
            Control_Joint1();

            LCD_ShowHome();
        }


        /* =================================================
         * JOINT 2
         * ================================================= */

        else if(Key == '2')
        {
            Control_Joint2();

            LCD_ShowHome();
        }


        /* =================================================
         * JOINT 3
         * ================================================= */

        else if(Key == '3')
        {
            Control_Joint3();

            LCD_ShowHome();
        }


        /* =================================================
         * BASE
         * ================================================= */

        else if(Key == '4')
        {
            Control_Base();

            LCD_ShowHome();
        }


        /* =================================================
         * GRIPPER
         * ================================================= */

        else if(Key == '5')
        {
            Control_Gripper();

            LCD_ShowHome();
        }


        /* =================================================
         * STATUS
         * ================================================= */

        else if(Key == '6')
        {
            LCD_ShowArmStatus();

            LCD_ShowHome();
        }


        /* =================================================
         * SAVE POSITION
         * ================================================= */

        else if(Key == '7')
        {
            Save_Position();

            LCD_ShowHome();
        }


        /* =================================================
         * LOAD POSITION
         * ================================================= */

        else if(Key == '8')
        {
            Load_Position();

            LCD_ShowHome();
        }


        _delay_ms(150);
    }


    return 0;
}
