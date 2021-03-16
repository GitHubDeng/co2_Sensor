#include "TEMCU_Flash.h"
#include "ac780x_eflash.h" 
#include "ac780x_eflash_reg.h"

#define TEMCU_FLASH_UNLOCK_KEY1                   0xac7811UL
#define TEMCU_FLASH_UNLOCK_KEY2                   0x01234567UL

#define TEFLASH_CTRL_DEFAULT    0
#define TEFLASH_CTRL_TRIGGER    1
#define TEFLASH_CTRL_EXECUTE    2
#define TEFLASH_CTRL_OVER       3

#define TEFLASH_CTRL_LOCK       0  
#define TEFLASH_CTRL_UNLOCK     1

#define TEFLASH_CTRL_FREE       0

static void (*FlashProgram_CallBack)(TEFLASH_Addr addr,TEMCU_FLASH_StatusType res);
static void (*FlashErase_CallBack)(TEFLASH_Addr addr,TEMCU_FLASH_StatusType res);
static void (*FlashEraseVerift_CallBack)(TEFLASH_Addr addr,TEMCU_FLASH_StatusType res);

struct TEMCU_Flash_Data_Tag
{
    TEFLASH_Addr _addr_erase;
    TEFLASH_Addr _addr_program;
    TEFLASH_Addr _addr_eraseVerift;
    uint32_t _data;
}st_flash_d;

struct TEMCU_Flash_Status_Tag
{
    TEMCU_FLASH_StatusType program;
    TEMCU_FLASH_StatusType erase;
    TEMCU_FLASH_StatusType eraseVerift;
}st_flash_s;

union TEMCU_Flash_Ctrl_Tag
{
    uint16_t _hw;
    struct TEMCU_Flash_Trigger
    {
        uint16_t                    : 13;
        uint16_t    _program        : 1;
        uint16_t    _erase          : 1;
        uint16_t    _eraseVerift    : 1;
    }_trigger;
    struct TEMCU_Flash_Step
    {
        uint16_t                    : 5;
        uint16_t    _unlock         : 2;
        uint16_t    _program        : 2;
        uint16_t    _erase          : 2;
        uint16_t    _eraseVerift    : 2;
        uint16_t                    : 3;
    }_step;
    struct TEMCU_Flash_Status
    {
        uint16_t    _unlock         : 1;
        uint16_t                    : 6;
        uint16_t    _busy           : 6;
        uint16_t                    : 3;
    }_status;
    struct TEMCU_Flash_Lock
    {
        uint16_t                    : 13;
        uint16_t    _trigger        : 3;
    }_unlock;
    struct TEMCU_Flash_UserStatus
    {
        uint16_t                    : 13;
        uint16_t    _program        : 1;
        uint16_t    _erase          : 1;
        uint16_t    _eraseVerift    : 1;
    }_busy;
}un_flash_c;

uint32_t TEMCU_Flash_Read(TEFLASH_Addr addr)
{
    return *(uint32_t*)addr;
}

TEMCU_FLASH_StatusType TEMCU_Flash_Write(TEFLASH_Addr addr,uint32_t data)
{
    if(TEFLASH_CTRL_FREE != un_flash_c._busy._program)
    {
        return TEMCU_FLASH_STATUS_BUSY;
    }
    st_flash_d._addr_program = addr;
    st_flash_d._data = data;
    un_flash_c._trigger._program = 1;
    st_flash_s.program = TEMCU_FLASH_STATUS_DEFAULT;
    return TEMCU_FLASH_STATUS_OK;
}

TEMCU_FLASH_StatusType TEMCU_Flash_Erase(TEFLASH_Addr addr)
{
    if(TEFLASH_CTRL_FREE != un_flash_c._busy._erase)
    {
        return TEMCU_FLASH_STATUS_BUSY;
    }
    st_flash_d._addr_erase = addr;
    un_flash_c._trigger._erase = 1;
    st_flash_s.erase = TEMCU_FLASH_STATUS_DEFAULT;
    return TEMCU_FLASH_STATUS_OK;
}

TEMCU_FLASH_StatusType TEMCU_Flash_EraseVerify(TEFLASH_Addr addr)
{
    if(TEFLASH_CTRL_FREE != un_flash_c._busy._eraseVerift)
    {
        return TEMCU_FLASH_STATUS_BUSY;
    }
    st_flash_d._addr_eraseVerift = addr;
    un_flash_c._trigger._eraseVerift = 1;
    st_flash_s.eraseVerift = TEMCU_FLASH_STATUS_DEFAULT;
    return TEMCU_FLASH_STATUS_OK;
}

static BOOL_Type checkAddress(TEFLASH_Addr addr)
{
    if(addr < EFLASH_BASE_ADDRESS || addr >= EFLASH_END_ADDRESS)
    {
        return FALSE;
    }
    return TRUE;
}

static void unlockThread(void)
{
    
    switch(un_flash_c._step._unlock)
    {
        case TEFLASH_CTRL_DEFAULT:
            if(0 != un_flash_c._unlock._trigger)
            {
                un_flash_c._step._unlock = TEFLASH_CTRL_TRIGGER;
            }
            break;
        case TEFLASH_CTRL_TRIGGER:
            if(1 == EFLASH_GetCtrlLockBitReg())
            {
                EFLASH_SetKeyReg(TEMCU_FLASH_UNLOCK_KEY1);
                EFLASH_SetKeyReg(TEMCU_FLASH_UNLOCK_KEY2);
            }
            else
            {
                un_flash_c._step._unlock = TEFLASH_CTRL_EXECUTE;
            }
            break;
        case TEFLASH_CTRL_EXECUTE:
            un_flash_c._status._unlock = TEFLASH_CTRL_UNLOCK;
            un_flash_c._step._unlock = TEFLASH_CTRL_OVER;
            break;
        case TEFLASH_CTRL_OVER:
            if(0 ==  un_flash_c._unlock._trigger)
            {
                EFLASH_LockCtrlReg();
                un_flash_c._status._unlock = TEFLASH_CTRL_LOCK;
                un_flash_c._step._unlock = TEFLASH_CTRL_DEFAULT;
            }
            break;
        default:
            break;
    }
}

static void programThread(void)
{
    if(TEFLASH_CTRL_LOCK == un_flash_c._status._unlock || 1 == EFLASH_GetAnyBusySTReg())
    {
        return;
    }
    switch(un_flash_c._step._program)
    {
        case TEFLASH_CTRL_DEFAULT:
        if(TEFLASH_CTRL_FREE == un_flash_c._status._busy && 0 != un_flash_c._trigger._program)
        {
            un_flash_c._step._program = TEFLASH_CTRL_TRIGGER;
        }
            break;
        case TEFLASH_CTRL_TRIGGER:
            if(TRUE == checkAddress(st_flash_d._addr_program))
            {
                EFLASH_SetStartAddressReg(st_flash_d._addr_program);
                EFLASH_SetCtrlReg1((1<< EFLASH_CTRL0_PROG_LENGTH_Pos) | (uint32_t)EFLASH_CMD_PAGEPROGRAM);
                EFLASH_TrigCtrlCmdReg();
                *(uint32_t *)(st_flash_d._addr_program) = st_flash_d._data;
                un_flash_c._step._program = TEFLASH_CTRL_EXECUTE;
            }
            else
            {
                /*  */
                st_flash_s.program = TEMCU_FLASH_STATUS_ADDRESS_ERROR;
                un_flash_c._step._program = TEFLASH_CTRL_OVER;
            }
            break;
        case TEFLASH_CTRL_EXECUTE:
            if(1 == EFLASH_GetEopReg())
            {
                if (EFLASH_GetStatusReg() & EFLASH_SR0_PPADRER_Msk)
                {
                    /*  */
                    st_flash_s.program = TEMCU_FLASH_STATUS_ERROR;
                }
                else
                {
                    /*  */
                    st_flash_s.program = TEMCU_FLASH_STATUS_OK;
                }
                un_flash_c._step._program = TEFLASH_CTRL_OVER;
            }
            break;
        case TEFLASH_CTRL_OVER:
            EFLASH_SetCtrlReg1(0x0);
            un_flash_c._trigger._program = 0;
            FlashProgram_CallBack(st_flash_d._addr_program,st_flash_s.program);
            un_flash_c._step._program = TEFLASH_CTRL_DEFAULT;
            break;
        default:
            break;
    }
}

static void eraseThread(void)
{
    if(TEFLASH_CTRL_LOCK == un_flash_c._status._unlock || 1 == EFLASH_GetAnyBusySTReg())
    {
        return;
    }
    switch(un_flash_c._step._erase)
    {
        case TEFLASH_CTRL_DEFAULT:
            if(TEFLASH_CTRL_FREE == un_flash_c._status._busy && 0 != un_flash_c._trigger._erase)
            {
                un_flash_c._step._erase = TEFLASH_CTRL_TRIGGER;
            }
            break;
        case TEFLASH_CTRL_TRIGGER:
            if(TRUE == checkAddress(st_flash_d._addr_erase))
            {
                EFLASH_SetStartAddressReg(st_flash_d._addr_erase);
                EFLASH_SetCtrlReg1((uint32_t)EFLASH_CMD_PAGERASE);
                EFLASH_TrigCtrlCmdReg();
                un_flash_c._step._erase = TEFLASH_CTRL_EXECUTE;
            }
            else
            {
                st_flash_s.erase = TEMCU_FLASH_STATUS_ADDRESS_ERROR;
                un_flash_c._step._erase = TEFLASH_CTRL_OVER;
            }
            break;
        case TEFLASH_CTRL_EXECUTE:
            if(1 == EFLASH_GetEopReg())
            {
                if (EFLASH_GetStatusReg() & EFLASH_SR0_ERAER_Msk)
                {
                    /*  */
                    st_flash_s.erase = TEMCU_FLASH_STATUS_ERROR;
                }
                else
                {
                    /*  */
                    st_flash_s.erase = TEMCU_FLASH_STATUS_OK; 
                }
                un_flash_c._step._erase = TEFLASH_CTRL_OVER;
            }
            break;
        case TEFLASH_CTRL_OVER:
            EFLASH_SetCtrlReg1(0x0);
            un_flash_c._trigger._erase = 0;
            FlashErase_CallBack(st_flash_d._addr_erase,st_flash_s.erase);
            un_flash_c._step._erase = TEFLASH_CTRL_DEFAULT;
            break;
        default:
            break;
    }
}

static void eraseVerifyThread(void)
{
    if(TEFLASH_CTRL_LOCK == un_flash_c._status._unlock || 1 == EFLASH_GetAnyBusySTReg())
    {
        return;
    }
    switch(un_flash_c._step._eraseVerift)
    {
        case TEFLASH_CTRL_DEFAULT:
            if(TEFLASH_CTRL_FREE == un_flash_c._status._busy && 0 != un_flash_c._trigger._eraseVerift)
            {
                un_flash_c._step._eraseVerift = TEFLASH_CTRL_TRIGGER;
            }
            break;
        case TEFLASH_CTRL_TRIGGER:
            if(TRUE == checkAddress(st_flash_d._addr_eraseVerift))
            {
                EFLASH_SetStartAddressReg(st_flash_d._addr_eraseVerift);
                EFLASH_SetCtrlReg1((uint32_t)EFLASH_CMD_PAGERASEVERIFY);
                EFLASH_TrigCtrlCmdReg();
                un_flash_c._step._eraseVerift = TEFLASH_CTRL_EXECUTE;
            }
            else
            {
                st_flash_s.eraseVerift = TEMCU_FLASH_STATUS_ADDRESS_ERROR;
                un_flash_c._step._eraseVerift = TEFLASH_CTRL_OVER;
            }
            break;
        case TEFLASH_CTRL_EXECUTE:
            if(1 == EFLASH_GetEopReg())
            {
                if(EFLASH_GetStatusReg() & EFLASH_SR0_VRER_Msk)
                {
                    /*  */
                    st_flash_s.eraseVerift = TEMCU_FLASH_STATUS_ERROR;
                }
                else
                {
                    /*  */
                    st_flash_s.eraseVerift = TEMCU_FLASH_STATUS_OK;
                }
                un_flash_c._step._eraseVerift = TEFLASH_CTRL_OVER;
            }
            break;
        case TEFLASH_CTRL_OVER:
            EFLASH_SetCtrlReg1(0x0);
            un_flash_c._trigger._eraseVerift = 0;
            FlashEraseVerift_CallBack(st_flash_d._addr_eraseVerift,st_flash_s.eraseVerift);
            un_flash_c._step._eraseVerift = TEFLASH_CTRL_DEFAULT;
            break;
        default:
            break;
    }
}

void TEMCU_Flash_ProgramCallBack_Hook(void (*func)(TEFLASH_Addr addr,TEMCU_FLASH_StatusType res))
{
    FlashProgram_CallBack = func;
}

void TEMCU_Flash_EraseCallBack_Hook(void (*func)(TEFLASH_Addr addr,TEMCU_FLASH_StatusType res))
{
    FlashErase_CallBack = func;
}

void TEMCU_Flash_EraseVeriftCallBack_Hook(void (*func)(TEFLASH_Addr addr,TEMCU_FLASH_StatusType res))
{
    FlashEraseVerift_CallBack = func;
}

void TEMCU_Flash_Init(void)
{
    un_flash_c._hw = 0;
    st_flash_s.erase = TEMCU_FLASH_STATUS_DEFAULT;
    st_flash_s.eraseVerift = TEMCU_FLASH_STATUS_DEFAULT;
    st_flash_s.program = TEMCU_FLASH_STATUS_DEFAULT;
}

void TEMainThread_Flash(void)
{
    if(1 == EFLASH_GetPVDWarningReg())
    {
        return;
    }
    unlockThread();
    eraseThread();
    eraseVerifyThread();
    programThread();
}






