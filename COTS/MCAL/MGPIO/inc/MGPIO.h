#ifndef MGPIO_H_
#define MGPIO_H_

#include "../../STD_LIB/STD_TYPES.h"

typedef enum
{
    MGPIO_A = 0,
    MGPIO_B,
    MGPIO_C,
    MGPIO_D,
    MGPIO_E,
    MGPIO_H = 7

} MGPIO_Port_t;

typedef enum
{
    MGPIO_PIN_0 = 0,
    MGPIO_PIN_1,
    MGPIO_PIN_2,
    MGPIO_PIN_3,
    MGPIO_PIN_4,
    MGPIO_PIN_5,
    MGPIO_PIN_6,
    MGPIO_PIN_7,
    MGPIO_PIN_8,
    MGPIO_PIN_9,
    MGPIO_PIN_10,
    MGPIO_PIN_11,
    MGPIO_PIN_12,
    MGPIO_PIN_13,
    MGPIO_PIN_14,
    MGPIO_PIN_15

} MGPIO_Pin_t;

typedef enum
{
    MGPIO_INPUT = 0,
    MGPIO_OUTPUT,
    MGPIO_ALT_FUNC,
    MGPIO_ANALOG

} MGPIO_Mode_t;

typedef enum
{
    MGPIO_PUSH_PULL = 0,
    MGPIO_OPEN_DRAIN

} MGPIO_OutputType_t;

typedef enum 
{
    MGPIO_LOW_SPEED = 0,
    MGPIO_MEDIUM_SPEED,
    MGPIO_HIGH_SPEED,
    MGPIO_VERY_HIGH_SPEED

} MGPIO_Speed_t;

typedef enum 
{
    MGPIO_NO_PULLUP_PULLDOWN = 0,
    MGPIO_PULL_UP,
    MGPIO_PULL_DOWN,

} MGPIO_PullUpDown_t;

typedef enum
{
    MGPIO_AF0 = 0,
    MGPIO_AF1,     
    MGPIO_AF2, 
    MGPIO_AF3, 
    MGPIO_AF4, 
    MGPIO_AF5, 
    MGPIO_AF6, 
    MGPIO_AF7, 
    MGPIO_AF8, 
    MGPIO_AF9, 
    MGPIO_AF10,
    MGPIO_AF11,
    MGPIO_AF12,
    MGPIO_AF13,
    MGPIO_AF14,
    MGPIO_AF15,

} MGPIO_AlternateFunction_t;

typedef enum
{
    MGPIO_OK = 0,
    MGPIO_NOK,
    MGPIO_NULLPTR,
    MGPIO_INVALID_PORT,
    MGPIO_INVALID_PIN,
    MGPIO_INVALID_MODE,
    MGPIO_INVALID_OUTPUT_TYPE,
    MGPIO_INVALID_SPEED,
    MGPIO_INVALID_PULLUPDOWN_TYPE,
    MGPIO_INVALID_ALTERNATE_FUNCTION,
    MGPIO_INVALID_PIN_VALUE
    
} MGPIO_enuErrorStatus_t;

typedef enum 
{
    MGPIO_PIN_LOW = 0,
    MGPIO_PIN_HIGH,

} MGPIO_PinValue_t;

typedef struct 
{
    MGPIO_Port_t Port;
    MGPIO_Pin_t Pin;
    MGPIO_Mode_t Mode;
    MGPIO_OutputType_t OutputType;
    MGPIO_Speed_t Speed;
    MGPIO_PullUpDown_t PullUpDownType;
    MGPIO_AlternateFunction_t AlternateFunction;

} MGPIO_PinCfg_t;

MGPIO_enuErrorStatus_t MGPIO_enuPinCfg(MGPIO_PinCfg_t * Copy_strPinCfg);

MGPIO_enuErrorStatus_t MGPIO_enuSetPinValue(MGPIO_Port_t Copy_enuPort,MGPIO_Pin_t Copy_enuPin,MGPIO_PinValue_t Copy_enuPinValue);

MGPIO_enuErrorStatus_t MGPIO_enuGetPinValue(MGPIO_Port_t Copy_enuPort,MGPIO_Pin_t Copy_enuPin,u32* Add_u8PinValue);


#endif // MGPIO_H_
