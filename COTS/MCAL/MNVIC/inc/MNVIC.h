#ifndef MNVIC_H
#define MNVIC_H

#include "../../../STD_LIB/STD_TYPES.h"

typedef enum
{
    MNVIC_OK=0,
    MNVIC_NOK,
    MNVIC_INVALID_IRQ_NUMBER,
    MNVIC_ALREADY_ENABLED_IRQ,
    MNVIC_NULLPTR,
    MNVIC_INVALID_IRQ_PRIORITY,
    MNVIC_INVALID_PRI_GROUP,

} MNVIC_enuErrorStatus_t;

typedef enum 
{
    MNVIC_BIT_LOW = 0,
    MNVIC_BIT_HIGH,

} MNVIC_BitStatus_t;

typedef enum {
    MNVIC_enuWWDG,
    MNVIC_enuEXTI16_PVD,
    MNVIC_enuEXTI21_TAMP_STAMP,
    MNVIC_enuEXTI22_RTC_WKUP,
    MNVIC_enuFLASH,
    MNVIC_enuRCC,
    MNVIC_enuEXTI0,
    MNVIC_enuEXTI1,
    MNVIC_enuEXTI2,
    MNVIC_enuEXTI3,
    MNVIC_enuEXTI4,
    MNVIC_enuDMA1_Stream0,
    MNVIC_enuDMA1_Stream1,
    MNVIC_enuDMA1_Stream2,
    MNVIC_enuDMA1_Stream3,
    MNVIC_enuDMA1_Stream4,
    MNVIC_enuDMA1_Stream5,
    MNVIC_enuDMA1_Stream6,
    MNVIC_enuADC ,
    MNVIC_enuEXTI9_5=23,
    MNVIC_enuTIM1_BRK_TIM9,
    MNVIC_enuTIM1_UP_TIM10,
    MNVIC_enuTIM1_TRG_COM_TIM11,
    MNVIC_enuTIM1_CC,
    MNVIC_enuTIM2,
    MNVIC_enuTIM3,
    MNVIC_enuTIM4,
    MNVIC_enuI2C1_EV,
    MNVIC_enuI2C1_ER,
    MNVIC_enuI2C2_EV,
    MNVIC_enuI2C2_ER,
    MNVIC_enuSPI1,
    MNVIC_enuSPI2,
    MNVIC_enuUSART1,
    MNVIC_enuUSART2,
    MNVIC_enuEXTI15_10=40,
    MNVIC_enuEXTI17_RTC_Alarm,
    MNVIC_enuEXTI18_OTG_FS_WKUP,
    MNVIC_enuDMA1_Stream7=47,
    MNVIC_enuSDIO=49,
    MNVIC_enuTIM5,
    MNVIC_enuSPI3,
    MNVIC_enuDMA2_Stream0=56,
    MNVIC_enuDMA2_Stream1,
    MNVIC_enuDMA2_Stream2,
    MNVIC_enuDMA2_Stream3,
    MNVIC_enuDMA2_Stream4,
    MNVIC_enuOTG_FS=67,
    MNVIC_enuDMA2_Stream5,
    MNVIC_enuDMA2_Stream6,
    MNVIC_enuDMA2_Stream7,
    MNVIC_enuUSART6,
    MNVIC_enuI2C3_EV ,
    MNVIC_enuI2C3_ER,
    MNVIC_enuFPU=81,
    MNVIC_enuSPI4=84

} MNVIC_enuIRQn_t;

typedef enum{
    MNVIC_PRI_GROUP_16   = 0b000,
    MNVIC_PRI_GROUP_8    = 0b100,
    MNVIC_PRI_GROUP_4    = 0b101,
    MNVIC_PRI_GROUP_2    = 0b110,
    MNVIC_PRI_GROUP_NONE = 0b111,

} MNVIC_enuPriority_Grouping_t;

MNVIC_enuErrorStatus_t MNVIC_EnableIRQ(MNVIC_enuIRQn_t Copy_enuIRQnum);
MNVIC_enuErrorStatus_t MNVIC_DisableIRQ(MNVIC_enuIRQn_t Copy_enuIRQnum);
MNVIC_enuErrorStatus_t MNVIC_GetPendingIRQ(MNVIC_enuIRQn_t Copy_enuIRQnum, MNVIC_BitStatus_t* Addr_uint32PendingIRQ );
MNVIC_enuErrorStatus_t MNVIC_SetPendingIRQ(MNVIC_enuIRQn_t Copy_enuIRQnum);
MNVIC_enuErrorStatus_t MNVIC_ClearPendingIRQ (MNVIC_enuIRQn_t Copy_enuIRQnum);
MNVIC_enuErrorStatus_t MNVIC_GetActive(MNVIC_enuIRQn_t Copy_enuIRQnum, MNVIC_BitStatus_t* Addr_enuActiveIRQ );
MNVIC_enuErrorStatus_t MNVIC_SetPriority(MNVIC_enuIRQn_t Copy_enuIRQnum, u8 Copy_u8Priority);
MNVIC_enuErrorStatus_t MNVIC_GetPriority(MNVIC_enuIRQn_t Copy_enuIRQnum, MNVIC_BitStatus_t* Addr_enuPriority);
MNVIC_enuErrorStatus_t MNVIC_SystemReset(void);
MNVIC_enuErrorStatus_t MNVIC_SetPriorityGrouping(MNVIC_enuPriority_Grouping_t Copy_enuPriorityGrouping);

#endif // MNVIC_H