#include "../inc/MSYSTICK.h"
#include "../inc/MSYSTICK_cfg.h"

//extern u64 Global_uint64CurrentSystemClOCK;

static systickcbtf_t CallBack = NULL_PTR;
volatile u32 LoadVal = 0;
volatile u32 Counter = 0;

typedef struct{
    volatile u32 STK_CTRL;
    volatile u32 STK_LOAD;
    volatile u32 STK_VAL;
    volatile u32 STK_CALIB;

} MSYSTICK_StructREGS_t;

#define MSYSTICK_BASE_ADDRESS (0xE000E010)
#define MSYSTICK_REG ((volatile MSYSTICK_StructREGS_t*)MSYSTICK_BASE_ADDRESS)

#define MSYSTICK_COUNT_FLAG_MASK       0x00010000  // 1 if the count down reached 0
#define MSYSTICK_CLKSOURCE_MASK        0x00000004
#define MSYSTICK_EXCEPTION_REQ_EN_MASK 0x00000002  // 1 enable the writing CountFlag to set HIGH we the count down reach zero
#define MSYSTICK_COUNTER_EN_MASK       0x00000001

MSYSTICK_enuErrorStatus_t MSYSTICK_enuInit(void){
    volatile MSYSTICK_StructREGS_t* Loc_SYSTICK_REG = MSYSTICK_REG;
    MSYSTICK_enuErrorStatus_t Ret_enuErrorStatus = MSYSTICK_OK;
    #if MSYSTIC_CLOCK_SOURCE == MSYSTICK_CLOCK_SOURCE_AHB
        Loc_SYSTICK_REG->STK_CTRL |= MSYSTICK_CLKSOURCE_MASK;
    #elif MSYSTIC_CLOCK_SOURCE == MSYSTICK_CLOCK_SOURCE_AHB_BY_8 
        SYSTICK_REG->STK_CTRL &= ~MSYSTICK_CLKSOURCE_MASK;
    #endif

    #if MSYSTICK_TICKINT == MSYSTICK_ENABLE_EXCEPTION_REQUEST
        Loc_SYSTICK_REG->STK_CTRL |= MSYSTICK_EXCEPTION_REQ_EN_MASK;
    #elif MSYSTICK_TICKINT == MSYSTICK_DISABLE_EXCEPTION_REQUEST 
        Loc_SYSTICK_REG->STK_CTRL &= ~MSYSTICK_EXCEPTION_REQ_EN_MASK;
    #endif
    Loc_SYSTICK_REG->STK_VAL = 0x00;
    Loc_SYSTICK_REG->STK_LOAD = 0x3E81; // load = 16001 for 1msec delay

    return Ret_enuErrorStatus;
}

MSYSTICK_enuErrorStatus_t MSYSTICK_enuStart(void){
    MSYSTICK_enuErrorStatus_t Ret_enuErrorStatus = MSYSTICK_OK;
    volatile MSYSTICK_StructREGS_t* Loc_SYSTICK_REG = MSYSTICK_REG;
    Loc_SYSTICK_REG->STK_CTRL |= MSYSTICK_COUNTER_EN_MASK;
    return Ret_enuErrorStatus;
}

MSYSTICK_enuErrorStatus_t MSYSTICK_enuStop(void){
    MSYSTICK_enuErrorStatus_t Ret_enuErrorStatus = MSYSTICK_OK;
    volatile MSYSTICK_StructREGS_t* Loc_SYSTICK_REG = MSYSTICK_REG;
    Loc_SYSTICK_REG->STK_CTRL &= ~MSYSTICK_COUNTER_EN_MASK;
    return Ret_enuErrorStatus;
}

MSYSTICK_enuErrorStatus_t MSYSTICK_enuDelay_ms(u32 Copy_uint32DelayMs){
    MSYSTICK_enuErrorStatus_t Ret_enuErrorStatus = MSYSTICK_OK;
    volatile MSYSTICK_StructREGS_t* Loc_SYSTICK_REG = MSYSTICK_REG;


    return Ret_enuErrorStatus;
}

MSYSTICK_enuErrorStatus_t MSYSTICK_enuSetCyclicFunction(systickcbtf_t Addr_CallBackFunction, u32 Copy_uint32CyclicTime){
    MSYSTICK_enuErrorStatus_t Ret_enuErrorStatus = MSYSTICK_OK;
    if(Addr_CallBackFunction == NULL_PTR)
    {
        Ret_enuErrorStatus = MSYSTICK_NULLPTR;
    }
    else
    {
        LoadVal = Copy_uint32CyclicTime;  
        CallBack = Addr_CallBackFunction;

    }
    return Ret_enuErrorStatus;
}



//void __attribute__ ((section(".after_vectors"),weak))
SysTick_Handler(void){
    Counter++;
    if(Counter == LoadVal)
    {
        Counter = 0 ;
        CallBack();
    }
    return;
}
