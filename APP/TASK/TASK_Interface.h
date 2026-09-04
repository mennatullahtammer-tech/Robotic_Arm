#ifndef APP_TASK_TASK_INTERFACE_H_
#define APP_TASK_TASK_INTERFACE_H_

#include "../../LIB/STD_TYPES.h"


/* =====================================================
 *                  TASK TYPES
 * ===================================================== */

#define TASK_JOINT1       1
#define TASK_JOINT2       2
#define TASK_JOINT3       3
#define TASK_BASE_CW      4
#define TASK_BASE_CCW     5
#define TASK_GRIPPER_OPEN 6
#define TASK_GRIPPER_CLOSE 7


/* =====================================================
 *                  TASK FUNCTIONS
 * ===================================================== */

void TASK_voidInit(void);

void TASK_voidStartRecord(void);

void TASK_voidStopRecord(void);

void TASK_voidRecord(u8 Copy_u8Type, u8 Copy_u8Value);

void TASK_voidPlay(void);

void TASK_voidClear(void);

u8 TASK_u8GetCount(void);

u8 TASK_u8IsRecording(void);


#endif
