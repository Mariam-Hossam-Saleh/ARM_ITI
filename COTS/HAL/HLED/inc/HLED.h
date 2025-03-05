#ifndef HLED_H_
#define HLED_H_

#include "../../../STD_LIB/STD_TYPES.h"
#include "../../../COTS/MCAL/MGPIO/inc/MGPIO.h"
#include "HLED_cfg.h"

/**** Enum for Error Status ****/
typedef enum
{
	HLED_OK = 0,
	HLED_NOK,
	HLED_INVALID_LED_STATE,
	HLED_INVALID_LED_NAME,
    HLED_INVALID_CONNECTION
	
} HLED_enuErrorStatus_t ;

typedef enum
{
    HLED_FORWARD = 0,
    HLED_REVERSE

} HLED_ConnectionOption_t;

typedef enum
{
    HLED_LOW,
    HLED_HIGH

} HLED_STATE_t;

typedef struct
{
    MGPIO_Port_t LedPortNum;
    MGPIO_Pin_t LedPinNumber;
    MGPIO_OutputType_t LedPinType; // push-pull or open-drain
    HLED_STATE_t LedInitState;
    HLED_ConnectionOption_t LedConnection;

} HLED_Cfg_t;

void HLED_init(void);

HLED_enuErrorStatus_t HLED_enuSetLedState(HLED_LedsCfg_t Copy_u8LedName, HLED_STATE_t Copy_u8State);

#endif /* HLED_H_ */