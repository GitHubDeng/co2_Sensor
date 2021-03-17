#include "TECO2_Core.h"
#include "TEEvent.h"
#include "TECom_Cache.h"
#include "TEMem_Cache.h"

// TEEvent_New(synToComThread)
// {
//     TECom_SetData(TECom_SerialNumH,TEMem_GetData(TEMem_SerialNumH));
//     TECom_SetData(TECom_SerialNumL,TEMem_GetData(TEMem_SerialNumL));
//     TECom_SetData(TECom_OPSFlag,TEMem_GetData(TEMem_OPSFlag));
//     TECom_SetData(TECom_SampleTimeA,TEMem_GetData(TEMem_SampleTimeA));
//     TECom_SetData(TECom_SampleTimeB,TEMem_GetData(TEMem_SampleTimeB));
//     TECom_SetData(TECom_SinglePointCalPPM,TEMem_GetData(TEMem_SinglePointCalPPM));
//     TECom_SetData(TECom_ABCSampleRate,TEMem_GetData(TEMem_ABCSampleRate));
//     TECom_SetData(TECom_ABCEvalCount,TEMem_GetData(TEMem_ABCEvalCount));
//     TECom_SetData(TECom_ABCSampleCount,TEMem_GetData(TEMem_ABCSampleCount));
//     TECom_SetData(TECom_AltitudeH,TEMem_GetData(TEMem_AltitudeH));
//     TECom_SetData(TECom_AltitudeL,TEMem_GetData(TEMem_AltitudeL));
//     TECom_SetData(TECom_KPAThreshold0,TEMem_GetData(TEMem_KPAThreshold0));
//     TECom_SetData(TECom_KPAThreshold1,TEMem_GetData(TEMem_KPAThreshold1));
//     TECom_SetData(TECom_KPAThreshold2,TEMem_GetData(TEMem_KPAThreshold2));
//     TECom_SetData(TECom_KPAThreshold3,TEMem_GetData(TEMem_KPAThreshold3));


// }

TEEvent_New(synToMemThread)
{
    TEMem_SetData(TEMem_SerialNumH,TECom_GetData(TECom_SerialNumH));
    TEMem_SetData(TEMem_SerialNumL,TECom_GetData(TECom_SerialNumL));
    TEMem_SetData(TEMem_OPSFlag,TECom_GetData(TECom_OPSFlag));
    TEMem_SetData(TEMem_SampleTimeA,TECom_GetData(TECom_SampleTimeA));
    TEMem_SetData(TEMem_SampleTimeB,TECom_GetData(TECom_SampleTimeB));
    TEMem_SetData(TEMem_SinglePointCalPPM,TECom_GetData(TECom_SinglePointCalPPM));
    TEMem_SetData(TEMem_ABCSampleRate,TECom_GetData(TECom_ABCSampleRate));
    TEMem_SetData(TEMem_ABCEvalCount,TECom_GetData(TECom_ABCEvalCount));
    TEMem_SetData(TEMem_ABCSampleCount,TECom_GetData(TECom_ABCSampleCount));
    TEMem_SetData(TEMem_AltitudeH,TECom_GetData(TECom_AltitudeH));
    TEMem_SetData(TEMem_AltitudeL,TECom_GetData(TECom_AltitudeL));
    TEMem_SetData(TEMem_KPAThreshold0,TECom_GetData(TECom_KPAThreshold0));
    TEMem_SetData(TEMem_KPAThreshold1,TECom_GetData(TECom_KPAThreshold1));
    TEMem_SetData(TEMem_KPAThreshold2,TECom_GetData(TECom_KPAThreshold2));
    TEMem_SetData(TEMem_KPAThreshold3,TECom_GetData(TECom_KPAThreshold3));
}

TEEvent_New(synFromMemThread)
{
    TECom_SetData(TECom_SerialNumH,TEMem_GetData(TEMem_SerialNumH));
    TECom_SetData(TECom_SerialNumL,TEMem_GetData(TEMem_SerialNumL));
    TECom_SetData(TECom_OPSFlag,TEMem_GetData(TEMem_OPSFlag));
    TECom_SetData(TECom_SampleTimeA,TEMem_GetData(TEMem_SampleTimeA));
    TECom_SetData(TECom_SampleTimeB,TEMem_GetData(TEMem_SampleTimeB));
    TECom_SetData(TECom_SinglePointCalPPM,TEMem_GetData(TEMem_SinglePointCalPPM));
    TECom_SetData(TECom_ABCSampleRate,TEMem_GetData(TEMem_ABCSampleRate));
    TECom_SetData(TECom_ABCEvalCount,TEMem_GetData(TEMem_ABCEvalCount));
    TECom_SetData(TECom_ABCSampleCount,TEMem_GetData(TEMem_ABCSampleCount));
    TECom_SetData(TECom_AltitudeH,TEMem_GetData(TEMem_AltitudeH));
    TECom_SetData(TECom_AltitudeL,TEMem_GetData(TEMem_AltitudeL));
    TECom_SetData(TECom_KPAThreshold0,TEMem_GetData(TEMem_KPAThreshold0));
    TECom_SetData(TECom_KPAThreshold1,TEMem_GetData(TEMem_KPAThreshold1));
    TECom_SetData(TECom_KPAThreshold2,TEMem_GetData(TEMem_KPAThreshold2));
    TECom_SetData(TECom_KPAThreshold3,TEMem_GetData(TEMem_KPAThreshold3));

    TEEvent_Start(&synToMemThread,LOOP_Event,5);
}

void TECO2_Init(void)
{
    TEEvent_Start(&synFromMemThread,SINGLE_Event,100);
}
