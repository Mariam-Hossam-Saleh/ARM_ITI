#ifndef RCC_H_
#define RCC_H_

#include "../../STD_LIB/STD_TYPES.h"

/* Peripheral Buses */
typedef enum
{
    RCC_AHP1_BUS = 0,
    RCC_AHP2_BUS,
    RCC_APB1_BUS,
    RCC_APB2_BUS

}RCC_PeriphBus_t;

/* AHB1 */
#define RCC_GPIOA  (0x000000001ULL)
#define RCC_GPIOB  (0x000000002ULL)
#define RCC_GPIOC  (0x000000004ULL)
#define RCC_GPIOD  (0x000000008ULL)
#define RCC_GPIOE  (0x000000010ULL)
#define RCC_GPIOH  (0x000000080ULL)
#define RCC_CRC    (0x000001000ULL)
#define RCC_DMA1   (0x000200000ULL)
#define RCC_DMA2   (0x000400000ULL)

/* AHB2 */
#define RCC_OTGFS  (0x100000080ULL)

/* ABP1 */
#define RCC_TIM2   (0x2000000001ULL)
#define RCC_TIM3   (0x2000000002ULL)
#define RCC_TIM4   (0x2000000004ULL)
#define RCC_TIM5   (0x2000000008ULL)
#define RCC_WWDG   (0x2000000800ULL)
#define RCC_SPI2   (0x2000004000ULL)
#define RCC_SPI3   (0x2000008000ULL)
#define RCC_USART2 (0x2000020000ULL)
#define RCC_I2C1   (0x2000200000ULL)
#define RCC_I2C2   (0x2000400000ULL)
#define RCC_I2C3   (0x2000800000ULL)
#define RCC_PWR    (0x2010000000ULL)

/* ABP2 */
#define RCC_TIM1   (0x3000000001ULL)
#define RCC_USART1 (0x3000000010ULL)
#define RCC_USART6 (0x3000000020ULL)
#define RCC_ADC1   (0x3000000100ULL)
#define RCC_SDIO   (0x3000000800ULL)
#define RCC_SPI1   (0x3000001000ULL)
#define RCC_SPI4   (0x3000002000ULL)
#define RCC_SYSCFG (0x3000004000ULL)
#define RCC_TIM9   (0x3000010000ULL)
#define RCC_TIM10  (0x3000020000ULL)
#define RCC_TIM11  (0x3000040000ULL)

typedef enum
{
    RCC_HSI_CLK = 0,
    RCC_HSE_CLK,
    RCC_PLL_CLK   

} RCC_enuCLK_t;

typedef enum 
{
    RCC_PLL_SRC_HSI = 0,
    RCC_PLL_SRC_HSE

}RCC_enuPLL_SRC_t;

/* this value will be multiplied by 2 before using */
typedef enum 
{
    RCC_PLL_P_PRESCALER_2 = 0,
    RCC_PLL_P_PRESCALER_4,
    RCC_PLL_P_PRESCALER_6,
    RCC_PLL_P_PRESCALER_8

}RCC_enuPLL_P_CFG_t;

// typedef struct
// {
// 	u32 M_Prescaler;
// 	u32 N_Prescaler;
// 	u32 P_Prescaler;
// 	u32 PLLSRC;
// 	u32 Q_Prescaler;

// } RCC_PLLCfg_t;

typedef union {
    struct {
        u32 M_Prescaler                : 6;  // 6 bits for M
        u32 N_Multiplier               : 9;  // 9 bits for N
        u32                            : 1;  // 1 reserved bit
        RCC_enuPLL_P_CFG_t P_Prescaler : 2;  // 2 bits for P
        u32                            : 4;  // 4 reserved bits
        RCC_enuPLL_SRC_t CLK_SRC       : 1;  // 1 bit for SRC
        u32                            : 1;  // 1 reserved bit
        u32 Q_Prescaler                : 4;  // 4 bits for Q
        u32                            : 4;  // 4 reserved bits
    } Bits;           // Access individual fields
    u32 All_Reg;      // Access the entire register
} RCC_unionPLLCfg_t;

typedef enum
{
    RCC_OK,
    RCC_NOK,
    RCC_NULLPTR,
    RCC_INVALID_CLK_TYPE,
    RCC_CLK_TIMEOUT,
    RCC_CLK_DISABLED,
    RCC_NOTREADY_CLK,
    RCC_INVALID_PERIPHERAL,
    RCC_INVALID_BUS,
    RCC_PLLON,
    RCC_PLL_INVALID_CLK_SRC,
    RCC_PLL_INVALID_M_PRESCALER,
    RCC_PLL_INVALID_N_MULTIPLIER,
    RCC_PLL_INVALID_P_PRESCALER,
    RCC_PLL_INVALID_Q_PRESCALER,
    RCC_PLL_INVALID_VCO,
    RCC_PLL_INVALID_PLL_CLK,
    RCC_PLL_INVALID_USB_CLK

} RCC_enuErrorStatus_t;

RCC_enuErrorStatus_t RCC_enuCfgPLL(RCC_unionPLLCfg_t * Add_strRccPllCfg);

//RCC_enuErrorStatus_t RCC_enuCtlCLK(u32 CLK, u32 Status);

RCC_enuErrorStatus_t RCC_enuEnableCLk(RCC_enuCLK_t Copy_enuCLK);

RCC_enuErrorStatus_t RCC_enuDisableCLk(RCC_enuCLK_t Copy_enuCLK);

RCC_enuErrorStatus_t RCC_enuGetClkStatus(RCC_enuCLK_t Copy_enuCLK,u32* Add_ClkStatus);

RCC_enuErrorStatus_t RCC_enuSelSysClk(RCC_enuCLK_t Copy_enuClockCfg);

RCC_enuErrorStatus_t RCC_enuEnPeriperal(u64 Periperal);

RCC_enuErrorStatus_t RCC_enuDisPeriperal(u64 Periperal);

RCC_enuErrorStatus_t RCC_init();

u8 BinarySearch(u64 RCC_TargetPeripheral);

#endif /* RCC_H_ */