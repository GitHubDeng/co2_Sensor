#include "TESW_EEPROM.h"
#include "ac780x_eflash.h" 
#include "TEMCU_Flash.h"


#define SWEEPROM_PAGE_INDEX_BLOCK1          61
#define SWEEPROM_PAGE_INDEX_BLOCK2          62
#define SWEEPROM_PAGE_BLOCK_NUM             1

#define SWEEPROM_ADDRESS_BLOCK1_START       TEMCU_FLASH_ADDRESS_PAGE(SWEEPROM_PAGE_INDEX_BLOCK1)
#define SWEEPROM_ADDRESS_BLOCK2_START       TEMCU_FLASH_ADDRESS_PAGE(SWEEPROM_PAGE_INDEX_BLOCK2)

#define SWEEPROM_SIZE_BLOCK         SWEEPROM_PAGE_BLOCK_NUM*TEMCU_FLASH_PAGE_SIZE
#define SWEEPROM_SIZE_DATA          4

#define SWEEPROM_ADDRESS_BLOCK1_END         SWEEPROM_ADDRESS_BLOCK1_START+SWEEPROM_SIZE_BLOCK
#define SWEEPROM_ADDRESS_BLOCK2_END         SWEEPROM_ADDRESS_BLOCK2_START+SWEEPROM_SIZE_BLOCK

#define TE_BLOCK_DEFAULT    0
#define TE_BLOCK_EMPTY      1
#define TE_BLOCK_NOEMPTY    2

typedef union 
{
    uint32_t _w;
    struct TESWEEPROM_DataType
    {
        TESWEEPROM_Addr _addr;
        uint16_t _data;
    }type;
}TE_SWEEPROM_Data;

typedef struct 
{
    union TE_SWEEPROM_Ctrl
    {
        uint8_t _b;
        struct TE_SWEEPROM_CtrlType
        {
                uint8_t         : 5;
                uint8_t _write  : 1;
                uint8_t _erase  : 1;
                uint8_t _check  : 1;
        }_type;  
    }_flashctrl;
    union TE_SWEEPROM_Status
    {
        uint8_t _b;
        struct TE_SWEEPROM_StatusType
        {
            uint8_t          : 5;
            uint8_t _unlock  : 1;
            uint8_t _empty   : 1;
            uint8_t _full    : 1; 
        }_type;
    }_status; 
    const TEFLASH_Addr baseAddr;
    const TEFLASH_Addr statrtAddr;
    const TEFLASH_Addr endAddr;
    const uint16_t pageNum;
    TEFLASH_Addr insertAddr;
}TE_SWEEPROM_Block;

TE_SWEEPROM_Block block1 = {0,0,SWEEPROM_ADDRESS_BLOCK1_START,SWEEPROM_ADDRESS_BLOCK1_START+SWEEPROM_SIZE_DATA,SWEEPROM_ADDRESS_BLOCK1_END,SWEEPROM_PAGE_BLOCK_NUM,0};
TE_SWEEPROM_Block block2 = {0,0,SWEEPROM_ADDRESS_BLOCK2_START,SWEEPROM_ADDRESS_BLOCK2_START+SWEEPROM_SIZE_DATA,SWEEPROM_ADDRESS_BLOCK2_END,SWEEPROM_PAGE_BLOCK_NUM,0};

struct TESWEEPROM_StatusType
{
    TE_SWEEPROM_Block *program;
    TE_SWEEPROM_Block *backup;
}st_eeprom_s = {0};

struct TESWEEPROM_BlockCopyStatusType
{
    uint8_t lock;
    uint16_t _step;
}st_blockCopy_s = {0};

struct TESWEEPROM_BlockCheckStatusType
{
    uint8_t lock;
    uint16_t _step;
}st_blockCheck_s = {0};

static void checkThread(void)
{
    uint8_t temp_u8 = 0;
    TEFLASH_Addr temp_addr = 0;
    switch (st_blockCheck_s._step)
    {
        case 0:
            if(1 == st_blockCheck_s.lock)
            {
                st_blockCheck_s._step = 1;
            }
            break;
        case 1:/* 验证block1 */
            if(TEMCU_FLASH_STATUS_OK == TEMCU_Flash_EraseVerify(block1.baseAddr))
            {
                st_blockCheck_s._step = 0x20;
            }
            break;
        case 2:/*  验证block2 */
            if(TEMCU_FLASH_STATUS_OK == TEMCU_Flash_EraseVerify(block2.baseAddr))
            {
                st_blockCheck_s._step = 0x20;
            }
            break;
        case 0x20:
            if(0 == block1._flashctrl._type._check && 0 == block1._status._type._unlock)
            {
                st_blockCheck_s._step = 1;
                break;
            }
            if(0 == block2._flashctrl._type._check && 0 == block2._status._type._unlock)
            {
                st_blockCheck_s._step = 2;
                break;
            }
            if(1 == block1._status._type._unlock && 1 == block2._status._type._unlock)
            {
                st_blockCheck_s._step = 3;
            }
            break;
        case 3:
        /* 
            判断block状态
              b7       b6       b5       b4      b3       b2      b1      b0
               0       0     b1_empty b2_empty   0        0     b1_pro  b2_pro
        
         */
            temp_u8 = 0;
            temp_u8 |= block1._status._type._empty << 4;
            temp_u8 |= block2._status._type._empty << 5;
            if(0 == TEMCU_Flash_Read(block1.baseAddr))
            {
                temp_u8 |= 1 << 0;
            }
            if(0 == TEMCU_Flash_Read(block2.baseAddr))
            {
                temp_u8 |= 1 << 1;
            }
            switch(temp_u8 & 0x33)
            {
                case 0x30:/* flash全空 没有program */
                    TEMCU_Flash_Write(block1.baseAddr,0);
                    break;
                case 0x21:/* block1 program */
                    st_eeprom_s.program = &block1;
                    st_eeprom_s.backup = &block2;
                    st_blockCheck_s._step = 6;
                    break;
                case 0x12:/* block2 program */
                    st_eeprom_s.program = &block2;
                    st_eeprom_s.backup = &block1;
                    st_blockCheck_s._step = 6;
                    break;
                case 0x01:
                case 0x10:/* block2异常 */
                    if(TEMCU_FLASH_STATUS_OK == TEMCU_Flash_Erase(block2.baseAddr))//擦除block2
                    {
                        st_blockCheck_s._step = 5;
                    }
                    break;
                case 0x02:
                case 0x20:/* block1异常 */ 
                    if(TEMCU_FLASH_STATUS_OK == TEMCU_Flash_Erase(block1.baseAddr))//擦除block1
                    {
                        st_blockCheck_s._step = 4;
                    }
                    break;
                default:/* 两个block都异常 */
                    if(TEMCU_FLASH_STATUS_OK == TEMCU_Flash_Erase(block1.baseAddr))//擦除block1
                    {
                        st_blockCheck_s._step = 4;
                    }
                    break;
            }
            break;
        case 4:/* 擦block1 */
            if(0 == block1._flashctrl._type._erase)
            {
                st_blockCheck_s._step = 1;
            }
            break;
        case 5:/* 擦block2 */
            if(0 == block2._flashctrl._type._erase)
            {
                st_blockCheck_s._step = 2;
            }
            break;
        case 6:/* 查insert */
            for(temp_addr = st_eeprom_s.program->statrtAddr;temp_addr < st_eeprom_s.program->endAddr;temp_addr += SWEEPROM_SIZE_DATA)
            {
                if(0xFFFFFFFF == TEMCU_Flash_Read(temp_addr))
                {
                    st_eeprom_s.program->insertAddr = temp_addr;
                    break;
                }
            }
            if(temp_addr == st_eeprom_s.program->endAddr)
            {
                st_eeprom_s.program->insertAddr=st_eeprom_s.program->endAddr-SWEEPROM_SIZE_DATA;
                st_eeprom_s.program->_status._type._full = 1;
            }
            st_eeprom_s.backup->insertAddr = st_eeprom_s.backup->statrtAddr;
            st_blockCheck_s._step = 7;
            break;
        case 7:/* 结束 */
            st_blockCheck_s.lock = 0;
            st_blockCheck_s._step = 0;
            break;
        default:
            break;
    }
}

static void copyThread(void)
{
    static TE_SWEEPROM_Data read_p = {0};
    TE_SWEEPROM_Data read_b = {0};
    // TEFLASH_Addr count_p = 0;
    TEFLASH_Addr count_b = 0;
    TE_SWEEPROM_Block *tempBlock;
    switch(st_blockCopy_s._step)
    {
        case 0:/* 如果编程区已满，开始数据迁移 */
            if(1 == st_eeprom_s.program->_status._type._full)
            {
                st_blockCopy_s.lock = 1;
                st_blockCopy_s._step = 1;
            }
            break;
        case 1:/* 检查备份区是否为空 */
            if(1 == st_eeprom_s.backup->_status._type._unlock)/* 备份区已解锁 */
            {
                if(1 == st_eeprom_s.backup->_status._type._empty)/* 备份区为空 */
                {
                    st_blockCopy_s._step = 2;
                }
                else/* 备份区不为空 */
                {
                    /* 擦除备份区 */
                    if(TEMCU_FLASH_STATUS_OK == TEMCU_Flash_Erase(st_eeprom_s.backup->baseAddr))
                    {
                        st_eeprom_s.backup->_flashctrl._type._erase = 1;
                    }
                }
            }
            else if(0 == st_eeprom_s.backup->_flashctrl._type._check)
            {
                if(TEMCU_FLASH_STATUS_OK == TEMCU_Flash_EraseVerify(st_eeprom_s.backup->baseAddr))
                {
                    st_eeprom_s.backup->_flashctrl._type._check = 1;
                }
            }
            break;
        case 2:/* 检查eeprom数据是否已迁移 */
            for(st_eeprom_s.program->insertAddr = st_eeprom_s.program->endAddr-SWEEPROM_SIZE_DATA;
            st_eeprom_s.program->insertAddr >= st_eeprom_s.program->statrtAddr;
            st_eeprom_s.program->insertAddr -= SWEEPROM_SIZE_DATA)/* 扫描编程区 */
            {  
                read_p._w = TEMCU_Flash_Read(st_eeprom_s.program->insertAddr);  //读编程区数据
                /* 扫描备份区 */
                for(count_b = st_eeprom_s.backup->statrtAddr;count_b < st_eeprom_s.backup->insertAddr ; count_b += SWEEPROM_SIZE_DATA)
                {
                    read_b._w = TEMCU_Flash_Read(count_b);      //读备份区数据
                    if(read_b.type._addr == read_p.type._addr)//重复
                    {
                        break;
                    }
                }
                if(count_b == st_eeprom_s.backup->insertAddr)//没有重复
                {
                    st_blockCopy_s._step = 3;               //准备写
                    break;
                } 
            }
            if(st_eeprom_s.program->insertAddr < st_eeprom_s.program->statrtAddr)
            {
                /* 数据已经全部迁移至备份区 */
                st_blockCopy_s._step = 5;
            }
            break;
        case 3:/* 将数据从编程区拷贝至备份区 */
            if(TEMCU_FLASH_STATUS_OK == TEMCU_Flash_Write(st_eeprom_s.backup->insertAddr,read_p._w))
            {
                st_eeprom_s.backup->_flashctrl._type._write = 1;
                st_blockCopy_s._step = 4;                   //开始写
            }
            break;
        case 4:/* 检测是否写入完成 */
            if(0 == st_eeprom_s.backup->_flashctrl._type._write)
            {
                st_blockCopy_s._step = 2; 
            }
            break;
        case 5:/* 数据迁移完成，擦除编程区 */
            if(TEMCU_FLASH_STATUS_OK == TEMCU_Flash_Erase(st_eeprom_s.program->baseAddr))
            {
                st_eeprom_s.program->_flashctrl._type._erase = 1;
                st_blockCopy_s._step = 6; 
            }           
            break;
        case 6:/* 编程区和备份区互换位置 */
            if(0 == st_eeprom_s.program->_flashctrl._type._erase)/* 擦除完成 */
            {
                st_eeprom_s.program->_status._type._full = 0;
                /* 交换编程区和备份区  */
                tempBlock = st_eeprom_s.program;
                st_eeprom_s.program = st_eeprom_s.backup;
                st_eeprom_s.backup = tempBlock;
                /* 写编程区标识位（标识位位于每个block第一个字节，标识此区域为编程区，值为0） */
                if(TEMCU_FLASH_STATUS_OK == TEMCU_Flash_Write(st_eeprom_s.program->baseAddr,0))
                {
                    st_eeprom_s.program->_flashctrl._type._write = 1;
                    st_blockCopy_s._step = 7;                  
                }
            }
            break;
        case 7:
            if(0 == st_eeprom_s.program->_flashctrl._type._write)
            {
                st_blockCopy_s.lock = 0;
                st_blockCopy_s._step = 0; 
            }
            break;
        default:
            /* 需要reset */


            /* ******** */
            break;
    }
}

static void flashWriteEnd(TEFLASH_Addr addr,TEMCU_FLASH_StatusType res)
{
    if(0 == st_blockCopy_s.lock)
    {
        if(TEMCU_FLASH_STATUS_OK == res)
        {
            st_eeprom_s.program->insertAddr = addr + SWEEPROM_SIZE_DATA;
            if(st_eeprom_s.program->insertAddr >= st_eeprom_s.program->endAddr)
            {
                st_eeprom_s.program->_status._type._full = 1;
            }
        }
    }
    else
    {
        if(TEMCU_FLASH_STATUS_OK == res)
        {
            st_eeprom_s.backup->insertAddr = addr + SWEEPROM_SIZE_DATA;
            st_eeprom_s.program->insertAddr -= SWEEPROM_SIZE_DATA;
        } 
    }
    st_eeprom_s.backup->_flashctrl._type._write = 0;
}

static void flashEraseEnd(TEFLASH_Addr addr,TEMCU_FLASH_StatusType res)
{
    if(block1.baseAddr == addr)
    {
        if(TEMCU_FLASH_STATUS_OK == res)
        {
            block1._flashctrl._type._erase = 0;
        }
    }
    else if(block2.baseAddr == addr)
    {
        if(TEMCU_FLASH_STATUS_OK == res)
        {
            block2._flashctrl._type._erase = 0;
        }
    }
}

static void flashcheckEnd(TEFLASH_Addr addr,TEMCU_FLASH_StatusType res)
{
    if(block1.baseAddr == addr)
    {
        if(TEMCU_FLASH_STATUS_OK == res)
        {
            block1._status._type._empty = 1;
            block1._status._type._unlock = 1;
        }
        else if(TEMCU_FLASH_STATUS_ERROR == res)
        {
            block1._status._type._empty = 0;
            block1._status._type._unlock = 1;
        }
        else
        { 
            block1._status._type._unlock = 0;
        }
        block1._flashctrl._type._check = 0;
    }
    else if(block2.baseAddr == addr)
    {
        if(TEMCU_FLASH_STATUS_OK == res)
        {
            block2._status._type._empty = 1;
            block2._status._type._unlock = 1;
        }
        else if(TEMCU_FLASH_STATUS_ERROR == res)
        {
            block2._status._type._empty = 0; 
            block2._status._type._unlock = 1;          
        }
        else
        {
            block2._status._type._unlock = 0;
        }
        block2._flashctrl._type._check = 0;
    }
}

TESW_EEPROM_StatusType TESW_EEPROM_Read(TESWEEPROM_Addr addr,uint16_t *data)
{  
    uint32_t temp_addr;
    if(1 == st_blockCopy_s.lock || 1 == st_blockCheck_s.lock)
    {
        return TESWEEPROM_STATUS_BUSY;
    }
    if(NULL == st_eeprom_s.program || 0 == st_eeprom_s.program->_status._type._unlock)
    {
        return TESWEEPROM_STATUS_ERROR;
    }
    for(temp_addr = st_eeprom_s.program->insertAddr-SWEEPROM_SIZE_DATA;
    temp_addr >= st_eeprom_s.program->statrtAddr;
    temp_addr -= SWEEPROM_SIZE_DATA)
    {
        TE_SWEEPROM_Data temp_data;
        temp_data._w = TEMCU_Flash_Read(temp_addr);
        if(addr == temp_data.type._addr)
        {
            *data = temp_data.type._data;
            return TESWEEPROM_STATUS_OK;
        }
    }
    return TESWEEPROM_STATUS_READ_EMPTY;
}

TESW_EEPROM_StatusType TESW_EEPROM_Write(TESWEEPROM_Addr addr,uint16_t data)
{
    TE_SWEEPROM_Data temp;
    temp.type._addr = addr;
    temp.type._data = data;
    if(1 == st_blockCopy_s.lock || 1 == st_blockCheck_s.lock)
    {
        return TESWEEPROM_STATUS_BUSY;
    }
    if(NULL == st_eeprom_s.program || 0 == st_eeprom_s.program->_status._type._unlock)
    {
        return TESWEEPROM_STATUS_ERROR;
    }
    if(TEMCU_FLASH_STATUS_OK == TEMCU_Flash_Write(st_eeprom_s.program->insertAddr,temp._w))
    {
        return TESWEEPROM_STATUS_OK;
    }
    return TESWEEPROM_STATUS_ERROR;    
}

void TESW_EEPROM_EraseBlock1(void)
{
    if(TEMCU_FLASH_STATUS_OK == TEMCU_Flash_Erase(block1.baseAddr))
    {
        block1._flashctrl._type._erase = 1;
    }
}

void TESW_EEPROM_EraseBlock2(void)
{
    if(TEMCU_FLASH_STATUS_OK == TEMCU_Flash_Erase(block2.baseAddr))
    {
        block2._flashctrl._type._erase = 1;
    }
}

void TESW_EEPROM_Init(void)
{
    TEMCU_Flash_Init();
    TEMCU_Flash_ProgramCallBack_Hook(flashWriteEnd);
    TEMCU_Flash_EraseCallBack_Hook(flashEraseEnd);
    TEMCU_Flash_EraseVeriftCallBack_Hook(flashcheckEnd);
    st_blockCheck_s.lock = 1;
}

void TEMainThread_SWEEPRom(void)
{
    checkThread();
    copyThread();
    TEMainThread_Flash();
}
