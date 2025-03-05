#include "../COTS/MCAL/RCC/inc/RCC.h"
#include "../COTS/MCAL/MGPIO/inc/MGPIO.h"
#include "../COTS/MCAL/MSYSTICK/inc/MSYSTICK.h"
#include "../COTS/MCAL/MSYSTICK/inc/MSYSTICK_cfg.h"
#include "../COTS/HAL/HLED/inc/HLED.h"
#include "../COTS/HAL/HLED/inc/HLED_cfg.h"
#include "../COTS/HAL/HSCHED/inc/HSCHED.h"

void ToggleLED1(void);
void ToggleLED2(void);
void ToggleLED3(void);

HSCHED_RunnableCfg_t LED1 =
{
    .CallBack = ToggleLED1,
    .CyclicTime = 1000,
    .Priority = 0,
    .FirstDelay = 0
};

HSCHED_RunnableCfg_t LED2 =
{
    .CallBack = ToggleLED2,
    .CyclicTime = 2000,
    .Priority = 1,
    .FirstDelay = 0
};

HSCHED_RunnableCfg_t LED3 =
{
    .CallBack = ToggleLED3,
    .CyclicTime = 3000,
    .Priority = 2,
    .FirstDelay = 0
};

int main(void)
{

    RCC_enuEnableCLk(RCC_HSI_CLK);
    RCC_enuSelSysClk(RCC_HSI_CLK);
    RCC_enuEnPeriperal(RCC_GPIOA);
    HLED_init();
    HSCHED_enuInit();
    HSCHED_enuRegisterRunnable(&LED1);
    HSCHED_enuRegisterRunnable(&LED2);
    HSCHED_enuRegisterRunnable(&LED3);
    HSCHED_enuStart();

    while(1)
    {

    }
}

void ToggleLED1(void)
{
    static HLED_STATE_t LED_State = HLED_HIGH;
    HLED_enuSetLedState(LED_START,LED_State);
    LED_State = !LED_State;

}

void ToggleLED2(void)
{
    static HLED_STATE_t LED_State = HLED_HIGH;
    HLED_enuSetLedState(LED_STOP,LED_State);
    LED_State = !LED_State;

}

void ToggleLED3(void)
{
    static HLED_STATE_t LED_State = HLED_HIGH;
    HLED_enuSetLedState(LED_ALERT,LED_State);
    LED_State = !LED_State;

}