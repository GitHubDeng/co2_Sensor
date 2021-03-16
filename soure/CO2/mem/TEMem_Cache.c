#include "TEMem_Cache.h"

typedef struct
{
    const uint16_t _addr;
    uint16_t _data;
}TEMem_NVMTabUnit;

TEMem_NVMTabUnit TEMem_NVMTab[TEMEM_TAB_NUM] = 
{
    {0x0001,0},     //0 设备编码H
    {0x0002,0},     //1 设备编码L
    {0x0003,0},     //2 工作模式
    {0x0004,0},     //3 A模式采样率
    {0x0005,0},     //4 B模式采样率
    {0x0006,0},     //5 单点测试PPM值
    {0x0007,0},     //6 ABC校准采样率
    {0x0008,0},     //7 ABC定值计数
    {0x0009,0},     //8 ABC采样计数
    {0x0010,0},     //9 气压H
    {0x0011,0},     //10 气压L
    {0x0012,0},     //11 KPA阈值0
    {0x0013,0},     //12 KPA阈值1
    {0x0014,0},     //13 KPA阈值2
    {0x0015,0}      //14 KPA阈值3
};


uint16_t TEMem_GetData(TEMem_TabType type)
{
    return TEMem_NVMTab[type]._data;
}

void TEMem_SetData(TEMem_TabType type,uint16_t value)
{
    TEMem_NVMTab[type]._data = value;
}

TEMem_Addr TEMem_GetAddr(TEMem_TabType type)
{
    return TEMem_NVMTab[type]._addr;
}

void TEMem_SynData(TEMem_TabType type,uint16_t value)
{
    TEMem_NVMTab[type]._data = value;
}



