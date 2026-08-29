#ifndef MCAL_PWM_PWM_INTERFACE_H_
#define MCAL_PWM_PWM_INTERFACE_H_

#include "../../LIB/STD_TYPES.h"


/* Initialize Timer1 PWM */

void PWM_voidInit(void);


/* Set Compare Value on Channel A */

void PWM_voidSetCompareA(u16 Copy_u16Value);


/* Set Compare Value on Channel B */

void PWM_voidSetCompareB(u16 Copy_u16Value);


#endif
