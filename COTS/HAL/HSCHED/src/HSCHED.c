#include "../inc/HSCHED.h"
#include "../inc/HSCHED_cfg.h"
#include "../inc/MSYSTICK.h"

static void HSCHED_CyclicFunc(void);
static HSCHED_RunnableCfg_t * HSCHED_strRunnableArray[MAX_RUNNABLES]; 

// init systick , set tick time , set callback (static func) hsched cyclic func
HSCHED_enuErrorStatus HSCHED_enuInit(void)
{
    HSCHED_enuErrorStatus Loc_enuErrorStatus = HSCHED_OK;
    Loc_enuErrorStatus = MSYSTICK_enuInit();
    MSYSTICK_enuSetCyclicFunction(HSCHED_CyclicFunc,1);
    return Loc_enuErrorStatus;
}

// save func in array [100]
HSCHED_enuErrorStatus HSCHED_enuRegisterRunnable(HSCHED_RunnableCfg_t * Add_HSCHED_cfg)
{
    HSCHED_enuErrorStatus Loc_enuErrorStatus = HSCHED_OK;
    if(NULL_PTR == Add_HSCHED_cfg)
    {
        Loc_enuErrorStatus = HSCHED_NULL_PTR;
    }
    else
    {
        if((NULL_PTR != HSCHED_strRunnableArray[Add_HSCHED_cfg->Priority]) || (Add_HSCHED_cfg->Priority >= MAX_RUNNABLES))
        {
            Loc_enuErrorStatus = HSCHED_WRONG_PRIORITY;
        }
        else if(NULL_PTR == Add_HSCHED_cfg->CallBack)
        {
            Loc_enuErrorStatus = HSCHED_NULL_PTR_CBF;
        }
        else
        {
            HSCHED_strRunnableArray[Add_HSCHED_cfg->Priority] = Add_HSCHED_cfg;
        }
    }
    return Loc_enuErrorStatus;
}

HSCHED_enuErrorStatus HSCHED_enuStart(void)
{
    HSCHED_enuErrorStatus Loc_enuErrorStatus = HSCHED_OK;
    Loc_enuErrorStatus = MSYSTICK_enuStart();
    return Loc_enuErrorStatus;
}

HSCHED_enuErrorStatus HSCHED_enuStop(void)
{
    HSCHED_enuErrorStatus Loc_enuErrorStatus = HSCHED_OK;
    Loc_enuErrorStatus = MSYSTICK_enuStop();
    return Loc_enuErrorStatus;
}
// modulas % (static variable --)
static void HSCHED_CyclicFunc(void)
{
    static u32 Tick = 1;
    for(u32 itr = 0 ; itr < MAX_RUNNABLES ; itr++)
    {
        if((NULL_PTR != HSCHED_strRunnableArray[itr]) && (NULL_PTR != HSCHED_strRunnableArray[itr]->CallBack))
        {
            if((0 == HSCHED_strRunnableArray[itr]->FirstDelay) && (Tick % HSCHED_strRunnableArray[itr]->CyclicTime == 0))
            {
                HSCHED_strRunnableArray[itr]->CallBack();
            }
            else if(0 != HSCHED_strRunnableArray[itr]->FirstDelay)
            {
                HSCHED_strRunnableArray[itr]->FirstDelay--;
            }
        }
    }
    Tick++;
}