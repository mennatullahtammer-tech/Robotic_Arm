#ifndef KPD_CONFIG_H_
#define KPD_CONFIG_H_
#include "../../LIB/STD_Types.h"
/* ================= KPD PORT ================= */

#define KPD_PORT        DIO_PORTA


/* ================= ROWS ================= */

#define KPD_R0          DIO_PIN0
#define KPD_R1          DIO_PIN1
#define KPD_R2          DIO_PIN2
#define KPD_R3          DIO_PIN3


/* ================= COLUMNS ================= */

#define KPD_C0          DIO_PIN4
#define KPD_C1          DIO_PIN5
#define KPD_C2          DIO_PIN6
#define KPD_C3          DIO_PIN7


/* ================= Scan Limits ================= */

#define KPD_ROW_INIT    KPD_R0
#define KPD_ROW_END     KPD_R3

#define KPD_COL_INIT    KPD_C0
#define KPD_COL_END     KPD_C3


/* ================= Not Pressed ================= */

#define NOTPRESSED      0xFF


/* ================= Keypad Buttons ================= */

static const u8 KPD_u8Buttons[4][4] =
{
    { '7', '8', '9', '/' },
    { '4', '5', '6', '*' },
    { '1', '2', '3', '-' },
    { 'C', '0', '=', '+' }
};

#endif
