#include "../inc/MNVIC.h"
#include "../inc/MNVIC_cfg.h"
#include "../../../STD_LIB/STD_TYPES.h"

#if STM == STM32F401CC
    #define IRQ_NUMBER MNVIC_enuSPI4
#else
    #define IRQ_NUMBER 239 
#endif

typedef struct{
    volatile u32 ISER[8];     // set enable
    const u32 RESERVED0[24];  // 96 bytes in-between
    volatile u32 ICER[8];     // clear enable
    const u32 RESERVED1[24];  // 96 bytes in-between
    volatile u32 ISPR[8];     // set pending
    const u32 RESERVED2[24];  // 96 bytes in-between
    volatile u32 ICPR[8];     // clear pending
    const u32 RESERVED3[24];  // 96 bytes in-between
    volatile u32 IABR[8];     // active bit
    const u32 RESERVED4[56];  // 224 bytes in-between
    volatile u32 IPR[60];     // priority
    const u32 RESERVED5[644]; // 2576 bytes in-between
    volatile u32 STIR;        // software trigger

} MNVIC_strREGS_t;

typedef struct{
    volatile u32 ACTLR; 
    const u32 RESERVED0[829];
    volatile u32 CPUID; 
    volatile u32 ICSR;
    volatile u32 VTOR;
    volatile u32 AIRCR; 
    volatile u32 SCR;
    volatile u32 CCR; 
    volatile u32 SHPR1; 
    volatile u32 SHPR2;
    volatile u32 SHPR3;
    volatile u32 SHCSR; 
    volatile u32 CFSR; 
    volatile u32 MMSR; 
    volatile u32 BFSR; 
    volatile u32 UFSR; 
    volatile u32 HFSR;
    volatile u32 MMAR;
    volatile u32 BFAR; 
    volatile u32 AFSR; 

} MSCB_strREGS_t;

#define MNVIC_BASE_ADDRESS 0xE000E100
#define MSCB_BASE_ADDRESS  0xE000E008
#define MSCB_REG ((volatile MSCB_strREGS_t*)MSCB_BASE_ADDRESS)
#define MNVIC_REG ((volatile MNVIC_strREGS_t*)MNVIC_BASE_ADDRESS) 

#define IS_PRI_MSK(X) ((X!=MNVIC_PRI_GROUP_16) || (X!=MNVIC_PRI_GROUP_8) || (X!=MNVIC_PRI_GROUP_4) || (X!=MNVIC_PRI_GROUP_2) || (X!=MNVIC_PRI_GROUP_NONE))
#define GROUP_PRI_MSK 0xFFFFF8FF
#define SYS_RESET_MSK 0x00000004
MNVIC_enuErrorStatus_t MNVIC_EnableIRQ(MNVIC_enuIRQn_t Copy_enuIRQnum)
{
    volatile MNVIC_strREGS_t * Loc_MNVIC_REG = MNVIC_REG;
    MNVIC_enuErrorStatus_t Loc_enuErrorStatus = MNVIC_OK;
    u32 Loc_u32RegNum = (Copy_enuIRQnum/32);
    u32 Loc_u32BitNum = (Copy_enuIRQnum%32);
    u32 Loc_u32IsIRQEnabled = ((Loc_MNVIC_REG->ISER[Loc_u32RegNum] >> Loc_u32BitNum) & 0x01);
    if(Copy_enuIRQnum >= IRQ_NUMBER)
    {
        Loc_enuErrorStatus = MNVIC_INVALID_IRQ_NUMBER;
    }
    else if(Loc_u32IsIRQEnabled == MNVIC_BIT_HIGH)
    {
        Loc_enuErrorStatus = MNVIC_ALREADY_ENABLED_IRQ;
    }
    else
    {
        Loc_MNVIC_REG->ICPR[Loc_u32RegNum] |= (1<<Loc_u32BitNum); // Clear Pinding Reg
        Loc_MNVIC_REG->ISER[Loc_u32RegNum] |= (1<<Loc_u32BitNum); // Enable IRQ
    }
    return Loc_enuErrorStatus;
}

MNVIC_enuErrorStatus_t MNVIC_DisableIRQ(MNVIC_enuIRQn_t Copy_enuIRQnum)
{
    volatile MNVIC_strREGS_t * Loc_MNVIC_REG = MNVIC_REG;
    MNVIC_enuErrorStatus_t Loc_enuErrorStatus = MNVIC_OK;
    u32 Loc_u32RegNum = (Copy_enuIRQnum/32);
    u32 Loc_u32BitNum = (Copy_enuIRQnum%32);
    if(Copy_enuIRQnum >= IRQ_NUMBER)
    {
        Loc_enuErrorStatus = MNVIC_INVALID_IRQ_NUMBER;
    }
    else
    {
        Loc_MNVIC_REG->ICER[Loc_u32RegNum] |= (1<<Loc_u32BitNum); // Disable IRQ
    }
    return Loc_enuErrorStatus;
}

MNVIC_enuErrorStatus_t MNVIC_GetPendingIRQ(MNVIC_enuIRQn_t Copy_enuIRQnum, MNVIC_BitStatus_t* Addr_enuPendingIRQ )
{
    volatile MNVIC_strREGS_t * Loc_MNVIC_REG = MNVIC_REG;
    MNVIC_enuErrorStatus_t Loc_enuErrorStatus = MNVIC_OK;
    if(Copy_enuIRQnum >= IRQ_NUMBER)
    {
        Loc_enuErrorStatus = MNVIC_INVALID_IRQ_NUMBER;
    }
    else if(NULL_PTR == Addr_enuPendingIRQ)
    {
        Loc_enuErrorStatus = MNVIC_NULLPTR;
    }
    else
    {
        u32 Loc_u32RegNum = (Copy_enuIRQnum/32);
        u32 Loc_u32BitNum = (Copy_enuIRQnum%32);
        *Addr_enuPendingIRQ = ((Loc_MNVIC_REG->ISPR[Loc_u32RegNum] >> Loc_u32BitNum) & 0x01); // Get Pending status IRQ
    }
    return Loc_enuErrorStatus;
}

MNVIC_enuErrorStatus_t MNVIC_SetPendingIRQ(MNVIC_enuIRQn_t Copy_enuIRQnum)
{
    volatile MNVIC_strREGS_t * Loc_MNVIC_REG = MNVIC_REG;
    MNVIC_enuErrorStatus_t Loc_enuErrorStatus = MNVIC_OK;
    if(Copy_enuIRQnum >= IRQ_NUMBER)
    {
        Loc_enuErrorStatus = MNVIC_INVALID_IRQ_NUMBER;
    }
    else
    {
        u32 Loc_u32RegNum = (Copy_enuIRQnum/32);
        u32 Loc_u32BitNum = (Copy_enuIRQnum%32);
        Loc_MNVIC_REG->ISPR[Loc_u32RegNum] |= (1<<Loc_u32BitNum); 
    }
    return Loc_enuErrorStatus;
}

MNVIC_enuErrorStatus_t MNVIC_ClearPendingIRQ (MNVIC_enuIRQn_t Copy_enuIRQnum)
{
    volatile MNVIC_strREGS_t * Loc_MNVIC_REG = MNVIC_REG;
    MNVIC_enuErrorStatus_t Loc_enuErrorStatus = MNVIC_OK;
    if(Copy_enuIRQnum >= IRQ_NUMBER)
    {
        Loc_enuErrorStatus = MNVIC_INVALID_IRQ_NUMBER;
    }
    else
    {
        u32 Loc_u32RegNum = (Copy_enuIRQnum/32);
        u32 Loc_u32BitNum = (Copy_enuIRQnum%32);
        Loc_MNVIC_REG->ICPR[Loc_u32RegNum] |= (1<<Loc_u32BitNum); 
    }
    return Loc_enuErrorStatus;
}

MNVIC_enuErrorStatus_t MNVIC_GetActive(MNVIC_enuIRQn_t Copy_enuIRQnum, MNVIC_BitStatus_t* Addr_enuActiveIRQ )
{
    volatile MNVIC_strREGS_t * Loc_MNVIC_REG = MNVIC_REG;
    MNVIC_enuErrorStatus_t Loc_enuErrorStatus = MNVIC_OK;
    if(Copy_enuIRQnum >= IRQ_NUMBER)
    {
        Loc_enuErrorStatus = MNVIC_INVALID_IRQ_NUMBER;
    }
    else if(NULL_PTR == Addr_enuActiveIRQ)
    {
        Loc_enuErrorStatus = MNVIC_NULLPTR;
    }
    else
    {
        u32 Loc_u32RegNum = (Copy_enuIRQnum/32);
        u32 Loc_u32BitNum = (Copy_enuIRQnum%32);
        *Addr_enuActiveIRQ = ((Loc_MNVIC_REG->IABR[Loc_u32RegNum] >> Loc_u32BitNum) & 0x01); // Get Active status IRQ
    }
    return Loc_enuErrorStatus;
}

MNVIC_enuErrorStatus_t MNVIC_SetPriority(MNVIC_enuIRQn_t Copy_enuIRQnum, u8 Copy_u8Priority)
{
    volatile MNVIC_strREGS_t * Loc_MNVIC_REG = MNVIC_REG;
    MNVIC_enuErrorStatus_t Loc_enuErrorStatus = MNVIC_OK;
    if(Copy_enuIRQnum >= IRQ_NUMBER)
    {
        Loc_enuErrorStatus = MNVIC_INVALID_IRQ_NUMBER;
    }
    else if(Copy_u8Priority > 15)
    {
        Loc_enuErrorStatus = MNVIC_INVALID_IRQ_PRIORITY;
    }
    else
    {
        Loc_MNVIC_REG->IPR[Copy_enuIRQnum] = (Copy_u8Priority<<4);      
    }
    return Loc_enuErrorStatus;
}

MNVIC_enuErrorStatus_t MNVIC_GetPriority(MNVIC_enuIRQn_t Copy_enuIRQnum, MNVIC_BitStatus_t* Addr_enuPriority)
{
    volatile MNVIC_strREGS_t * Loc_MNVIC_REG = MNVIC_REG;
    MNVIC_enuErrorStatus_t Loc_enuErrorStatus = MNVIC_OK;
    if(Copy_enuIRQnum >= IRQ_NUMBER)
    {
        Loc_enuErrorStatus = MNVIC_INVALID_IRQ_NUMBER;
    }
    else if(NULL_PTR == Addr_enuPriority)
    {
        Loc_enuErrorStatus = MNVIC_NULLPTR;
    }
    else
    {
        *Addr_enuPriority = (Loc_MNVIC_REG->IPR[Copy_enuIRQnum]>>4); // Get Priority
    }
    return Loc_enuErrorStatus;
}

MNVIC_enuErrorStatus_t MNVIC_SetPriorityGrouping(MNVIC_enuPriority_Grouping_t Copy_enuPriorityGrouping)
{
    volatile MSCB_strREGS_t * Loc_MSCB_REG = MSCB_REG;
    MNVIC_enuErrorStatus_t Loc_enuErrorStatus = MNVIC_OK;
    if(IS_PRI_MSK(Copy_enuPriorityGrouping))
    {
        Loc_enuErrorStatus = MNVIC_INVALID_PRI_GROUP;
    }
    else
    {
        u32 Loc_u32AIRCR = Loc_MSCB_REG->AIRCR;
        Loc_u32AIRCR &= (~GROUP_PRI_MSK);
        Loc_u32AIRCR |= (Copy_enuPriorityGrouping<<8);
        Loc_MSCB_REG->AIRCR = Loc_u32AIRCR;
    }
    return Loc_enuErrorStatus;    
}

MNVIC_enuErrorStatus_t MNVIC_SystemReset(void)
{
    volatile MSCB_strREGS_t * Loc_MSCB_REG = MSCB_REG;
    MNVIC_enuErrorStatus_t Loc_enuErrorStatus = MNVIC_OK;
    Loc_MSCB_REG->AIRCR |= SYS_RESET_MSK;
    return Loc_enuErrorStatus;
}