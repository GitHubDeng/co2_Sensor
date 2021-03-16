#include "TEMem_Cache.h"

typedef struct
{
    uint16_t _syn           : 1;
    uint16_t _update        : 1;
    const uint16_t _addr    : 14;
    uint16_t _data;
}TEMem_NVMTabUnit;

TEMem_NVMTabUnit TEMem_NVMTab[TEMEM_TAB_NUM] = 
{
    {._syn = 0,._update = 0,._addr = 0x0001,._data = 0},     //设备编码H
    {._syn = 0,._update = 0,._addr = 0x0002,._data = 0},     //设备编码L
    {._syn = 0,._update = 0,._addr = 0x0003,._data = 0},     //工作模式
    {._syn = 0,._update = 0,._addr = 0x0004,._data = 0},     //A模式采样率
    {._syn = 0,._update = 0,._addr = 0x0005,._data = 0},     //B模式采样率
    {._syn = 0,._update = 0,._addr = 0x0006,._data = 0},     //单点测试PPM值
    {._syn = 0,._update = 0,._addr = 0x0007,._data = 0},     //ABC校准采样率
    {._syn = 0,._update = 0,._addr = 0x0008,._data = 0},     //ABC定值计数
    {._syn = 0,._update = 0,._addr = 0x0009,._data = 0},     //ABC采样计数
    {._syn = 0,._update = 0,._addr = 0x0010,._data = 0},     //气压H
    {._syn = 0,._update = 0,._addr = 0x0011,._data = 0},     //气压L
    {._syn = 0,._update = 0,._addr = 0x0012,._data = 0},     //KPA阈值0
    {._syn = 0,._update = 0,._addr = 0x0013,._data = 0},     //KPA阈值1
    {._syn = 0,._update = 0,._addr = 0x0014,._data = 0},     //KPA阈值2
    {._syn = 0,._update = 0,._addr = 0x0015,._data = 0}      //KPA阈值3
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
    TEMem_NVMTab[type]._syn = 1;
}



