#include "TECrc16_modbus.h"

#define TECRC16_INIT    0xFFFF
#define TECRC16_POLY    0xA001

uint16_t getCrc16_Modbus(uint8_t *data,uint16_t len)
{
    uint16_t INIT = TECRC16_INIT;
    uint16_t i,j;

    for(i = 0;i<len;i++)
    {
        INIT ^= (uint16_t)data[i];
        for(j=0;j<8;j++)
        {
            if(INIT & 0x01)
            {
                INIT >>= 1;
                INIT ^= TECRC16_POLY;
            }
            else
            {
                INIT >>= 1;
            }
        }
    }
    INIT = ((INIT & 0x00FF) <<8) | ((INIT & 0xFF00) >> 8);

    return INIT;
}
