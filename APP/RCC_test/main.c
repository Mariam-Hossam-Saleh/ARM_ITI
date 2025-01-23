#include "../COTS/MCAL/RCC/inc/RCC.h"
#include "../COTS/MCAL/RCC/inc/RCC_cfg.h"

#define GPIOA_BASE  0x40020000UL
#define GPIOA_MODER (*(volatile u32 *)(GPIOA_BASE + 0x00))
#define GPIOA_ODR   (*(volatile u32 *)(GPIOA_BASE + 0x14))

int main(void) 
{
    RCC_enuErrorStatus_t status;
    RCC_unionPLLCfg_t PLL_CFG;
    PLL_CFG.Bits.M_Prescaler = 16;
    PLL_CFG.Bits.N_Multiplier = 320;
    PLL_CFG.Bits.P_Prescaler = RCC_PLL_P_PRESCALER_8;
    PLL_CFG.Bits.CLK_SRC = RCC_HSI_CLK;
    PLL_CFG.Bits.Q_Prescaler = 15;

    RCC_enuDisableCLk(RCC_PLL_CLK);
    RCC_enuCfgPLL(&PLL_CFG);

    // Enable PLL Clock
    status = RCC_enuEnableCLk(RCC_PLL_CLK);
    if (status != RCC_OK) 
    {
        while (1);
    }

    // Select PLL as System Clock
    status = RCC_enuSelSysClk(RCC_PLL_CLK);
    if (status != RCC_OK) 
    {
        while (1);
    }

    // Enable GPIOA Clock
    status = RCC_enuEnPeriperal(RCC_GPIOA);
    if (status != RCC_OK) 
    {
        while (1);
    }

    // Configure GPIOA Pin 0 as Output
    GPIOA_MODER &= 0xFFFFFFFC; // Clear mode bits for pin 0
    GPIOA_MODER |= 0x1;        // Set pin 0 to output mode

    while (1) 
    {
        GPIOA_ODR ^= 1;     // toggle LED
        for (volatile u32 i = 0; i < 800000; i++);
    }

    return 0;
}

