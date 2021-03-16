#ifndef TEMCU_DMA_H
#define TEMCU_DMA_H

#include "stdint.h"

typedef enum
{
    DMA_CH0 = 0,
    DMA_CH1,
    DMA_CH2,
    DMA_CH3
}TEDMA_CH_Type;

typedef enum
{
    DMA_INT_Complete = 0x01,
    DMA_INT_Half     = 0x02,
    DMA_INT_Error    = 0x04
}TEDMA_INT_Type;

void TEDMA_CallBack_Hook(TEDMA_CH_Type ch,void (*CallBack)(TEDMA_INT_Type intType));
void TEDMA_MemBuf_Set(TEDMA_CH_Type ch,uint32_t *buf,uint16_t len);

void TEMCU_DMA_Init(void);

#endif


