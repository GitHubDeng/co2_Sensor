#ifndef TEMCU_FLASH_H
#define TEMCU_FLASH_H

#include <ac780x.h>

#define TEMCU_FLASH_BASE_ADDRESS            (0x08000000UL)
#define TEMCU_FLASH_SIZE                    (0x00020000UL)
#define TEMCU_FLASH_PAGE_SIZE               (0x00000800UL)
#define TEMCU_FLASH_END_ADDRESS             (TEMCU_FLASH_BASE_ADDRESS+TEMCU_FLASH_SIZE)
#define TEMCU_FLASH_PAGE_AMOUNT             64


#define TEMCU_FLASH_ADDRESS_PAGE(n)         (TEMCU_FLASH_BASE_ADDRESS+(TEMCU_FLASH_PAGE_SIZE*n))

typedef uint32_t TEFLASH_Addr;

typedef enum
{
    TEMCU_FLASH_STATUS_DEFAULT = 0,
    TEMCU_FLASH_STATUS_OK,
    TEMCU_FLASH_STATUS_ERROR,
    TEMCU_FLASH_STATUS_ADDRESS_ERROR,
    TEMCU_FLASH_STATUS_BUSY
}TEMCU_FLASH_StatusType;

uint32_t TEMCU_Flash_Read(TEFLASH_Addr addr);

TEMCU_FLASH_StatusType TEMCU_Flash_Write(TEFLASH_Addr addr,uint32_t data);
TEMCU_FLASH_StatusType TEMCU_Flash_Erase(TEFLASH_Addr addr);
TEMCU_FLASH_StatusType TEMCU_Flash_EraseVerify(TEFLASH_Addr addr);


void TEMCU_Flash_ProgramCallBack_Hook(void (*func)(TEFLASH_Addr addr,TEMCU_FLASH_StatusType res));
void TEMCU_Flash_EraseCallBack_Hook(void (*func)(TEFLASH_Addr addr,TEMCU_FLASH_StatusType res));
void TEMCU_Flash_EraseVeriftCallBack_Hook(void (*func)(TEFLASH_Addr addr,TEMCU_FLASH_StatusType res));

void TEMCU_Flash_Init(void);
void TEMainThread_Flash(void);

#endif

