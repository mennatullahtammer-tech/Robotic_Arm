#define F_CPU 8000000UL

#include <util/delay.h>

#include "LCD_Interface.h"
#include "LCD_Config.h"


/* =========================================================
 * Send one 4-bit nibble to LCD
 * D4 -> PC4
 * D5 -> PC5
 * D6 -> PC6
 * D7 -> PC7
 * ========================================================= */

static void LCD_Send_Nibble(u8 nibble)
{
    /* D4 */
    if(nibble & 0x10)
    {
        DIO_enumSetPinValue(LCD_DATA_PORT,
                            LCD_D4,
                            DIO_PIN_HIGH);
    }
    else
    {
        DIO_enumSetPinValue(LCD_DATA_PORT,
                            LCD_D4,
                            DIO_PIN_LOW);
    }


    /* D5 */
    if(nibble & 0x20)
    {
        DIO_enumSetPinValue(LCD_DATA_PORT,
                            LCD_D5,
                            DIO_PIN_HIGH);
    }
    else
    {
        DIO_enumSetPinValue(LCD_DATA_PORT,
                            LCD_D5,
                            DIO_PIN_LOW);
    }


    /* D6 */
    if(nibble & 0x40)
    {
        DIO_enumSetPinValue(LCD_DATA_PORT,
                            LCD_D6,
                            DIO_PIN_HIGH);
    }
    else
    {
        DIO_enumSetPinValue(LCD_DATA_PORT,
                            LCD_D6,
                            DIO_PIN_LOW);
    }


    /* D7 */
    if(nibble & 0x80)
    {
        DIO_enumSetPinValue(LCD_DATA_PORT,
                            LCD_D7,
                            DIO_PIN_HIGH);
    }
    else
    {
        DIO_enumSetPinValue(LCD_DATA_PORT,
                            LCD_D7,
                            DIO_PIN_LOW);
    }


    /* Enable pulse */
    LCD_Send_Falling_Edge();
}


/* =========================================================
 * LCD Initialization
 * ========================================================= */

void LCD_Init(void)
{
    /* Wait for LCD power stabilization */
    _delay_ms(50);


    /* =====================================================
     * Data pins PC4 -> PC7 OUTPUT
     * ===================================================== */

    DIO_enumSetPinDirection(LCD_DATA_PORT,
                            LCD_D4,
                            DIO_PIN_OUTPUT);

    DIO_enumSetPinDirection(LCD_DATA_PORT,
                            LCD_D5,
                            DIO_PIN_OUTPUT);

    DIO_enumSetPinDirection(LCD_DATA_PORT,
                            LCD_D6,
                            DIO_PIN_OUTPUT);

    DIO_enumSetPinDirection(LCD_DATA_PORT,
                            LCD_D7,
                            DIO_PIN_OUTPUT);


    /* =====================================================
     * Control pins
     * ===================================================== */

    DIO_enumSetPinDirection(LCD_CONTROL_PORT,
                            LCD_RS,
                            DIO_PIN_OUTPUT);

    DIO_enumSetPinDirection(LCD_CONTROL_PORT,
                            LCD_RW,
                            DIO_PIN_OUTPUT);

    DIO_enumSetPinDirection(LCD_CONTROL_PORT,
                            LCD_EN,
                            DIO_PIN_OUTPUT);


    /* Initial control states */

    DIO_enumSetPinValue(LCD_CONTROL_PORT,
                        LCD_RS,
                        DIO_PIN_LOW);

    DIO_enumSetPinValue(LCD_CONTROL_PORT,
                        LCD_RW,
                        DIO_PIN_LOW);

    DIO_enumSetPinValue(LCD_CONTROL_PORT,
                        LCD_EN,
                        DIO_PIN_LOW);


    /* =====================================================
     * LCD 4-bit initialization
     * ===================================================== */

    /* Function reset */
    LCD_Send_Nibble(0x30);

    _delay_ms(5);


    /* Function reset */
    LCD_Send_Nibble(0x30);

    _delay_us(150);


    /* Function reset */
    LCD_Send_Nibble(0x30);

    _delay_us(150);


    /* Set 4-bit mode */
    LCD_Send_Nibble(0x20);

    _delay_us(150);


    /* =====================================================
     * LCD Commands
     * ===================================================== */

    /* 4-bit mode
     * 2 lines
     * 5x8 font
     */
    LCD_Send_Command(0x28);


    /* Display ON
     * Cursor OFF
     * Blink OFF
     */
    LCD_Send_Command(0x0C);


    /* Clear Display */
    LCD_Send_Command(0x01);

    _delay_ms(2);


    /* Entry Mode
     * Cursor moves right
     */
    LCD_Send_Command(0x06);
}


/* =========================================================
 * Send Data / Character
 * ========================================================= */

void LCD_Send_Data(u8 data)
{
    /* RS = 1 -> Data */

    DIO_enumSetPinValue(LCD_CONTROL_PORT,
                        LCD_RS,
                        DIO_PIN_HIGH);


    /* RW = 0 -> Write */

    DIO_enumSetPinValue(LCD_CONTROL_PORT,
                        LCD_RW,
                        DIO_PIN_LOW);


    /* Send High Nibble */

    LCD_Send_Nibble(data & 0xF0);


    /* Send Low Nibble */

    LCD_Send_Nibble((data << 4) & 0xF0);
}


/* =========================================================
 * Send Command
 * ========================================================= */

void LCD_Send_Command(u8 command)
{
    /* RS = 0 -> Command */

    DIO_enumSetPinValue(LCD_CONTROL_PORT,
                        LCD_RS,
                        DIO_PIN_LOW);


    /* RW = 0 -> Write */

    DIO_enumSetPinValue(LCD_CONTROL_PORT,
                        LCD_RW,
                        DIO_PIN_LOW);


    /* Send High Nibble */

    LCD_Send_Nibble(command & 0xF0);


    /* Send Low Nibble */

    LCD_Send_Nibble((command << 4) & 0xF0);


    /* Command execution delay */

    _delay_us(50);
}


/* =========================================================
 * Send String
 * ========================================================= */

void LCD_Send_String(u8 *string)
{
    u8 i = 0;

    while(string[i] != '\0')
    {
        LCD_Send_Data(string[i]);

        i++;
    }
}


/* =========================================================
 * Send Number
 * ========================================================= */

void LCD_Send_Number(long Number)
{
    if(Number == 0)
    {
        LCD_Send_Data('0');

        return;
    }


    if(Number < 0)
    {
        LCD_Send_Data('-');

        Number = -Number;
    }


    if(Number >= 1000000000L)
        LCD_Send_Data((Number / 1000000000L) + '0');


    if(Number >= 100000000L)
        LCD_Send_Data(((Number / 100000000L) % 10) + '0');


    if(Number >= 10000000L)
        LCD_Send_Data(((Number / 10000000L) % 10) + '0');


    if(Number >= 1000000L)
        LCD_Send_Data(((Number / 1000000L) % 10) + '0');


    if(Number >= 100000L)
        LCD_Send_Data(((Number / 100000L) % 10) + '0');


    if(Number >= 10000L)
        LCD_Send_Data(((Number / 10000L) % 10) + '0');


    if(Number >= 1000L)
        LCD_Send_Data(((Number / 1000L) % 10) + '0');


    if(Number >= 100L)
        LCD_Send_Data(((Number / 100L) % 10) + '0');


    if(Number >= 10L)
        LCD_Send_Data(((Number / 10L) % 10) + '0');


    LCD_Send_Data((Number % 10) + '0');
}


/* =========================================================
 * Clear LCD
 * ========================================================= */

void LCD_Clear_Screen(void)
{
    LCD_Send_Command(0x01);

    _delay_ms(2);
}


/* =========================================================
 * Enable Falling Edge
 * ========================================================= */

void LCD_Send_Falling_Edge(void)
{
    /* EN = HIGH */

    DIO_enumSetPinValue(LCD_CONTROL_PORT,
                        LCD_EN,
                        DIO_PIN_HIGH);

    _delay_us(10);


    /* EN = LOW */

    DIO_enumSetPinValue(LCD_CONTROL_PORT,
                        LCD_EN,
                        DIO_PIN_LOW);

    _delay_us(50);
}


/* =========================================================
 * Go To XY
 * ========================================================= */

void LCD_GotoXY(u8 row, u8 col)
{
    if(row == CLCD_ROW_1)
    {
        LCD_Send_Command(0x80 + (col - 1));
    }
    else if(row == CLCD_ROW_2)
    {
        LCD_Send_Command(0xC0 + (col - 1));
    }
}

/* =========================================================
 * Display Special Character
 * ========================================================= */

void DisplaySpecialCharecter(u8 *pattern)
{
    u8 i;


    /* CGRAM address */

    LCD_Send_Command(0x40);


    /* Write 8 bytes */

    for(i = 0; i < 8; i++)
    {
        LCD_Send_Data(pattern[i]);
    }


    /* Back to DDRAM */

    LCD_Send_Command(0x80);


    /* Display character 0 */

    LCD_Send_Data(0);
}
