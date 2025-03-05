#ifndef HSCHED_H
#define HSCHED_H

#include "../../STD_LIB/STD_TYPES.h"

typedef void (*schedCBF_t) (void);
typedef enum
{
    HSCHED_OK = 0,
    HSCHED_NOK,
    HSCHED_NULL_PTR,
    HSCHED_WRONG_PRIORITY,
    HSCHED_NULL_PTR_CBF,

} HSCHED_enuErrorStatus;

typedef struct 
{
    u32 Priority;
    u32 CyclicTime;
    schedCBF_t CallBack;
    u32 FirstDelay;

} HSCHED_RunnableCfg_t;

HSCHED_enuErrorStatus HSCHED_enuInit(void);
HSCHED_enuErrorStatus HSCHED_enuRegisterRunnable(HSCHED_RunnableCfg_t * Add_HSCHED_cfg);
HSCHED_enuErrorStatus HSCHED_enuStart(void);
HSCHED_enuErrorStatus HSCHED_enuStop(void);


#endif // HSCHED_H