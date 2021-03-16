#ifndef TEMCU_H
#define TEMCU_H

#include "TEMCU_ADC.h"
#include "TEMCU_CLK.h"
#include "TEMCU_DMA.h"
#include "TEMCU_Flash.h"
#include "TEMCU_IO.h"
#include "TEMCU_NVIC.h"
#include "TEMCU_Timer.h"
#include "TEMCU_Uart.h"

void TEMCU_Init(void);
void TEMainThread_MCU(void);

#endif
