#ifndef TEMEM_CACHE_H
#define TEMEM_CACHE_H

#include "TETypes.h"

typedef uint16_t TEMem_Addr;

#define TEMEM_TAB_NUM   15

#define TEMem_TabHead   TEMem_SerialNumH
#define TEMem_TabTail   TEMem_KPAThreshold3

typedef enum
{
    TEMem_SerialNumH = 0,
    TEMem_SerialNumL = 1,
    TEMem_OPSFlag = 2,
    TEMem_SampleTimeA = 3,
    TEMem_SampleTimeB = 4,
    TEMem_SinglePointCalPPM = 5,
    TEMem_ABCSampleRate = 6,
    TEMem_ABCEvalCount = 7,
    TEMem_ABCSampleCount = 8,
    TEMem_AltitudeH = 9,
    TEMem_AltitudeL = 10,
    TEMem_KPAThreshold0 = 11,
    TEMem_KPAThreshold1 = 12,
    TEMem_KPAThreshold2 = 13,
    TEMem_KPAThreshold3 = 14
}TEMem_TabType;

uint16_t TEMem_GetData(TEMem_TabType type);
void TEMem_SetData(TEMem_TabType type,uint16_t value);

TEMem_Addr TEMem_GetAddr(TEMem_TabType type);

#endif
