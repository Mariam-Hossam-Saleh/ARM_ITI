#include "../../COTS/MCAL/RCC/inc/RCC.h"
#include "../../COTS/MCAL/RCC/inc/RCC_cfg.h"
#include "../../COTS/MCAL/MGPIO/inc/MGPIO.h"
#include "../../COTS/MCAL/MGPIO/inc/MGPIO_cfg.h"
#include "../../STD_LIB/STD_TYPES.h"

int main(void) 
{

    // for 32000000 PLL output
    RCC_unionPLLCfg_t PLL_CFG = {
        .Bits.N_Multiplier = 128,
        .Bits.M_Prescaler = 8,
        .Bits.P_Prescaler = RCC_PLL_P_PRESCALER_8,
        .Bits.Q_Prescaler = 6,
        .Bits.CLK_SRC = RCC_HSI_CLK,
    };
    // for 64000000 PLL output
    //  RCC_unionPLLCfg_t PLL_CFG = {
    //     .Bits.N_Multiplier = 128,
    //     .Bits.M_Prescaler = 8,
    //     .Bits.P_Prescaler = RCC_PLL_P_PRESCALER_4,
    //     .Bits.Q_Prescaler = 6,
    //     .Bits.CLK_SRC = RCC_HSI_CLK,
    // };
    // for 84000000 PLL output
    // RCC_unionPLLCfg_t PLL_CFG = {
    //     .Bits.N_Multiplier = 168,
    //     .Bits.M_Prescaler = 8,
    //     .Bits.P_Prescaler = RCC_PLL_P_PRESCALER_4,
    //     .Bits.Q_Prescaler = 7,
    //     .Bits.CLK_SRC = RCC_HSI_CLK,
    // };

    RCC_enuDisableCLk(RCC_PLL_CLK);
    RCC_enuCfgPLL(&PLL_CFG);
    
    RCC_enuErrorStatus_t status = RCC_OK;
    // Enable HSI Clock
    status = RCC_enuEnableCLk(RCC_PLL_CLK);
    if (status != RCC_OK) 
    {
        while (1);
    }

    // Select HSI as System Clock
    status = RCC_enuSelSysClk(RCC_PLL_CLK);
    if (status != RCC_OK) 
    {
        while (1);
    }

    // Enable MGPIOA Clock
    status = RCC_enuEnPeriperal(RCC_GPIOA);
    if (status != RCC_OK) 
    {
        while (1);
    }
    RCC_enuEnPeriperal(RCC_GPIOA);
MGPIO_PinCfg_t Pin0_Cfg = {
    .Port = MGPIO_A,
    .Pin = MGPIO_PIN_0,
    .Mode = MGPIO_OUTPUT,
    //.OutputType = MGPIO_PUSH_PULL
};
MGPIO_PinCfg_t Pin1_Cfg = {
    .Port = MGPIO_A,
    .Pin = MGPIO_PIN_1,
    .Mode = MGPIO_OUTPUT,
    //.OutputType = MGPIO_PUSH_PULL
};
MGPIO_PinCfg_t Pin2_Cfg = {
    .Port = MGPIO_A,
    .Pin = MGPIO_PIN_2,
    .Mode = MGPIO_OUTPUT,
    //.OutputType = MGPIO_PUSH_PULL
};
volatile u32 Loc_PinValue = 10;
MGPIO_enuPinCfg(&Pin0_Cfg);
MGPIO_enuPinCfg(&Pin1_Cfg);
MGPIO_enuPinCfg(&Pin2_Cfg);

    while(1)
    {
        MGPIO_enuSetPinValue(MGPIO_A,MGPIO_PIN_0,MGPIO_PIN_HIGH);
        MGPIO_enuGetPinValue(MGPIO_A,MGPIO_PIN_0,&Loc_PinValue);
        if(Loc_PinValue == MGPIO_PIN_HIGH)
        {
            MGPIO_enuSetPinValue(MGPIO_A,MGPIO_PIN_1,MGPIO_PIN_HIGH);
            MGPIO_enuSetPinValue(MGPIO_A,MGPIO_PIN_2,MGPIO_PIN_LOW);
        }
        for (volatile u32 i = 0; i < 1000000; i++);
        
        MGPIO_enuSetPinValue(MGPIO_A,MGPIO_PIN_0,MGPIO_PIN_LOW);
        MGPIO_enuGetPinValue(MGPIO_A,MGPIO_PIN_0,&Loc_PinValue);
        if(Loc_PinValue == MGPIO_PIN_LOW)
        {
            MGPIO_enuSetPinValue(MGPIO_A,MGPIO_PIN_2,MGPIO_PIN_HIGH);
            MGPIO_enuSetPinValue(MGPIO_A,MGPIO_PIN_1,MGPIO_PIN_LOW);
        }
        for (volatile u32 i = 0; i < 1000000; i++);
    }

    return 0;
}