#include "TEMCU_NVIC.h"
#include "ac780x.h"
#include "TEMacro.h"

/* 优先级等级 */
#define PRI_LEVEL0      0       //高
#define PRI_LEVEL1      1
#define PRI_LEVEL2      2
#define PRI_LEVEL3      3       //低

/* 默认优先级 */
#define PRI_DEFAULT     PRI_LEVEL3

/* 各中断优先级设置 */
#define PRI_PWDT0           PRI_DEFAULT
#define PRI_PWDT1           PRI_DEFAULT
#define PRI_PWM0            PRI_DEFAULT
#define PRI_PWM1            PRI_DEFAULT
#define PRI_ACMP0           PRI_DEFAULT
#define PRI_UART0           PRI_DEFAULT
#define PRI_UART1           PRI_DEFAULT
#define PRI_UART2               PRI_LEVEL3
#define PRI_WDG             PRI_DEFAULT
#define PRI_SPI0            PRI_DEFAULT
#define PRI_SPI1            PRI_DEFAULT
#define PRI_I2C0            PRI_DEFAULT
#define PRI_I2C1            PRI_DEFAULT
#define PRI_DMA0_CHANNEL0       PRI_LEVEL3
#define PRI_DMA0_CHANNEL1   PRI_DEFAULT
#define PRI_DMA0_CHANNEL2   PRI_DEFAULT
#define PRI_DMA0_CHANNEL3   PRI_DEFAULT
#define PRI_TIMER_CHANNEL0  PRI_DEFAULT
#define PRI_TIMER_CHANNEL1  PRI_DEFAULT
#define PRI_TIMER_CHANNEL2  PRI_DEFAULT
#define PRI_TIMER_CHANNEL3  PRI_DEFAULT
#define PRI_RTC             PRI_DEFAULT
#define PRI_PVD             PRI_DEFAULT
#define PRI_SPM             PRI_DEFAULT
#define PRI_CAN0            PRI_DEFAULT
#define PRI_ADC0            PRI_DEFAULT
#define PRI_ECC_SRAM        PRI_DEFAULT
#define PRI_EXTI0           PRI_DEFAULT
#define PRI_EXTI1           PRI_DEFAULT
#define PRI_EXTI2           PRI_DEFAULT
#define PRI_EXTI3_8         PRI_DEFAULT
#define PRI_EXTI9_15        PRI_DEFAULT

#define NVIC_Init(n)    NVIC_DisableIRQ(n##_IRQn);\
                        NVIC_SetPriority(n##_IRQn, PRI_##n);\
                        NVIC_ClearPendingIRQ(n##_IRQn);\
                        NVIC_EnableIRQ(n##_IRQn)

void TEMCU_NVIC_Init(void)
{
#ifdef  TE_IRQ_PWDT0
    NVIC_Init(PWDT0);
#endif

#ifdef  TE_IRQ_PWDT1
    NVIC_Init(PWDT1);
#endif

#ifdef  TE_IRQ_PWM0
    NVIC_Init(PWM0);
#endif

#ifdef  TE_IRQ_PWM1
    NVIC_Init(PWM1);
#endif

#ifdef  TE_IRQ_ACMP0
    NVIC_Init(ACMP0);
#endif

#ifdef  TE_IRQ_UART0
    NVIC_Init(UART0);
#endif

#ifdef  TE_IRQ_UART1
    NVIC_Init(UART1);
#endif

#ifdef  TE_IRQ_UART2
    NVIC_Init(UART2);
#endif

#ifdef  TE_IRQ_WDG
    NVIC_Init(WDG);
#endif

#ifdef  TE_IRQ_SPI0
    NVIC_Init(SPI0);
#endif

#ifdef  TE_IRQ_SPI1
    NVIC_Init(SPI1);
#endif

#ifdef  TE_IRQ_I2C0
    NVIC_Init(I2C0);
#endif

#ifdef  TE_IRQ_I2C1
    NVIC_Init(I2C1);
#endif

#ifdef  TE_IRQ_DMA0_CHANNEL0
    NVIC_Init(DMA0_CHANNEL0);
#endif

#ifdef  TE_IRQ_DMA0_CHANNEL1
    NVIC_Init(DMA0_CHANNEL1);
#endif

#ifdef  TE_IRQ_DMA0_CHANNEL2
    NVIC_Init(DMA0_CHANNEL2);
#endif

#ifdef  TE_IRQ_DMA0_CHANNEL3
    NVIC_Init(DMA0_CHANNEL3);
#endif

#ifdef  TE_IRQ_TIMER_CHANNEL0
    NVIC_Init(TIMER_CHANNEL0);
#endif

#ifdef  TE_IRQ_TIMER_CHANNEL1
    NVIC_Init(TIMER_CHANNEL1);
#endif

#ifdef  TE_IRQ_TIMER_CHANNEL2
    NVIC_Init(TIMER_CHANNEL2);
#endif

#ifdef  TE_IRQ_TIMER_CHANNEL3
    NVIC_Init(TIMER_CHANNEL3);
#endif

#ifdef  TE_IRQ_RTC
    NVIC_Init(RTC);
#endif

#ifdef  TE_IRQ_PVD
    NVIC_Init(PVD);
#endif

#ifdef  TE_IRQ_SPM
    NVIC_Init(SPM);
#endif

#ifdef  TE_IRQ_CAN0
    NVIC_Init(CAN0);
#endif

#ifdef  TE_IRQ_ADC0
    NVIC_Init(ADC0);
#endif

#ifdef  TE_IRQ_ECC_SRAM
    NVIC_Init(ECC_SRAM);
#endif

#ifdef  TE_IRQ_EXTI0
    NVIC_Init(EXTI0);
#endif

#ifdef  TE_IRQ_EXTI1
    NVIC_Init(EXTI1);
#endif

#ifdef  TE_IRQ_EXTI2
    NVIC_Init(EXTI2);
#endif

#ifdef  TE_IRQ_EXTI3_8
    NVIC_Init(EXTI3_8);
#endif

#ifdef  TE_IRQ_EXTI9_15
    NVIC_Init(EXTI9_15);
#endif
}

