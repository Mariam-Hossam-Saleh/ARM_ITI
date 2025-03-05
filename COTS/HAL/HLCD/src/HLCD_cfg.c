#include "../inc/HLCD.h"
#include "../inc/HLCD_cfg.h"

HLCD_strCfg_t HLCD_cfgArr[NUMBER_OF_LCD] = {
    [LCD_ONE] = {
        .HLCD_Mode = HLCD_8_BIT_MODE,
        .HLCD_DataPins = {
            [HLCD_PIN_0] = {MGPIO_C, MGPIO_PIN_0},
            [HLCD_PIN_1] = {MGPIO_C, MGPIO_PIN_1},
            [HLCD_PIN_2] = {MGPIO_C, MGPIO_PIN_2},
            [HLCD_PIN_3] = {MGPIO_C, MGPIO_PIN_3},
            [HLCD_PIN_4] = {MGPIO_C, MGPIO_PIN_4},
            [HLCD_PIN_5] = {MGPIO_C, MGPIO_PIN_5},
            [HLCD_PIN_6] = {MGPIO_C, MGPIO_PIN_6},
            [HLCD_PIN_7] = {MGPIO_C, MGPIO_PIN_7},
        },
        .HLCD_ControlPins = {
            [HLCD_PIN_RS] = {MGPIO_D, MGPIO_PIN_2},
            [HLCD_PIN_RW] = {MGPIO_D, MGPIO_PIN_1},
            [HLCD_PIN_EN] = {MGPIO_D, MGPIO_PIN_0},
        }
    }
};

// typedef struct 
// {
//     MGPIO_Port_t Port;
//     MGPIO_Pin_t Pin;
//     MGPIO_Mode_t Mode;
//     MGPIO_OutputType_t OutputType;
//     MGPIO_Speed_t Speed;
//     MGPIO_PullUpDown_t PullUpDownType;
//     MGPIO_AlternateFunction_t AlternateFunction;

// } MGPIO_PinCfg_t;