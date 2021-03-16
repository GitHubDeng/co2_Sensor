#include "TEMCU_Timer.h"
#include "TEMacro.h"
#include "string.h"
#include "ac780x_timer_reg.h"

#define Timer_CLK         (APB_BUS_FREQ)
#define DelayS(n)   (Timer_CLK*n-1)
#define Delay1ms    (Timer_CLK/1000-1)
#define Delay10ms   (Timer_CLK/100-1)

#ifdef TE_MODULAR_TIMER_CH0
    static void (*TimerCH0_CallBack)(void);
#endif
#ifdef TE_MODULAR_TIMER_CH1
    static void (*TimerCH1_CallBack)(void);
#endif
#ifdef TE_MODULAR_TIMER_CH2
    static void (*TimerCH2_CallBack)(void);
#endif
#ifdef TE_MODULAR_TIMER_CH3
    static void (*TimerCH3_CallBack)(void);
#endif

void TEMCU_Timer_CallBack_Hook(TEMCU_TimerCH_Type ch,void (*Timer_CallBack)(void))
{
    switch(ch)
    {
#ifdef TE_MODULAR_TIMER_CH0
        case TETimer_CH0:
        TimerCH0_CallBack = Timer_CallBack;
        break;
#endif
#ifdef TE_MODULAR_TIMER_CH1
        case TETimer_CH1:
        TimerCH1_CallBack = Timer_CallBack;
        break;
#endif
#ifdef TE_MODULAR_TIMER_CH2
        case TETimer_CH2:
        TimerCH2_CallBack = Timer_CallBack;
        break;
#endif
#ifdef TE_MODULAR_TIMER_CH3
        case TETimer_CH3:
        TimerCH3_CallBack = Timer_CallBack;
        break;
#endif
        default:
        break;
    }
}

#if defined TE_MODULAR_TIMER_CH0 || defined TE_MODULAR_TIMER_CH1 || defined TE_MODULAR_TIMER_CH2 || defined TE_MODULAR_TIMER_CH3
static void Timer_Task(void *device, uint32_t wpara, uint32_t lpara)
{		
		if(device == TIMER_CHANNEL0)
		{
#ifdef TE_MODULAR_TIMER_CH0
			TimerCH0_CallBack();
#endif
		}
		else if(device == TIMER_CHANNEL1)
		{
#ifdef TE_MODULAR_TIMER_CH1
			TimerCH1_CallBack();
#endif
		}
		else if(device == TIMER_CHANNEL2)
		{
#ifdef TE_MODULAR_TIMER_CH2
			TimerCH2_CallBack();
#endif
		}
		else if(device == TIMER_CHANNEL3)
		{
#ifdef TE_MODULAR_TIMER_CH3
			TimerCH3_CallBack();
#endif
		}
}
#endif

void TEMCU_Timer_Init(void)
{
#if defined TE_MODULAR_TIMER_CH0 || defined TE_MODULAR_TIMER_CH1 || defined TE_MODULAR_TIMER_CH2 || defined TE_MODULAR_TIMER_CH3
    TIMER_ConfigType  timerConfig;   //定时器配置对象
#endif
#ifdef	TE_MODULAR_TIMER_CH0
    memset(&timerConfig, 0, sizeof(timerConfig));   //情况配置对象

    timerConfig.periodValue = Delay1ms;             //周期
    timerConfig.interruptEn = ENABLE;               //中断使能
    timerConfig.linkModeEn = DISABLE;               //link模式除能
    timerConfig.timerEn = ENABLE;                   //定时器使能
    timerConfig.callBack = Timer_Task;              //设置回调

    TIMER_Init(TIMER_CHANNEL0, &timerConfig);
#endif

#ifdef	TE_MODULAR_TIMER_CH1
    memset(&timerConfig, 0, sizeof(timerConfig));

    timerConfig.periodValue = Delay1ms;
    timerConfig.interruptEn = ENABLE;
    timerConfig.linkModeEn = DISABLE;
    timerConfig.timerEn = ENABLE;
    timerConfig.callBack = Timer_Task;

    TIMER_Init(TIMER_CHANNEL1, &timerConfig);
#endif

#ifdef	TE_MODULAR_TIMER_CH2
    memset(&timerConfig, 0, sizeof(timerConfig));

    timerConfig.periodValue = Delay1ms;
    timerConfig.interruptEn = ENABLE;
    timerConfig.linkModeEn = DISABLE;
    timerConfig.timerEn = ENABLE;
    timerConfig.callBack = Timer_Task;

    TIMER_Init(TIMER_CHANNEL2, &timerConfig);
#endif

#ifdef	TE_MODULAR_TIMER_CH3
    memset(&timerConfig, 0, sizeof(timerConfig));

    timerConfig.periodValue = Delay1ms;
    timerConfig.interruptEn = ENABLE;
    timerConfig.linkModeEn = DISABLE;
    timerConfig.timerEn = ENABLE;
    timerConfig.callBack = Timer_Task;

    TIMER_Init(TIMER_CHANNEL3, &timerConfig);
#endif

}

