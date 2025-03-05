#include "../../COTS/HAL/HLED/inc/HLED.h"
#include "../../COTS/HAL/HLED/inc/HLED_cfg.h"
#include "../../COTS/MCAL/RCC/inc/RCC.h"

int main(void)
{

    RCC_enuEnableCLk(RCC_HSI_CLK);
    RCC_enuSelSysClk(RCC_HSI_CLK);
    RCC_enuEnPeriperal(RCC_GPIOA);

    HLED_init();
    while(1)
    {
        HLED_enuSetLedState(LED_START,HLED_HIGH);
        for(volatile u32 i = 0 ; i < 100000; i++);
        HLED_enuSetLedState(LED_START,HLED_LOW);
        for(volatile u32 i = 0 ; i < 100000; i++);

    }
}


