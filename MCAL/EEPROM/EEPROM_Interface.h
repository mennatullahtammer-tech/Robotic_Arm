#ifndef MCAL_EEPROM_EEPROM_INTERFACE_H_
#define MCAL_EEPROM_EEPROM_INTERFACE_H_

#include "../../LIB/STD_TYPES.h"


void EEPROM_voidWriteByte(
    u16 Copy_u16Address,
    u8 Copy_u8Data
);


u8 EEPROM_u8ReadByte(
    u16 Copy_u16Address
);


/* =====================================================
 *              ROBOT POSITION
 * ===================================================== */


#endif
