#include "../inc/MGPIO.h"
#include "../inc/MGPIO_cfg.h"

typedef struct
{
    volatile u32 MGPIO_MODER;
    volatile u32 MGPIO_OTYPER;
    volatile u32 MGPIO_OSPEEDR;
    volatile u32 MGPIO_PUPDR;
    volatile u32 MGPIO_IDR;
    volatile u32 MGPIO_ODR;
    volatile u32 MGPIO_BSRR;
    volatile u32 MGPIO_LCKR;
    volatile u32 MGPIO_AFRL;
    volatile u32 MGPIO_AFRH;

} MGPIO_REGs_t;

#define MGPIOx_GET_BASE_ADD(MGPIOx) (MGPIO_REGs_t *)((u32)0x40020000 + ((u32)0x400 * (MGPIO_Port_t)MGPIOx)) 

MGPIO_enuErrorStatus_t MGPIO_enuPinCfg(MGPIO_PinCfg_t * Copy_strPinCfg)
{
    volatile MGPIO_REGs_t * MGPIO_REG = (volatile MGPIO_REGs_t *)MGPIOx_GET_BASE_ADD(Copy_strPinCfg->Port);
    MGPIO_enuErrorStatus_t Loc_enuErrorStatus = MGPIO_OK;
    if((Copy_strPinCfg->Port > MGPIO_H) || (Copy_strPinCfg->Port == 5) || (Copy_strPinCfg->Port == 6))
    {
        Loc_enuErrorStatus = MGPIO_INVALID_PORT;
    }
    else if(Copy_strPinCfg->Pin > MGPIO_PIN_15)
    {
        Loc_enuErrorStatus = MGPIO_INVALID_PIN;
    }
    else if(Copy_strPinCfg->Mode > MGPIO_ANALOG)
    {
        Loc_enuErrorStatus = MGPIO_INVALID_MODE;
    }
    else if(Copy_strPinCfg->OutputType > MGPIO_OPEN_DRAIN)
    {
        Loc_enuErrorStatus = MGPIO_INVALID_OUTPUT_TYPE; 
    }
    else if(Copy_strPinCfg->Speed > MGPIO_VERY_HIGH_SPEED)
    {
        Loc_enuErrorStatus = MGPIO_INVALID_SPEED;
    }
    else if(Copy_strPinCfg->PullUpDownType > MGPIO_PULL_DOWN)
    {
        Loc_enuErrorStatus = MGPIO_INVALID_PULLUPDOWN_TYPE;
    }
    else if(Copy_strPinCfg->AlternateFunction > MGPIO_AF15)
    {
        Loc_enuErrorStatus = MGPIO_INVALID_ALTERNATE_FUNCTION;
    }
    else
    {
        MGPIO_REG->MGPIO_MODER &= ~(Copy_strPinCfg->Mode << (Copy_strPinCfg->Pin*2));
        MGPIO_REG->MGPIO_MODER |= (Copy_strPinCfg->Mode << (Copy_strPinCfg->Pin*2));
        MGPIO_REG->MGPIO_OTYPER &= ~(Copy_strPinCfg->OutputType << (Copy_strPinCfg->Pin*2));
        MGPIO_REG->MGPIO_OTYPER |= (Copy_strPinCfg->OutputType << (Copy_strPinCfg->Pin*2));
        MGPIO_REG->MGPIO_OSPEEDR &= ~(Copy_strPinCfg->Speed << (Copy_strPinCfg->Pin*2));
        MGPIO_REG->MGPIO_OSPEEDR |= (Copy_strPinCfg->Speed << (Copy_strPinCfg->Pin*2));
        MGPIO_REG->MGPIO_PUPDR &= ~(Copy_strPinCfg->PullUpDownType << (Copy_strPinCfg->Pin*2));
        MGPIO_REG->MGPIO_PUPDR |= (Copy_strPinCfg->PullUpDownType << (Copy_strPinCfg->Pin*2));
        if(Copy_strPinCfg->Mode == MGPIO_ALT_FUNC)
        {
            if(Copy_strPinCfg->Pin < 8)
            {
                MGPIO_REG->MGPIO_AFRL &= ~((Copy_strPinCfg->AlternateFunction & 0xFF) << (Copy_strPinCfg->Pin*4));
                MGPIO_REG->MGPIO_AFRL |= ((Copy_strPinCfg->AlternateFunction & 0xFF) << (Copy_strPinCfg->Pin*4));
            }
            else
            {
                MGPIO_REG->MGPIO_AFRH &= ~((Copy_strPinCfg->AlternateFunction & 0xFF00) >> 8) << ((Copy_strPinCfg->Pin-8)*4);
                MGPIO_REG->MGPIO_AFRH |= ((Copy_strPinCfg->AlternateFunction & 0xFF00) >> 8) << ((Copy_strPinCfg->Pin-8)*4);
            }
        }
    }
    return Loc_enuErrorStatus;
}

MGPIO_enuErrorStatus_t MGPIO_enuSetPinValue(MGPIO_Port_t Copy_enuPort,MGPIO_Pin_t Copy_enuPin,MGPIO_PinValue_t Copy_enuPinValue)
{
    volatile MGPIO_REGs_t * MGPIO_REG = (volatile MGPIO_REGs_t *)MGPIOx_GET_BASE_ADD(Copy_enuPort);
    MGPIO_enuErrorStatus_t Loc_enuErrorStatus = MGPIO_OK;
    if((Copy_enuPort > MGPIO_H) || (Copy_enuPort == 5) || (Copy_enuPort == 6))
    {
        Loc_enuErrorStatus = MGPIO_INVALID_PORT;
    }
    else if(Copy_enuPin > MGPIO_PIN_15)
    {
        Loc_enuErrorStatus = MGPIO_INVALID_PIN;
    }
    else if(Copy_enuPinValue > MGPIO_PIN_HIGH)
    {
        Loc_enuErrorStatus = MGPIO_INVALID_PIN_VALUE;
    }
    else
    {
        MGPIO_REG->MGPIO_ODR &= ~(1 << Copy_enuPin);
        MGPIO_REG->MGPIO_ODR |= (Copy_enuPinValue << Copy_enuPin);
    }
    return Loc_enuErrorStatus;
}

MGPIO_enuErrorStatus_t MGPIO_enuGetPinValue(MGPIO_Port_t Copy_enuPort,MGPIO_Pin_t Copy_enuPin,u32* Add_u8PinValue)
{
    volatile MGPIO_REGs_t * MGPIO_REG = (volatile MGPIO_REGs_t *)MGPIOx_GET_BASE_ADD(Copy_enuPort);
    MGPIO_enuErrorStatus_t Loc_enuErrorStatus = MGPIO_OK;
    if((Copy_enuPort > MGPIO_H) || (Copy_enuPort == 5) || (Copy_enuPort == 6))
    {
        Loc_enuErrorStatus = MGPIO_INVALID_PORT;
    }
    else if(Copy_enuPin > MGPIO_PIN_15)
    {
        Loc_enuErrorStatus = MGPIO_INVALID_PIN;
    }
    else if(Add_u8PinValue == NULL_PTR)
    {
        Loc_enuErrorStatus = MGPIO_NULLPTR;
    }
    else
    {
        *Add_u8PinValue = (MGPIO_REG->MGPIO_IDR >> Copy_enuPin) & 1;
    }
    return Loc_enuErrorStatus;
}