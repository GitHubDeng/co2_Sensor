#ifndef TECOM_CACHE_H
#define TECOM_CACHE_H

#include "TETypes.h"

#define TECOM_FRAME_NUM     21

typedef enum
{
    TECom_DeviceReset = 0,
    TECom_SinglePointCalStart = 1,
    TECom_ABCLogicCtrl = 2,
    TECom_SerialNum = 3,
    TECom_SerialNumL = 4,
    TECom_OPSFlag = 5,
    TECom_SampleTimeA = 6,
    TECom_SampleTimeB = 7,
    TECom_SinglePointCalPPM = 8,
    TECom_ABCSampleRate = 9,
    TECom_ABCEvalCount = 10,
    TECom_ABCSampleCount = 11,
    TECom_Altitude = 12,
    TECom_AltitudeL = 13,
    TECom_KPAThreshold0 = 14,
    TECom_KPAThreshold1 = 15,
    TECom_KPAThreshold2 = 16,
    TECom_KPAThreshold3 = 17,
    TECom_FirmwareRevision = 18,
    TECom_Status = 19,
    TECom_PPM = 20
}TECom_StackType;


uint16_t TECom_GetData(TECom_StackType type);
void TECom_SetData(TECom_StackType type,uint16_t value);
uint16_t TECom_GetDataWithAddr(uint16_t addr);
void TECom_SetDataWithAddr(uint16_t addr,uint16_t value);
int TECom_CheckAddress(uint16_t addr);

#endif
