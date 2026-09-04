#include "../../MCAL/DIO/DIO_Interface.h"

/* LCD Commands */

#define EIGHT_BITS                  0x38

#define lcd_DisplayOn_CursorOff    0x0C
#define lcd_DisplayOn_CursorOn     0x0E
#define lcd_DisplayOff_CursorOff   0x08
#define lcd_Clear                   0x01
#define lcd_EntryMode               0x06
#define lcd_Home                    0x02
#define lcd_CGRAM                   0x40
#define lcd_SetCursor               0x80
#define lcd_FunctionReset           0x30

/* LCD Rows */

#define CLCD_ROW_1   1
#define CLCD_ROW_2   2

/* LCD Columns */

#define CLCD_COL_1   1
#define CLCD_COL_2   2
#define CLCD_COL_3   3
#define CLCD_COL_4   4
#define CLCD_COL_5   5
#define CLCD_COL_6   6
#define CLCD_COL_7   7
#define CLCD_COL_8   8
#define CLCD_COL_9   9
#define CLCD_COL_10  10
#define CLCD_COL_11  11
#define CLCD_COL_12  12
#define CLCD_COL_13  13
#define CLCD_COL_14  14
#define CLCD_COL_15  15
#define CLCD_COL_16  16


/* LCD Functions */

void LCD_Init(void);

void LCD_Send_Data(u8 data);

void LCD_Send_Command(u8 command);

void LCD_Send_String(u8 *string);

void LCD_Send_Number(long Number);

void LCD_GotoXY(u8 row, u8 col);

void DisplaySpecialCharecter(u8 *pattern);

void LCD_Clear_Screen(void);

void LCD_Send_Falling_Edge(void);

