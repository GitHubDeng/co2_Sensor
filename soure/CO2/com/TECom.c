#include "TECom.h"
#include "TEMCU_Uart.h"
#include "TEEvent.h"
#include "TECrc16_modbus.h"
#include "TECom_Cache.h"

#define TEComUart TEUart_2

#define TEComBuf_Len   12
union comBufTag
{
    uint8_t _b[TEComBuf_Len];
    struct BufType
    {
        uint16_t _dataLen;    
        uint16_t _crc;
        union BufDataType
        {
            struct Data_16b
            {
                uint16_t _na;
                uint16_t _hw;
            }_16b;
            TEWToHw _32b;
        }_data;
        uint16_t _addr;
        uint8_t _op;
        uint8_t _head;    
    }type;
    uint32_t dataType;
}comBuf = {0};

uint8_t sendBuf[TEComBuf_Len] = {0};

uint16_t uartStep = 0;

static void (*ComRev_CallBack)(uint8_t op,uint16_t addr);

void TECom_RevCallBack_Request_Hook(void (*func)(uint8_t op,uint16_t addr))
{
    ComRev_CallBack = func;
}


TEEvent_New(TERevFrame)
{
    uint16_t i =0;
    TEWToHw W2HW;
    TEWToByte W2B;
    TEHwToByte HW2B;

    uartStep = 0;
    /* 
        因为是小端存储，comBuf中的数据是逆序存放，在计算crc和发送应答时需要顺序存放数据，
        所以在计算crc和发送应答时需要倒转存放顺序
     */
    for(i = 0;i < 8;i++)
    {
        sendBuf[i] = comBuf._b[TEComBuf_Len-1-i];
    }
    /* 校验 */
    if(comBuf.type._crc != getCrc16_Modbus(sendBuf,4+comBuf.type._dataLen))//校验未通过
    {
        return;
    }
    /* 存cache */
    if(0x06 == comBuf.type._op || 0x05 == comBuf.type._op)
    {
        if(2 == comBuf.type._dataLen)
        {
            TECom_SetDataWithAddr(comBuf.type._addr,comBuf.type._data._16b._hw);
        }
        else if(4 == comBuf.type._dataLen)
        {
            TECom_SetDataWithAddr(comBuf.type._addr,comBuf.type._data._32b._hw._h);
            TECom_SetDataWithAddr(comBuf.type._addr+1,comBuf.type._data._32b._hw._l);
        }
    }
    /* 读cache */
    else if(0x04 == comBuf.type._op || 0x03 == comBuf.type._op)
    {
        if(0x0FA2 == comBuf.type._addr || 0x0FD7 == comBuf.type._addr)
        {
            W2HW._hw._h = TECom_GetDataWithAddr(comBuf.type._addr);
            W2HW._hw._l = TECom_GetDataWithAddr(comBuf.type._addr+1);
            W2B._w = W2HW._w;
            sendBuf[3] = W2B._B._B0;
            sendBuf[4] = W2B._B._B1;
            sendBuf[5] = W2B._B._B2;
            sendBuf[6] = W2B._B._B3;
        }
        else
        {
            HW2B._hw = TECom_GetDataWithAddr(comBuf.type._addr);
            sendBuf[3] = HW2B._B._h;
            sendBuf[4] = HW2B._B._l;
        }
    }

    /* 同步/更新 协议栈（回调） */
    if(ComRev_CallBack != NULL)
    {
        ComRev_CallBack(comBuf.type._op,comBuf.type._addr);
    }

    /* 应答 */
    switch(comBuf.type._op)
    {
        case 0x05:
            TEMCU_Uart_Send(TEComUart,sendBuf,3);
            break;
        case 0x06:
            TEMCU_Uart_Send(TEComUart,sendBuf,4+comBuf.type._dataLen);
            break;
        case 0x03:
        case 0x04:

            sendBuf[2] = comBuf.type._dataLen;
            TEMCU_Uart_Send(TEComUart,sendBuf,3+comBuf.type._dataLen);
            break;
        default:
            break;
    }
}

void revData(uint8_t *data,uint16_t len)
{
    uint8_t temp_data = *data;
    switch(uartStep)
    {
        case 0:/* head */
        if(0x15 != temp_data)
        {
            return;
        }
        case 1:/* op */
        case 2:/* addr */
        case 3:/* addr */
        case 4:/* data0 */
        case 5:/* data1 */
        break;
        case 6:/* data2 特定的帧才会收到data2 data3 */
        if(!(0x06 == comBuf.type._op && (0x0fa2 == comBuf.type._addr || 0x0fd7 == comBuf.type._addr)))
        {
            uartStep += 2;
            comBuf.type._dataLen = 2;
        }
        else
        {
            comBuf.type._dataLen = 4;
        }
        case 7:/* data3 */
        case 8:/* CRC H */
            break;
        case 9:/* CRC L */ 
            TEEvent_Start(&TERevFrame,SINGLE_Event,0);
            break;
        default:
            uartStep = 0;
            break;
    }
    comBuf._b[TEComBuf_Len-1-uartStep] = temp_data;
    uartStep++;
}

void TECom_Init(void)
{
    TEMCU_Uart_Init();
    TEMCU_Uart_RevCallBack_Hook(TEComUart,revData);
    TEMCU_Uart_RevStart(TEComUart);
}
