#ifndef MSYSTICK_H_
#define MSYSTICK_H_

#include "STD_TYPES.h"

typedef enum
{
    MSYSTICK_COUNT_EDISBLE = 0,
    MSYSTICK_COUNT_ENABLE,
    
} MSYSTICK_enuCountEnable_t;

typedef enum {
    MSYSTICK_DISABLE_EXCEPTION_REQUEST,
    MSYSTICK_ENABLE_EXCEPTION_REQUEST

} MSYSTICK_enuExceptionRequestStatus_t;

typedef enum
{
    MSYSTICK_OK=0,
    MSYSTICK_NULLPTR,

} MSYSTICK_enuErrorStatus_t;

typedef void (*systickcbtf_t) (void);

MSYSTICK_enuErrorStatus_t MSYSTICK_enuInit(void);
MSYSTICK_enuErrorStatus_t MSYSTICK_enuStart(void);
MSYSTICK_enuErrorStatus_t MSYSTICK_enuStop(void);
MSYSTICK_enuErrorStatus_t MSYSTICK_enuSetCyclicFunction(systickcbtf_t Add_CallBackFunction, u32 Copy_u32CyclicTime);
MSYSTICK_enuErrorStatus_t MSYSTICK_enuDelay_ms(u32 Copy_uint32DelayMs);

#endif