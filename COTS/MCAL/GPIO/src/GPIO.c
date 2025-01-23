#include "../inc/GPIO.h"
#include "../inc/GPIO_cfg.h"

// #define GPIOA_BASE_ADD ((volatile *) 0x40020000)
// #define GPIOB_BASE_ADD ((volatile *) 0x40020400)
// #define GPIOC_BASE_ADD ((volatile *) 0x40020800)
// #define GPIOD_BASE_ADD ((volatile *) 0x40020C00)
// #define GPIOE_BASE_ADD ((volatile *) 0x40021000)
// #define GPIOH_BASE_ADD ((volatile *) 0x40021C00)

#define GPIOx_BASE_ADD(GPIOx) (0x40020000 + (0x400 * GPIOx)) 

typedef struct
{
    volatile u32 GPIO_MODER;
    volatile u32 GPIO_OTYPER;
    volatile u32 GPIO_OSPEEDR;
    volatile u32 GPIO_PUPDR;
    volatile u32 GPIO_IDR;
    volatile u32 GPIO_ODR;
    volatile u32 GPIO_BSRR;
    volatile u32 GPIO_LCKR;
    volatile u32 GPIO_AFRL;
    volatile u32 GPIO_AFRH;

} GPIO_REGs_t;

GPIO_REGs_t * GPIOx_REG = (volatile GPIO_REGs_t *)GPIOx_BASE_ADD(GPIO_A);
