#ifndef HAL_EEPROM_POSITION_EEPROM_POSITION_H_
#define HAL_EEPROM_POSITION_EEPROM_POSITION_H_

#include "../../LIB/STD_TYPES.h"


/* Save complete robot position */

void EEPROM_Position_Save(
    u8 PositionNumber,
    u8 Joint1Angle,
    u8 Joint2Angle,
    u8 Joint3Angle,
    u16 BaseAngle,
    u8 GripperState
);


/* Load complete robot position */

u8 EEPROM_Position_Load(
    u8 PositionNumber,
    u8 *Joint1Angle,
    u8 *Joint2Angle,
    u8 *Joint3Angle,
    u16 *BaseAngle,
    u8 *GripperState
);


#endif
