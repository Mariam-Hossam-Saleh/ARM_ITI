#include "../inc/HLED.h"
#include "../inc/HLED_cfg.h"

extern HLED_Cfg_t ledCfgArr[NUM_OF_LEDS];

void HLED_init(void)
{
    for(u8 itration = 0 ; itration < NUM_OF_LEDS ; itration++)
    {
        MGPIO_PinCfg_t Pin_Cfg = {
            .Port = ledCfgArr[itration].LedPortNum,
            .Pin = ledCfgArr[itration].LedPinNumber,
            .Mode = MGPIO_OUTPUT,
            .OutputType = ledCfgArr[itration].LedPinType,
        };
        MGPIO_enuPinCfg(&Pin_Cfg);
        MGPIO_enuSetPinValue(ledCfgArr[itration].LedPortNum,ledCfgArr[itration].LedPinNumber,ledCfgArr[itration].LedInitState);
    }
}

HLED_enuErrorStatus_t HLED_enuSetLedState(HLED_LedsCfg_t Copy_u8LedName, HLED_STATE_t Copy_u8State)
{
    MGPIO_Port_t Loc_enuPortNum = ledCfgArr[Copy_u8LedName].LedPortNum;
    MGPIO_Pin_t Loc_enuPinNum = ledCfgArr[Copy_u8LedName].LedPinNumber;
	HLED_enuErrorStatus_t Loc_enuErrorStatus = HLED_NOK;
    
    if(Copy_u8LedName >= NUM_OF_LEDS)
    {
        Loc_enuErrorStatus = HLED_INVALID_LED_NAME;
    }
    else if(Copy_u8State > HLED_HIGH)
    {
        Loc_enuErrorStatus = HLED_INVALID_LED_STATE;
    }
    else
    {
        Loc_enuErrorStatus = HLED_OK;
        if(ledCfgArr[Copy_u8LedName].LedConnection == HLED_FORWARD)
        {
            MGPIO_enuSetPinValue(Loc_enuPortNum, Loc_enuPinNum, Copy_u8State);
        }
        else if(ledCfgArr[Copy_u8LedName].LedConnection == HLED_REVERSE)
        {
            MGPIO_enuSetPinValue(Loc_enuPortNum, Loc_enuPinNum, !Copy_u8State);
        }
        else
        {
            Loc_enuErrorStatus = HLED_INVALID_CONNECTION;
        }
    }
    return Loc_enuErrorStatus;
    
}