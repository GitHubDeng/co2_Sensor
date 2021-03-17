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
    static TEMem_TabType memTabCount = TEMem_TabHead;
    TEMem_Addr temp_addr = 0; 
    uint16_t temp_data = 0;
    if(memTabCount > TEMem_TabTail)
    {
        memTabCount = TEMem_TabHead;
    }
    temp_addr= TEMem_GetAddr(memTabCount);

    /* EEPROM为空 */
    if(TESWEEPROM_STATUS_READ_EMPTY == TESW_EEPROM_Read(temp_addr,&temp_data))
    {
        //写入
        if(TESWEEPROM_STATUS_OK == TESW_EEPROM_Write(temp_addr,temp_data))
        {
            memTabCount++;
        }
        return;
    }
    /* 数据更新 */
    if(TEMem_GetData(memTabCount) != temp_data)
    {
        //写入
        if(TESWEEPROM_STATUS_OK == TESW_EEPROM_Write(temp_addr,TEMem_GetData(memTabCount)))
        {
            memTabCount++;
        }
        return;
    }
    memTabCount++;
}

TEEvent_New(memSynThread)
{
    static TEMem_TabType memTabCount = TEMem_TabHead;
    TEMem_Addr temp_addr = TEMem_GetAddr(memTabCount);
    uint16_t temp_data = 0;
    switch(TESW_EEPROM_Read(temp_addr,&temp_data))
    {
        case TESWEEPROM_STATUS_OK:
        case TESWEEPROM_STATUS_READ_EMPTY:
            TEMem_SetData(memTabCount,temp_data);
            memTabCount++;
            break;
        case TESWEEPROM_STATUS_BUSY:
        case TESWEEPROM_STATUS_ERROR:
            break;
        default:
            break;
    }
    if(memTabCount > TEMem_TabTail)
    {
        memTabCount = TEMem_TabHead;
        TEEvent_Stop(&memSynThread);
    }
}

// void TEMem_Syn(void)
// {
//     TEMem_TabType memTabCount = TEMem_TabHead;
//     TEMem_Addr temp_addr = 0;
//     uint16_t temp_data = 0;
//     for(memTabCount = TEMem_TabHead;memTabCount<=TEMem_TabTail;memTabCount++)
//     {
//         temp_addr = TEMem_GetAddr(memTabCount);
//         TESW_EEPROM_Read(temp_addr,&temp_data);
//         TEMem_SetData(memTabCount,temp_data);
//     }
// }



void TEMem_Init(void)
{
    TESW_EEPROM_Init();

    // TESW_EEPROM_EraseBlock1();
    // TESW_EEPROM_EraseBlock2();

    TEEvent_Start(&memSynThread,LOOP_Event,0);
    TEEvent_Start(&memDriverThread,LOOP_Event,0);
    TEEvent_Start(&memUpdateThread,LOOP_Event,5);
}

