#include "../inc/HLED.h"
#include "../inc/HLED_cfg.h"

HLED_Cfg_t ledCfgArr[NUM_OF_LEDS]=
{
    [LED_ALERT] = {
        .LedPortNum = MGPIO_A,
        .LedPinNumber = MGPIO_PIN_2,
        .LedPinType = MGPIO_PUSH_PULL,
        .LedInitState = HLED_LOW,
        .LedConnection = HLED_FORWARD
    },
    [LED_STOP] = {
        .LedPortNum = MGPIO_A,
        .LedPinNumber = MGPIO_PIN_1,
        .LedPinType = MGPIO_PUSH_PULL,
        .LedInitState = HLED_LOW,
        .LedConnection = HLED_FORWARD
    },
    [LED_START] ={
        .LedPortNum = MGPIO_A,
        .LedPinNumber = MGPIO_PIN_0,
        .LedPinType = MGPIO_PUSH_PULL,
        .LedInitState = HLED_LOW,
        .LedConnection = HLED_FORWARD
    }
};