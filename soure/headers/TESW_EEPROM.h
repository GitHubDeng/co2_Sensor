#ifndef TEMCU_SWEEPROM_H
#define TEMCU_SWEEPROM_H

#include "TETypes.h"

typedef uint16_t TESWEEPROM_Addr;

typedef enum
{
    TESWEEPROM_STATUS_OK = 1,
    TESWEEPROM_STATUS_READ_EMPTY,
    TESWEEPROM_STATUS_ERROR,
    TESWEEPROM_STATUS_BUSY
}TESW_EEPROM_StatusType;


TESW_EEPROM_StatusType TESW_EEPROM_Read(TESWEEPROM_Addr addr,uint16_t *data);
TESW_EEPROM_StatusType TESW_EEPROM_Write(TESWEEPROM_Addr addr,uint16_t data);

void TESW_EEPROM_EraseBlock1(void);
void TESW_EEPROM_EraseBlock2(void);

void TESW_EEPROM_Init(void);
void TEMainThread_SWEEPRom(void);

#endif
