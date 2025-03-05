#include "../../HSCHED/inc/HSCHED.h"
#include "../../HSCHED/inc/HSCHED_cfg.h"
#include "../inc/HLCD.h"
#include "../inc/HLCD_cfg.h"


extern HLCD_strCfg_t HLCD_cfgArr[NUMBER_OF_LCD];

typedef enum
{
    INIT_POWER_ON = 0,
    INIT_FUNC_SET,
    INIT_DISPLAY_ON,
    INIT_DISPLAY_CLEAR,
    INIT_ENTRY_MODE,
    INIT_FINISH_STATE

} HLCD_enuInitState_t;

typedef enum
{
    INIT_STATE = 0,
    WRITE_STATE,
    IDLE_STATE

} HLCD_enuRunnableState_t;

typedef enum
{
    WRITE_COMMAND = 0,
    WRITE_CHAR,
    WRITE_STR,
    WRITE_NUM,
    WRITE_FINISH

} HLCD_enuWriteState_t;

#define DATA_BUFFER_SIZE 25

static void HLCD_enuRunnable(void);
static HLCD_enuErrorStatus_t HLCD_enuInitStateMachine(void);
static HLCD_enuErrorStatus_t HLCD_enuWriteStateMachine(void);
static HLCD_enuErrorStatus_t HLCD_enuWriteCommand(u8 Copy_u8Command,u8 Copy_u8LCDNum);
static HLCD_enuErrorStatus_t HLCD_enuWriteData(u8 Copy_u8Data,u8 Copy_u8LCDNum);
static HLCD_enuErrorStatus_t HLCD_enuGoToCGRAM(u8 Copy_BatternNum,u8 Copy_u8LCDNum);

static bool flag_isBusy[NUMBER_OF_LCD];
static HLCD_enuRunnableState_t Global_enuMachineCurrentState[NUMBER_OF_LCD];
static HLCD_enuWriteState_t Global_enuWriteState;
static u8 Global_u8LCDNum = 0;
static u8 Global_u8Command = 0;
static u8 Global_u8DataBuffer[DATA_BUFFER_SIZE];
static u8 Global_u8StrLength = 0;
static s64 Global_u64Num = 0; 

HSCHED_RunnableCfg_t Global_enuLCDRunnable =
{
    .CallBack = HLCD_enuRunnable,
    .CyclicTime = 2,
    .Priority = 0,
    .FirstDelay = 0
};

static void HLCD_enuRunnable(void)
{
    static HLCD_enuRunnableState_t Loc_enuRunnableState = INIT_STATE;
    switch (Loc_enuRunnableState)
    {
        case INIT_STATE:
            HLCD_enuInitStateMachine();
            break;
        case WRITE_STATE:
            HLCD_enuWriteStateMachine();
        default:
            break;
    }
}

static HLCD_enuErrorStatus_t HLCD_enuInitStateMachine(void)
{
    HLCD_enuErrorStatus_t Loc_enuErrorStatus = HLCD_OK;
    static HLCD_enuInitState_t Loc_enuInitState = INIT_POWER_ON;
    static u8 counter = 0;
    for(u8 itr = 0 ; itr < NUMBER_OF_LCD ; itr++)
    {
        switch (Loc_enuInitState)
        {
            case  INIT_POWER_ON:
            flag_isBusy[itr] = true;
                if(counter == 20)
                {
                    Loc_enuInitState = INIT_FUNC_SET;
                    counter = 0;
                }
                counter++;
            break;
            case  INIT_FUNC_SET:
                if(HLCD_cfgArr[itr].HLCD_Mode == HLCD_8_BIT_MODE)
                {
                    HLCD_enuWriteCommand(HLCD_CMD06_FUNC_SET_8_BIT_MODE | HLCD_CMD06_FUNC_SET_2_DISPLAY_LINE | HLCD_CMD06_FUNC_SET_5x7_DOTS,itr);
                }
                else if(HLCD_cfgArr[itr].HLCD_Mode == HLCD_4_BIT_MODE)
                {
                    HLCD_enuWriteCommand(HLCD_CMD06_FUNC_SET_4_BIT_MODE | HLCD_CMD06_FUNC_SET_2_DISPLAY_LINE | HLCD_CMD06_FUNC_SET_5x7_DOTS,itr);
                }
                Loc_enuInitState = INIT_DISPLAY_ON;
            break;
            case  INIT_DISPLAY_ON:
                // set EN pin to LOW
                MGPIO_enuSetPinValue(HLCD_cfgArr[itr].HLCD_ControlPins[HLCD_PIN_EN].HLCD_portNum,HLCD_cfgArr[itr].HLCD_ControlPins[HLCD_PIN_EN].HLCD_pinNum,MGPIO_PIN_LOW);
                HLCD_enuWriteCommand(HLCD_CMD04_DISPLAY_ON | HLCD_CMD04_DISPLAY_CURSOR_ON | HLCD_CMD04_DISPLAY_CURSOR_BLINK_ON , itr);
                Loc_enuInitState = INIT_DISPLAY_CLEAR;
            break;
            case  INIT_DISPLAY_CLEAR:
                // set EN pin to LOW
                MGPIO_enuSetPinValue(HLCD_cfgArr[itr].HLCD_ControlPins[HLCD_PIN_EN].HLCD_portNum,HLCD_cfgArr[itr].HLCD_ControlPins[HLCD_PIN_EN].HLCD_pinNum,MGPIO_PIN_LOW);
                HLCD_enuWriteCommand(HLCD_CMD01_CLR_DISPLAY , itr);
                Loc_enuInitState = INIT_ENTRY_MODE;
            break;
            case  INIT_ENTRY_MODE:
                // set EN pin to LOW
                MGPIO_enuSetPinValue(HLCD_cfgArr[itr].HLCD_ControlPins[HLCD_PIN_EN].HLCD_portNum,HLCD_cfgArr[itr].HLCD_ControlPins[HLCD_PIN_EN].HLCD_pinNum,MGPIO_PIN_LOW);
                HLCD_enuWriteCommand(HLCD_CMD03_ENTRY_MODE_CURSOR_DIR_INC | HLCD_CMD03_ENTRY_MODE_DISPLAY_SHIFT_OFF , itr);
                Loc_enuInitState = INIT_FINISH_STATE;
            break;
            case  INIT_FINISH_STATE:
                // set EN pin to LOW
                MGPIO_enuSetPinValue(HLCD_cfgArr[itr].HLCD_ControlPins[HLCD_PIN_EN].HLCD_portNum,HLCD_cfgArr[itr].HLCD_ControlPins[HLCD_PIN_EN].HLCD_pinNum,MGPIO_PIN_LOW);
                flag_isBusy[itr] = false;
                Global_enuMachineCurrentState[itr] = IDLE_STATE;
            break;
            default:
            break;
        }
    }
    return Loc_enuErrorStatus;
}

static HLCD_enuErrorStatus_t HLCD_enuWriteStateMachine(void)
{
    HLCD_enuErrorStatus_t Loc_enuErrorStatus = HLCD_OK;
    static u8 Loc_u8StrBufferCounter = 0;
    static s64 Loc_s64Num = 1;
    switch (Global_enuWriteState)
    {
        case WRITE_COMMAND:
            HLCD_enuWriteCommand(Global_u8LCDNum,Global_u8Command);
            // set EN pin to LOW
            MGPIO_enuSetPinValue(HLCD_cfgArr[Global_u8LCDNum].HLCD_ControlPins[HLCD_PIN_EN].HLCD_portNum,HLCD_cfgArr[Global_u8LCDNum].HLCD_ControlPins[HLCD_PIN_EN].HLCD_pinNum,MGPIO_PIN_LOW);
            Global_enuWriteState = WRITE_FINISH;
        break;
        case WRITE_CHAR:
            HLCD_enuWriteData(Global_u8DataBuffer[0],Global_u8LCDNum);
            // set EN pin to LOW
            MGPIO_enuSetPinValue(HLCD_cfgArr[Global_u8LCDNum].HLCD_ControlPins[HLCD_PIN_EN].HLCD_portNum,HLCD_cfgArr[Global_u8LCDNum].HLCD_ControlPins[HLCD_PIN_EN].HLCD_pinNum,MGPIO_PIN_LOW);
            Global_enuWriteState = WRITE_FINISH;
        break;
        case WRITE_STR:
            if(Loc_u8StrBufferCounter < Global_u8StrLength)
            {
                HLCD_enuWriteData(Global_u8DataBuffer[Loc_u8StrBufferCounter++],Global_u8LCDNum);
                // set EN pin to LOW
                MGPIO_enuSetPinValue(HLCD_cfgArr[Global_u8LCDNum].HLCD_ControlPins[HLCD_PIN_EN].HLCD_portNum,HLCD_cfgArr[Global_u8LCDNum].HLCD_ControlPins[HLCD_PIN_EN].HLCD_pinNum,MGPIO_PIN_LOW);
            }
            else
            {
                Global_enuWriteState = WRITE_FINISH;
            }
        break;
        case WRITE_NUM:
            if(Global_u64Num == 0)
            {
                HLCD_enuWriteData('0',Global_u8LCDNum);
                // set EN pin to LOW
                MGPIO_enuSetPinValue(HLCD_cfgArr[Global_u8LCDNum].HLCD_ControlPins[HLCD_PIN_EN].HLCD_portNum,HLCD_cfgArr[Global_u8LCDNum].HLCD_ControlPins[HLCD_PIN_EN].HLCD_pinNum,MGPIO_PIN_LOW);
                Global_enuWriteState = WRITE_FINISH;
            }
            else
            {
                if(Global_u64Num < 0)
                {
                    HLCD_enuWriteData('-',Global_u8LCDNum);
                    Global_u8LCDNum *= -1;
                    // set EN pin to LOW
                    MGPIO_enuSetPinValue(HLCD_cfgArr[Global_u8LCDNum].HLCD_ControlPins[HLCD_PIN_EN].HLCD_portNum,HLCD_cfgArr[Global_u8LCDNum].HLCD_ControlPins[HLCD_PIN_EN].HLCD_pinNum,MGPIO_PIN_LOW);
                    Global_enuWriteState = WRITE_FINISH;
                }
                // invert the number before sending
                while(Global_u64Num)
                {
                    Loc_s64Num = (Global_u64Num%10) + (Loc_s64Num*10);
                    Global_u64Num /= 10;
                }
                if(Loc_s64Num != 1)
                {
                    HLCD_enuWriteData((Loc_s64Num%10)+'0',Global_u8LCDNum);
                    Loc_s64Num /= 10;
                    // set EN pin to LOW
                    MGPIO_enuSetPinValue(HLCD_cfgArr[Global_u8LCDNum].HLCD_ControlPins[HLCD_PIN_EN].HLCD_portNum,HLCD_cfgArr[Global_u8LCDNum].HLCD_ControlPins[HLCD_PIN_EN].HLCD_pinNum,MGPIO_PIN_LOW);
                }
                else
                {
                    Global_enuWriteState = WRITE_FINISH;
                }
            }
        break;
        case WRITE_FINISH:
            flag_isBusy[Global_u8LCDNum] = false;
            Global_enuMachineCurrentState[Global_u8LCDNum] = IDLE_STATE;
        break;
        default:
        break;
    }
    return Loc_enuErrorStatus;
}

static HLCD_enuErrorStatus_t HLCD_enuWriteCommand(u8 Copy_u8Command,u8 Copy_u8LCDNum)
{
    HLCD_enuErrorStatus_t Loc_enuErrorStatus = HLCD_OK;
    MGPIO_enuSetPinValue(HLCD_cfgArr[Copy_u8LCDNum].HLCD_ControlPins[HLCD_PIN_RS].HLCD_portNum,HLCD_cfgArr[Copy_u8LCDNum].HLCD_ControlPins[HLCD_PIN_RS].HLCD_pinNum,MGPIO_PIN_LOW);
    MGPIO_enuSetPinValue(HLCD_cfgArr[Copy_u8LCDNum].HLCD_ControlPins[HLCD_PIN_RW].HLCD_portNum,HLCD_cfgArr[Copy_u8LCDNum].HLCD_ControlPins[HLCD_PIN_RW].HLCD_pinNum,MGPIO_PIN_LOW);
    for(u8 DataPinsItr = 0 ;DataPinsItr < 8 ; DataPinsItr++ )
    {
        if(HLCD_cfgArr[Copy_u8LCDNum].HLCD_Mode == HLCD_8_BIT_MODE)
        {
            Copy_u8Command >>= DataPinsItr;
            Loc_enuErrorStatus = MGPIO_enuSetPinValue(HLCD_cfgArr[Copy_u8LCDNum].HLCD_ControlPins[DataPinsItr].HLCD_portNum,HLCD_cfgArr[Copy_u8LCDNum].HLCD_ControlPins[DataPinsItr].HLCD_pinNum,Copy_u8Command & 0x01); 
        }
    //     else if(HLCD_cfgArr[Copy_u8LCDNum].HLCD_Mode == HLCD_4_BIT_MODE)
    //     {
    //         Copy_u8Command >>= DataPinsItr;
    //         Loc_enuErrorStatus = MGPIO_enuSetPinValue(HLCD_cfgArr[Copy_u8LCDNum].HLCD_ControlPins[DataPinsItr].HLCD_portNum,HLCD_cfgArr[Copy_u8LCDNum].HLCD_ControlPins[DataPinsItr].HLCD_pinNum,Copy_u8Command & 0x01);    
    //     }
    // }
    }
    // set EN pin to HIGH
    MGPIO_enuSetPinValue(HLCD_cfgArr[Copy_u8LCDNum].HLCD_ControlPins[HLCD_PIN_EN].HLCD_portNum,HLCD_cfgArr[Copy_u8LCDNum].HLCD_ControlPins[HLCD_PIN_EN].HLCD_pinNum,MGPIO_PIN_HIGH);
    return Loc_enuErrorStatus;
}

static HLCD_enuErrorStatus_t HLCD_enuWriteData(u8 Copy_u8Data,u8 Copy_u8LCDNum)
{
    HLCD_enuErrorStatus_t Loc_enuErrorStatus = HLCD_OK;
    MGPIO_enuSetPinValue(HLCD_cfgArr[Copy_u8LCDNum].HLCD_ControlPins[HLCD_PIN_RS].HLCD_portNum,HLCD_cfgArr[Copy_u8LCDNum].HLCD_ControlPins[HLCD_PIN_RS].HLCD_pinNum,MGPIO_PIN_HIGH);
    MGPIO_enuSetPinValue(HLCD_cfgArr[Copy_u8LCDNum].HLCD_ControlPins[HLCD_PIN_RW].HLCD_portNum,HLCD_cfgArr[Copy_u8LCDNum].HLCD_ControlPins[HLCD_PIN_RW].HLCD_pinNum,MGPIO_PIN_LOW);
    for(u8 DataPinsItr = 0 ;DataPinsItr < 8 ; DataPinsItr++ )
    {
        if(HLCD_cfgArr[Copy_u8LCDNum].HLCD_Mode == HLCD_8_BIT_MODE)
        {
            Copy_u8Data >>= DataPinsItr;
            Loc_enuErrorStatus = MGPIO_enuSetPinValue(HLCD_cfgArr[Copy_u8LCDNum].HLCD_ControlPins[DataPinsItr].HLCD_portNum,HLCD_cfgArr[Copy_u8LCDNum].HLCD_ControlPins[DataPinsItr].HLCD_pinNum,Copy_u8Data & 0x01); 
        }
    //     else if(HLCD_cfgArr[Copy_u8LCDNum].HLCD_Mode == HLCD_4_BIT_MODE)
    //     {
    //         Copy_u8Data >>= DataPinsItr;
    //         Loc_enuErrorStatus = MGPIO_enuSetPinValue(HLCD_cfgArr[Copy_u8LCDNum].HLCD_ControlPins[DataPinsItr].HLCD_portNum,HLCD_cfgArr[Copy_u8LCDNum].HLCD_ControlPins[DataPinsItr].HLCD_pinNum,Copy_u8Command & 0x01);    
    //     }
    // }
    }
    // set EN pin to HIGH
    MGPIO_enuSetPinValue(HLCD_cfgArr[Copy_u8LCDNum].HLCD_ControlPins[HLCD_PIN_EN].HLCD_portNum,HLCD_cfgArr[Copy_u8LCDNum].HLCD_ControlPins[HLCD_PIN_EN].HLCD_pinNum,MGPIO_PIN_HIGH);
    return Loc_enuErrorStatus;
}

HLCD_enuErrorStatus_t HLCD_enuAsyncInit(void)
{
    HLCD_enuErrorStatus_t Loc_enuErrorStatus = HLCD_OK;
    
    for(u8 itr = 0 ; itr < NUMBER_OF_LCD ; itr++)
    {
        flag_isBusy[itr] = true; 
        Global_enuMachineCurrentState[itr] = INIT_STATE;
        for(u8 DataPinsItr = 0 ;DataPinsItr < 8 ; DataPinsItr++ )
        {
            if(HLCD_cfgArr[itr].HLCD_Mode == HLCD_4_BIT_MODE)
            {
                MGPIO_PinCfg_t Loc_strPinCfg = {
                    .Port = HLCD_cfgArr[itr].HLCD_DataPins[DataPinsItr+4].HLCD_portNum,
                    .Pin = HLCD_cfgArr[itr].HLCD_DataPins[DataPinsItr+4].HLCD_pinNum,
                    .Mode = MGPIO_OUTPUT,
                    .OutputType = MGPIO_PUSH_PULL, 
                };
                Loc_enuErrorStatus = MGPIO_enuPinCfg(&Loc_strPinCfg); 
            }
            else
            {
                MGPIO_PinCfg_t Loc_strPinCfg = {
                    .Port = HLCD_cfgArr[itr].HLCD_DataPins[DataPinsItr].HLCD_portNum,
                    .Pin = HLCD_cfgArr[itr].HLCD_DataPins[DataPinsItr].HLCD_pinNum,
                    .Mode = MGPIO_OUTPUT,
                    .OutputType = MGPIO_PUSH_PULL, 
                };
                Loc_enuErrorStatus = MGPIO_enuPinCfg(&Loc_strPinCfg);
            }
        }
        for(u8 ControlPinsItr = 0;ControlPinsItr < 3; ControlPinsItr++ )
        {
            MGPIO_PinCfg_t Loc_strPinCfg = {
                .Port = HLCD_cfgArr[itr].HLCD_DataPins[ControlPinsItr].HLCD_portNum,
                .Pin = HLCD_cfgArr[itr].HLCD_DataPins[ControlPinsItr].HLCD_pinNum,
                .Mode = MGPIO_OUTPUT,
                .OutputType = MGPIO_PUSH_PULL, 
            };
            Loc_enuErrorStatus = MGPIO_enuPinCfg(&Loc_strPinCfg);
        }
    }
    HSCHED_enuRegisterRunnable(&Global_enuLCDRunnable);
    HSCHED_enuStart();
    return Loc_enuErrorStatus;
}

HLCD_enuErrorStatus_t HLCD_enuAsyncWriteCommand(u8 Copy_u8Command, u8 Copy_u8LCDNum)
{
    HLCD_enuErrorStatus_t Loc_enuErrorStatus = HLCD_OK;
    if(flag_isBusy[Copy_u8LCDNum] == true)
    {
        Loc_enuErrorStatus = HLCD_BUSY;
    }
    else
    {
        flag_isBusy[Copy_u8LCDNum] = true;
        Global_enuMachineCurrentState[Copy_u8LCDNum] = WRITE_STATE;
        Global_enuWriteState = WRITE_COMMAND;
        Global_u8LCDNum = Copy_u8LCDNum;
        Global_u8Command = Copy_u8Command;
    }
    return Loc_enuErrorStatus;
}

HLCD_enuErrorStatus_t HLCD_enuAsyncWriteChar(u8 Copy_u8Data,u8 Copy_u8LCDNum)
{
    HLCD_enuErrorStatus_t Loc_enuErrorStatus = HLCD_OK;
    if(flag_isBusy[Copy_u8LCDNum] == true)
    {
        Loc_enuErrorStatus = HLCD_BUSY;
    }
    else
    {
        flag_isBusy[Copy_u8LCDNum] = true;
        Global_enuMachineCurrentState[Copy_u8LCDNum] = WRITE_STATE;
        Global_enuWriteState = WRITE_CHAR;
        Global_u8LCDNum = Copy_u8LCDNum;
        Global_u8DataBuffer[0] = Copy_u8Data;
    }
    return Loc_enuErrorStatus;
}

HLCD_enuErrorStatus_t HLCD_enuAsyncWriteString(u8 * Add_String,u8 Copy_u8LCDNum)
{
    HLCD_enuErrorStatus_t Loc_enuErrorStatus = HLCD_OK;
    if(flag_isBusy[Copy_u8LCDNum] == true)
    {
        Loc_enuErrorStatus = HLCD_BUSY;
    }
    else
    {
        // get string length
        Global_u8StrLength = 0;
        while(Add_String[Global_u8StrLength] != '\0')
        {
            Global_u8StrLength++;
        }
        flag_isBusy[Copy_u8LCDNum] = true;
        Global_enuMachineCurrentState[Copy_u8LCDNum] = WRITE_STATE;
        Global_enuWriteState = WRITE_STR;
        Global_u8LCDNum = Copy_u8LCDNum;
        for(int itr = 0 ; itr < Global_u8StrLength ; itr++)
        {
            Global_u8DataBuffer[itr] = Add_String[itr];
        }
    }    
    
	return Loc_enuErrorStatus;
}

HLCD_enuErrorStatus_t HLCD_enuAsyncWriteNumber(s64 Copy_Number,u8 Copy_u8LCDNum)
{
    HLCD_enuErrorStatus_t Loc_enuErrorStatus = HLCD_OK;
    if(flag_isBusy[Copy_u8LCDNum] == true)
    {
        Loc_enuErrorStatus = HLCD_BUSY;
    }
    else
    {
        flag_isBusy[Copy_u8LCDNum] = true;
        Global_enuMachineCurrentState[Copy_u8LCDNum] = WRITE_STATE;
        Global_enuWriteState = WRITE_NUM;
        Global_u8LCDNum = Copy_u8LCDNum;
        Global_u64Num = Copy_Number;
    }    
	return Loc_enuErrorStatus;
} 

// HLCD_enuErrorStatus_t HLCD_enuGoToRowColumn(HLCD_Row_t Copy_enuRow, u8 Copy_enuColumn,u8 Copy_u8LCDNum)
// {
// 	HLCD_enuErrorStatus_t Loc_enuErrorStatus = HLCD_OK;
//     u8 Loc_u8Location = 128;
//     if((Copy_enuColumn > 39) || (Copy_enuColumn < 0))
//     {
//         Loc_enuErrorStatus = HLCD_INVALID_COLUMN;
//     }
//     else if(Copy_enuRow == HLCD_FIRST_LINE)
//     {
//         Loc_u8Location += Copy_enuColumn;
//         HLCD_enuWriteCommand(Loc_u8Location,Copy_u8LCDNum);
//     }
//     else if(Copy_enuRow == HLCD_SECOND_LINE)
//     {
//         Loc_u8Location += Copy_enuColumn + 0x40;
//         HLCD_enuWriteCommand(Loc_u8Location,Copy_u8LCDNum);
//     }
//     else
//     {
//         Loc_enuErrorStatus = HLCD_INVALID_LINE;
//     }
//     return Loc_enuErrorStatus;
// }

// HLCD_enuErrorStatus_t HLCD_enuStoreSpecialChar(u8 * Copy_u8BatternArr,u8 Copy_BatternNum,u8 Copy_u8LCDNum )
// {
// 	HLCD_enuErrorStatus_t Loc_enuErrorStatus = HLCD_OK;
//     if(NULL_PTR == Copy_u8BatternArr)
//     {
//         Loc_enuErrorStatus = HLCD_NULL_POINTER;
//     }
//     else if((Copy_BatternNum > 7) || (Copy_BatternNum < 0 ))
//     {
//         Loc_enuErrorStatus = HLCD_INVALID_BATTERN_NUMBER;
//     }
//     else
//     {
//         HLCD_enuGoToCGRAM(Copy_BatternNum,Copy_u8LCDNum);
//         for(u8 itr = 0 ; itr < 8 ; itr++)
//         {
//             HLCD_enuWriteData(Copy_u8BatternArr[itr],Copy_u8LCDNum);
//         }
//         HLCD_enuGoToRowColumn(HLCD_FIRST_LINE,0,Copy_u8LCDNum);
//     }
// 	return Loc_enuErrorStatus;
// }

// HLCD_enuErrorStatus_t HLCD_enuDisplaySpecialChar(u8 Copy_BatternNum,HLCD_Row_t Copy_enuRow, u8 Copy_enuColumn,u8 Copy_u8LCDNum)
// {
// 	HLCD_enuErrorStatus_t Loc_enuErrorStatus = HLCD_OK;
//     if((Copy_BatternNum > 7) || (Copy_BatternNum < 0))
//     {
//         Loc_enuErrorStatus = HLCD_INVALID_BATTERN_NUMBER;
//     }
//     else if(Copy_enuRow > HLCD_SECOND_LINE)
//     {
//         Loc_enuErrorStatus = HLCD_INVALID_LINE;
//     }
//     else if((Copy_enuColumn > 40) || (Copy_enuColumn < 0))
//     {
//         Loc_enuErrorStatus = HLCD_INVALID_COLUMN;
//     }
//     else
//     {
//         HLCD_enuGoToRowColumn(Copy_enuRow,Copy_enuColumn,Copy_u8LCDNum);
//         HLCD_enuWriteData(Copy_BatternNum,Copy_u8LCDNum);
//     }
// 	return Loc_enuErrorStatus;
// }

// static HLCD_enuErrorStatus_t HLCD_enuGoToCGRAM(u8 Copy_BatternNum,u8 Copy_u8LCDNum)
// {
// 	HLCD_enuErrorStatus_t Loc_enuErrorStatus = HLCD_OK;
//     u8 Loc_u8Location = 64;
//     if((Copy_BatternNum > 7) || (Copy_BatternNum < 0))
//     {
//         Loc_enuErrorStatus = HLCD_INVALID_COLUMN;
//     }
//     else
//     {
//         Loc_u8Location += (8 * Copy_BatternNum);
//         HLCD_enuWriteCommand(Loc_u8Location,Copy_u8LCDNum);
        
//     }
//     return Loc_enuErrorStatus;
// }
