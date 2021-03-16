#include "TEMem.h"
#include "TESW_EEPROM.h"
#include "TEMem_Cache.h"
#include "TEEvent.h"

TEEvent_New(memDriverThread)
{
    TEMainThread_SWEEPRom();
}

TEEvent_New(memUpdateThread)
{
    static uint16_t memTabCount = 0;

    uint16_t temp_data = 0;
    /* EEPROM为空 */
    if(TESWEEPROM_STATUS_READ_EMPTY == TESW_EEPROM_Read();)
}

void TEMem_Init(void)
{
    TEMem_Syn();
    TEEvent_Start(&memDriverThread,LOOP_Event,0);
}

void TEMem_Syn(void)
{
    uint16_t i = 0;
    TEMem_Addr temp_addr = 0;
    uint16_t temp_data = 0;
    for(i = 0;i<TEMEM_TAB_NUM;i++)
    {
        temp_addr = TEMem_GetAddr(i);
        TESW_EEPROM_Read(temp_addr,&temp_data);
        TEMem_SetData(i,temp_data);
    }
}
