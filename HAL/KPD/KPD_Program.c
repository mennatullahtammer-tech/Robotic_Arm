#include "KPD_Interface.h"
#include "KPD_Config.h"


static u8 KPD_Arr[4][4] =
{
    {'7', '8', '9', '/'},
    {'4', '5', '6', '*'},
    {'1', '2', '3', '-'},
    {'C', '0', '=', '+'}
};


void KPD_Init(void)
{
    /* Rows PD0 - PD3 as Output */
    DIO_enumSetPinDirection(KPD_PORT, DIO_PIN0, DIO_PIN_OUTPUT);
    DIO_enumSetPinDirection(KPD_PORT, DIO_PIN1, DIO_PIN_OUTPUT);
    DIO_enumSetPinDirection(KPD_PORT, DIO_PIN2, DIO_PIN_OUTPUT);
    DIO_enumSetPinDirection(KPD_PORT, DIO_PIN3, DIO_PIN_OUTPUT);

    /* Columns PD4 - PD7 as Input */
    DIO_enumSetPinDirection(KPD_PORT, DIO_PIN4, DIO_PIN_INPUT);
    DIO_enumSetPinDirection(KPD_PORT, DIO_PIN5, DIO_PIN_INPUT);
    DIO_enumSetPinDirection(KPD_PORT, DIO_PIN6, DIO_PIN_INPUT);
    DIO_enumSetPinDirection(KPD_PORT, DIO_PIN7, DIO_PIN_INPUT);

    /* Rows HIGH */
    DIO_enumSetPinValue(KPD_PORT, DIO_PIN0, DIO_PIN_HIGH);
    DIO_enumSetPinValue(KPD_PORT, DIO_PIN1, DIO_PIN_HIGH);
    DIO_enumSetPinValue(KPD_PORT, DIO_PIN2, DIO_PIN_HIGH);
    DIO_enumSetPinValue(KPD_PORT, DIO_PIN3, DIO_PIN_HIGH);

    /* Columns HIGH -> Internal Pull-Up */
    DIO_enumSetPinValue(KPD_PORT, DIO_PIN4, DIO_PIN_HIGH);
    DIO_enumSetPinValue(KPD_PORT, DIO_PIN5, DIO_PIN_HIGH);
    DIO_enumSetPinValue(KPD_PORT, DIO_PIN6, DIO_PIN_HIGH);
    DIO_enumSetPinValue(KPD_PORT, DIO_PIN7, DIO_PIN_HIGH);
}


u8 KPD_u8GetPressed(void)
{
    u8 row;
    u8 col;
    u8 pinValue;

    for(row = 0; row < 4; row++)
    {
        /* All rows HIGH */
        DIO_enumSetPinValue(KPD_PORT, DIO_PIN0, DIO_PIN_HIGH);
        DIO_enumSetPinValue(KPD_PORT, DIO_PIN1, DIO_PIN_HIGH);
        DIO_enumSetPinValue(KPD_PORT, DIO_PIN2, DIO_PIN_HIGH);
        DIO_enumSetPinValue(KPD_PORT, DIO_PIN3, DIO_PIN_HIGH);

        /* Current row LOW */
        DIO_enumSetPinValue(KPD_PORT, row, DIO_PIN_LOW);

        for(col = 0; col < 4; col++)
        {
            /* Read column */
            DIO_enumGetPinValue(KPD_PORT,
                                col + 4,
                                &pinValue);

            if(pinValue == DIO_PIN_LOW)
            {
                /* Wait until key is released */
                do
                {
                    DIO_enumGetPinValue(KPD_PORT,
                                        col + 4,
                                        &pinValue);

                } while(pinValue == DIO_PIN_LOW);

                return KPD_Arr[row][col];
            }
        }
    }

    return NOTPRESSED;
}
