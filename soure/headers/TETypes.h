#ifndef TETYPES_H
#define TETYPES_H

#include <stdint.h>
#include <stdio.h>

typedef union
{
    uint32_t _w;
    struct HwType
    {
        uint16_t _l;
        uint16_t _h;
    }_hw;
}TEWToHw;

typedef union
{
    uint32_t _w;
    struct WByteType
    {
        uint8_t _B3;
        uint8_t _B2;
        uint8_t _B1;
        uint8_t _B0;
    }_B;
}TEWToByte;

typedef union
{
    uint16_t _hw;
    struct HwByteType
    {
        uint8_t _l;
        uint8_t _h;
    }_B;
}TEHwToByte;

#endif
