#ifndef TEMCU_TIMER_H
#define TEMCU_TIMER_H

#include "stdint.h"

typedef enum
{
    TETimer_CH0,
    TETimer_CH1,
    TETimer_CH2,
    TETimer_CH3
}TEMCU_TimerCH_Type;

void TEMCU_Timer_CallBack_Hook(TEMCU_TimerCH_Type ch,void (*Timer_CallBack)(void));
void TEMCU_Timer_Init(void);

#endif
