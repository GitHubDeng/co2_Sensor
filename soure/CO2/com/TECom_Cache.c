#include "TECom_Cache.h"


uint16_t TECOM_StackAddr[TECOM_FRAME_NUM][2] = 
{
    {0x03E8,0},
    {0x03EC,0},
    {0x03EE,0},
    {0x0FA2,0},
    {0x0FA3,0},
    {0x0FA6,0},
    {0x0FA9,0},
    {0x0FAA,0},
    {0x0FC2,0},
    {0x0FC4,0},
    {0x0FC5,0},
    {0x0FC6,0},
    {0x0FD7,0},
    {0x0FD8,0},
    {0x1067,0},
    {0x1069,0},
    {0x106B,0},
    {0x106D,0},
    {0x1388,0},
    {0x1389,0},
    {0X138A,0}
};

uint16_t TECom_GetData(TECom_StackType type)
{
    return TECOM_StackAddr[type][1];
}

void TECom_SetData(TECom_StackType type,uint16_t value)
{
    TECOM_StackAddr[type][1] = value;
}

uint16_t TECom_GetDataWithAddr(uint16_t addr)
{
    uint16_t i = 0;
    for(i = 0;i<TECOM_FRAME_NUM;i++)
    {
        if(addr == TECOM_StackAddr[i][0])
        {
            return TECOM_StackAddr[i][1];
        }   
    }
    return 0;
}


void TECom_SetDataWithAddr(uint16_t addr,uint16_t value)
{
    uint16_t i = 0;
    for(i = 0;i<TECOM_FRAME_NUM;i++)
    {
        if(addr == TECOM_StackAddr[i][0])
        {
            TECOM_StackAddr[i][1] = value;
            return;
        }   
    }
}

int TECom_CheckAddress(uint16_t addr)
{
    uint16_t i = 0;
    for(i = 0;i<TECOM_FRAME_NUM;i++)
    {
        if(addr == TECOM_StackAddr[i][0])
        {
            return i;
        }   
    }
    return -1;
}
