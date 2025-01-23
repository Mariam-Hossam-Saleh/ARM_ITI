#ifndef GPIO_H_
#define GPIO_H_

#include "../../../STD_LIB/STD_TYPES.h"

typedef enum
{
    GPIO_A = 0,
    GPIO_B,
    GPIO_C,
    GPIO_D,
    GPIO_E,
    GPIO_H = 7

} GPIO_PORT_t;

typedef enum
{
    GPIO_PIN_0 = 0,
    GPIO_PIN_1,
    GPIO_PIN_2,
    GPIO_PIN_3,
    GPIO_PIN_4,
    GPIO_PIN_5,
    GPIO_PIN_6,
    GPIO_PIN_7,
    GPIO_PIN_8,
    GPIO_PIN_9,
    GPIO_PIN_10,
    GPIO_PIN_11,
    GPIO_PIN_12,
    GPIO_PIN_13,
    GPIO_PIN_14,
    GPIO_PIN_15

} GPIO_PIN_t;

typedef enum
{
    GPIO_PUSH_PULL_PULL_UP = 0,
    GPIO_PUSH_PULL_PULL_DOWN,
    GPIO_OPEN_DRAIN_PULL_UP,
    GPIO_OPEN_DRAIN_PULL_DOWN

} GPIO_OUTPUT_STATE;

typedef enum
{

} GPIO_OUTPUT_DATA;

typedef enum
{
    GPIO_OK,
    GPIO_NOK,
    GPIO_NULLPTR,
    GPIO_INVALID_PORT,
    GPIO_INVALID_PIN,
    GPIO_INVALID_BUS,
    
} RCC_enuErrorStatus_t;

#endif // GPIO_H_
