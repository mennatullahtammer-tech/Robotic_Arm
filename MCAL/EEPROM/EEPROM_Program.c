#include "EEPROM_Interface.h"
#include "EEPROM_Config.h"
#include "EEPROM_Private.h"

#include "../../LIB/BIT_Math.h"
#include <avr/io.h>


void EEPROM_voidWriteByte(u16 Copy_u16Address, u8 Copy_u8Data)
{
    if(Copy_u16Address >= EEPROM_SIZE)
    {
        return;
    }

    while(READ_BIT(EECR, EEPROM_EEWE))
    {
    }

    EEAR = Copy_u16Address;

    EEDR = Copy_u8Data;

    SET_BIT(EECR, EEPROM_EEMWE);

    SET_BIT(EECR, EEPROM_EEWE);

    while(READ_BIT(EECR, EEPROM_EEWE))
    {
    }
}


u8 EEPROM_u8ReadByte(u16 Copy_u16Address)
{
    if(Copy_u16Address >= EEPROM_SIZE)
    {
        return 0;
    }

    while(READ_BIT(EECR, EEPROM_EEWE))
    {
    }

    EEAR = Copy_u16Address;

    SET_BIT(EECR, 0);

    return EEDR;
}

