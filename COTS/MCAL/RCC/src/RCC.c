#include "../inc/RCC.h"
#include "../inc/RCC_cfg.h"

typedef struct
{
    volatile u32 RCC_CR;
    volatile u32 RCC_PLLCFGR;
    volatile u32 RCC_CFGR;
    volatile u32 RCC_CIR;
    volatile u32 RCC_AHB1RSTR;
    volatile u32 RCC_AHB2RSTR;
    u32 Reserved0[2];
    volatile u32 RCC_APB1RSTR;
    volatile u32 RCC_APB2RSTR;
    u32 Reserved1[2];
    volatile u32 RCC_AHB1ENR;
    volatile u32 RCC_AHB2ENR;
    u32 Reserved2[2];
    volatile u32 RCC_APB1ENR;
    volatile u32 RCC_APB2ENR;
    u32 Reserved3[2];
    volatile u32 RCC_AHB1LPENR;
    volatile u32 RCC_AHB2LPENR;
    u32 Reserved4[2];
    volatile u32 RCC_APB1LPENR;
    volatile u32 RCC_APB2LPENR;
    u32 Reserved5[2];
    volatile u32 RCC_BDCR;
    volatile u32 RCC_CSR;
    u32 Reserved6[2];
    volatile u32 RCC_SSCGR;
    volatile u32 RCC_PLLI2SCFGR;
    u32 Reserved7;
    volatile u32 RCC_DCKCFGR;

} RCC_REG_t;

#define RCC_BASE_ADD (0x40023800UL)
#define RCC_REG ((RCC_REG_t*)RCC_BASE_ADD) // or create local var
#define NUMBER_OF_PERIPHERALS 33

u64 RCC_PeripheralArr[NUMBER_OF_PERIPHERALS]={
    /************ AHB1 **************/
    RCC_GPIOA,
    RCC_GPIOB,
    RCC_GPIOC,
    RCC_GPIOD,
    RCC_GPIOE,
    RCC_GPIOH,
    RCC_CRC,
    RCC_DMA1,
    RCC_DMA2,
    /************  AHB2 **************/   
    RCC_OTGFS,
    /************  ABP1 **************/  
    RCC_TIM2,
    RCC_TIM3,
    RCC_TIM4,
    RCC_TIM5,
    RCC_WWDG,
    RCC_SPI2,
    RCC_SPI3,
    RCC_USART2,
    RCC_I2C1,
    RCC_I2C2,
    RCC_I2C3,
    RCC_PWR,
    /************ ABP2 **************/
    RCC_TIM1,
    RCC_USART1,
    RCC_USART6,
    RCC_ADC1,
    RCC_SDIO,
    RCC_SPI1,
    RCC_SPI4,
    RCC_SYSCFG,
    RCC_TIM9,
    RCC_TIM10,
    RCC_TIM11
};

#define RCC_HSI_ON_MSK    (0x00000001UL)
#define RCC_HSI_RDY_MSK   (0x00000002UL)
#define RCC_HSE_ON_MSK    (0x00010000UL)
#define RCC_HSE_RDY_MSK   (0x00020000UL)
#define RCC_PLL_ON_MSK    (0x01000000UL)
#define RCC_PLL_RDY_MSK   (0x02000000UL)
#define RCC_HSI_RDY_BIT   1
#define RCC_HSE_RDY_BIT   17
#define RCC_PLL_RDY_BIT   25
#define RCC_SW_CLK_MSK    (0xFFFFFFFCUL)
#define RCC_PLL_EN_MSK    (0x03000000UL)

/*
•f(VCO clock) = f(PLL clock input) × (PLLN / PLLM)
•f(PLL general clock output) = f(VCO clock) / PLLP
•f(USB OTG FS, SDIO, RNG clock output) = f(VCO clock) / PLLQ
*/
RCC_enuErrorStatus_t RCC_enuCfgPLL(RCC_unionPLLCfg_t * Add_unionRCC_PLLCfg)
{
    volatile RCC_REG_t * RCC = (volatile RCC_REG_t *) RCC_BASE_ADD;
    RCC_enuErrorStatus_t Loc_RCC_errStatus = RCC_OK;
    u32 Loc_VCO = 0;
    u32 Loc_PLLClk = 0 ;
    u32 Loc_USB_OTG = 0;
    if(Add_unionRCC_PLLCfg == NULL_PTR)
    {
        Loc_RCC_errStatus = RCC_NULLPTR;
    }
    else if(RCC->RCC_CR & RCC_PLL_EN_MSK) // check if the pll is ON
    {
        Loc_RCC_errStatus = RCC_PLLON;
    }
    else if((Add_unionRCC_PLLCfg->Bits.CLK_SRC < RCC_PLL_SRC_HSI) || (Add_unionRCC_PLLCfg->Bits.CLK_SRC > RCC_PLL_SRC_HSE))
    {
        Loc_RCC_errStatus = RCC_PLL_INVALID_CLK_SRC;
    }
    else if((Add_unionRCC_PLLCfg->Bits.M_Prescaler < 2) || (Add_unionRCC_PLLCfg->Bits.M_Prescaler > 63)) // check if user configuration is valid ( he didn't set another bit )
    {
        Loc_RCC_errStatus = RCC_PLL_INVALID_M_PRESCALER;
    }
    else if((Add_unionRCC_PLLCfg->Bits.N_Multiplier < 192) || (Add_unionRCC_PLLCfg->Bits.N_Multiplier > 432)) // check if user configuration is valid ( he didn't set another bit )
    {
        Loc_RCC_errStatus = RCC_PLL_INVALID_N_MULTIPLIER;
    }
    else if((Add_unionRCC_PLLCfg->Bits.P_Prescaler < RCC_PLL_P_PRESCALER_2) || (Add_unionRCC_PLLCfg->Bits.P_Prescaler > RCC_PLL_P_PRESCALER_8)) // check if user configuration is valid
    {
        Loc_RCC_errStatus = RCC_PLL_INVALID_P_PRESCALER;
    }
    else if((Add_unionRCC_PLLCfg->Bits.Q_Prescaler < 2) || (Add_unionRCC_PLLCfg->Bits.Q_Prescaler > 15)) // check if user configuration is valid ( he didn't set another bit )
    {
        Loc_RCC_errStatus = RCC_PLL_INVALID_Q_PRESCALER;
    }
    else
    {
        if(Add_unionRCC_PLLCfg->Bits.CLK_SRC == RCC_PLL_SRC_HSI)
        {
            Loc_VCO = RCC_HSI_CLK_CFG / Add_unionRCC_PLLCfg->Bits.M_Prescaler;
        }
        else
        {
            Loc_VCO = RCC_HSE_CLK_CFG / Add_unionRCC_PLLCfg->Bits.M_Prescaler;
        }        
        if((Loc_VCO < 1000000) || (Loc_VCO > 2000000))
        {
            Loc_RCC_errStatus = RCC_PLL_INVALID_VCO;    
        }
        else
        {
            Loc_VCO *=  Add_unionRCC_PLLCfg->Bits.N_Multiplier;
            if((Loc_VCO < 192000000) || (Loc_VCO > 432000000))
            {
                Loc_RCC_errStatus = RCC_PLL_INVALID_VCO;
            }
            else
            {
                Loc_PLLClk = Loc_VCO / ((Add_unionRCC_PLLCfg->Bits.P_Prescaler+1)*2);
                Loc_USB_OTG = Loc_VCO / Add_unionRCC_PLLCfg->Bits.Q_Prescaler;
                if(Loc_PLLClk > 84000000)
                {
                    Loc_RCC_errStatus = RCC_PLL_INVALID_PLL_CLK;                    
                }
                else if(Loc_USB_OTG > 48000000)
                {
                    Loc_RCC_errStatus = RCC_PLL_INVALID_USB_CLK;
                }
            }
        }
        if(Loc_RCC_errStatus == RCC_OK)
        {
            u32 Loc_PLLCfg = RCC->RCC_PLLCFGR;
            Loc_PLLCfg &= (~Add_unionRCC_PLLCfg->All_Reg);
            Loc_PLLCfg |= Add_unionRCC_PLLCfg->All_Reg;
            RCC->RCC_PLLCFGR = Loc_PLLCfg;
        }
    }
    return Loc_RCC_errStatus;
}

RCC_enuErrorStatus_t RCC_enuEnableCLk(RCC_enuCLK_t Copy_enuCLK)
{
    volatile RCC_REG_t * Loc_RCC_REG = (RCC_REG_t*)RCC_BASE_ADD;
    RCC_enuErrorStatus_t Loc_enuErrorStatus = RCC_OK;
    u16 Loc_TimeOut = 1000;
    switch (Copy_enuCLK)
    {
        case RCC_HSI_CLK:
            Loc_RCC_REG->RCC_CR |= RCC_HSI_ON_MSK;
            while(!(Loc_RCC_REG->RCC_CR & RCC_HSI_RDY_MSK) && Loc_TimeOut--);
            if(!(Loc_RCC_REG->RCC_CR & RCC_HSI_RDY_MSK))
            {
                Loc_enuErrorStatus = RCC_CLK_TIMEOUT;
            }
            break;
        case RCC_HSE_CLK:
            Loc_RCC_REG->RCC_CR |= RCC_HSE_ON_MSK;
            while(!(Loc_RCC_REG->RCC_CR & RCC_HSE_RDY_MSK)&& Loc_TimeOut--);
            if(!(Loc_RCC_REG->RCC_CR & RCC_HSE_RDY_MSK))
            {
                Loc_enuErrorStatus = RCC_CLK_TIMEOUT;
            }
            break;
        case RCC_PLL_CLK:
            Loc_RCC_REG->RCC_CR |= RCC_PLL_ON_MSK;
            while(!(Loc_RCC_REG->RCC_CR & RCC_PLL_RDY_MSK)&& Loc_TimeOut--);
            if(!(Loc_RCC_REG->RCC_CR & RCC_PLL_RDY_MSK))
            {
                Loc_enuErrorStatus = RCC_CLK_TIMEOUT;
            }
            break;
        default:
            Loc_enuErrorStatus = RCC_INVALID_CLK_TYPE;
            break;
    }
    return Loc_enuErrorStatus;
}

RCC_enuErrorStatus_t RCC_enuDisableCLk(RCC_enuCLK_t Copy_enuCLK)
{
    RCC_REG_t * Loc_RCC_REG = (RCC_REG_t*)RCC_BASE_ADD;
    RCC_enuErrorStatus_t Loc_enuErrorStatus = RCC_OK;
    switch (Copy_enuCLK)
    {
        case RCC_HSI_CLK:
            Loc_RCC_REG->RCC_CR &= ~RCC_HSI_ON_MSK;
            break;
        case RCC_HSE_CLK:
            Loc_RCC_REG->RCC_CR &= ~RCC_HSE_ON_MSK;
            break;
        case RCC_PLL_CLK:
            Loc_RCC_REG->RCC_CR &= ~RCC_PLL_ON_MSK;
            break;
        default:
            Loc_enuErrorStatus = RCC_INVALID_CLK_TYPE;
            break;
    }
    return Loc_enuErrorStatus;
}

/* checks wither clock is enabled or disabled */
RCC_enuErrorStatus_t RCC_enuGetClkStatus(RCC_enuCLK_t Copy_enuCLK,u32* Add_ClkStatus)
{
    volatile RCC_REG_t * Loc_RCC_REG = (volatile RCC_REG_t*)RCC_BASE_ADD;
    RCC_enuErrorStatus_t Loc_enuErrorStatus = RCC_OK;
    if(Add_ClkStatus == NULL_PTR)
    {
        Loc_enuErrorStatus = RCC_NULLPTR;
    }
    else
    {
        switch (Copy_enuCLK)
        {
            case RCC_HSI_CLK:
                *Add_ClkStatus = (Loc_RCC_REG->RCC_CR & RCC_HSI_RDY_MSK) >> RCC_HSI_RDY_BIT;
                break;
            case RCC_HSE_CLK:
                *Add_ClkStatus = (Loc_RCC_REG->RCC_CR & RCC_HSE_RDY_MSK) >> RCC_HSE_RDY_BIT;
                break;
            case RCC_PLL_CLK:
                *Add_ClkStatus = (Loc_RCC_REG->RCC_CR & RCC_PLL_RDY_MSK) >> RCC_PLL_RDY_BIT;
                break;
            default:
                Loc_enuErrorStatus = RCC_INVALID_CLK_TYPE;
                break;
        }
    }
    return Loc_enuErrorStatus;
}

RCC_enuErrorStatus_t RCC_enuSelSysClk(RCC_enuCLK_t Copy_enuClockCfg){
    RCC_enuErrorStatus_t Local_enuErrorStatus = RCC_OK;
    if(Copy_enuClockCfg> RCC_PLL_CLK || Copy_enuClockCfg < RCC_HSI_CLK){
        Local_enuErrorStatus = RCC_INVALID_CLK_TYPE;
    }else{
        u32 Local_uint32ClockStatus;
        Local_enuErrorStatus = RCC_enuGetClkStatus(Copy_enuClockCfg,&Local_uint32ClockStatus);
        if(Local_enuErrorStatus == RCC_OK ){
            if(!Local_uint32ClockStatus){
                Local_enuErrorStatus = RCC_NOTREADY_CLK;
            }
            else{
                 u32 Local_uint32TempReg = ((RCC_REG->RCC_CFGR&RCC_SW_CLK_MSK) | ((u32) Copy_enuClockCfg));
                 RCC_REG->RCC_CFGR = Local_uint32TempReg;
            }
        }
    }
    return Local_enuErrorStatus;
}

RCC_enuErrorStatus_t RCC_enuEnPeriperal(u64 BusPeripheral)
{
    volatile RCC_REG_t * Loc_RCC_REG = (volatile RCC_REG_t*)RCC_BASE_ADD;
    RCC_enuErrorStatus_t Loc_enuErrorStatus = RCC_OK;
    RCC_PeriphBus_t Loc_Bus = (u32)(BusPeripheral >> 32);
    u32 Loc_Prephiral = (u32) BusPeripheral;
    if(!BinarySearch(BusPeripheral))
    {
        Loc_enuErrorStatus = RCC_INVALID_PERIPHERAL;
    }
    else
    {
        switch (Loc_Bus)
        {
            case RCC_AHP1_BUS :
                Loc_RCC_REG->RCC_AHB1ENR |= Loc_Prephiral;
                break;
            case RCC_AHP2_BUS :
                Loc_RCC_REG->RCC_AHB2ENR |= Loc_Prephiral;
                break;
            case RCC_APB1_BUS :
                Loc_RCC_REG->RCC_APB1ENR |= Loc_Prephiral;
                break;
            case RCC_APB2_BUS :
                Loc_RCC_REG->RCC_APB2ENR |= Loc_Prephiral;
                break;
            default:
                Loc_enuErrorStatus = RCC_INVALID_BUS;
                break;
        }
    }
    return Loc_enuErrorStatus;
}

RCC_enuErrorStatus_t RCC_enuDisPeriperal(u64 BusPeripheral)
{
   RCC_REG_t * Loc_RCC_REG = (RCC_REG_t*)RCC_BASE_ADD;
    RCC_enuErrorStatus_t Loc_enuErrorStatus = RCC_OK;
    RCC_PeriphBus_t Loc_Bus = (u32)(BusPeripheral >> 32);
    u32 Loc_Prephiral = (u32) BusPeripheral;
    if(!BinarySearch(BusPeripheral))
    {
        Loc_enuErrorStatus = RCC_INVALID_PERIPHERAL;
    }
    else
    {
        switch (Loc_Bus)
        {
            case RCC_AHP1_BUS :
                Loc_RCC_REG->RCC_AHB1ENR &= ~Loc_Prephiral;
                break;
            case RCC_AHP2_BUS :
                Loc_RCC_REG->RCC_AHB2ENR &= ~Loc_Prephiral;
                break;
            case RCC_APB1_BUS :
                Loc_RCC_REG->RCC_APB1ENR &= ~Loc_Prephiral;
                break;
            case RCC_APB2_BUS :
                Loc_RCC_REG->RCC_APB2ENR &= ~Loc_Prephiral;
                break;
            default:
                Loc_enuErrorStatus = RCC_INVALID_BUS;
                break;
        }
    }
    return Loc_enuErrorStatus;

}

u8 BinarySearch(u64 RCC_TargetPeripheral)
{
    u8 Start = 0,End = NUMBER_OF_PERIPHERALS-1, Mid;
    u8 Loc_IsPrephiral = 0;
    while(Start <= End)
    {
        Mid = (Start + End) / 2;
        if(RCC_PeripheralArr[Mid] == RCC_TargetPeripheral)
        {
            Loc_IsPrephiral = 1;
            break;
        }
        else if(RCC_PeripheralArr[Mid] > RCC_TargetPeripheral)
        {
            End = Mid-1;
        }
        else
        {
            Start = Mid+1;
        }
    }
    return Loc_IsPrephiral;
}
