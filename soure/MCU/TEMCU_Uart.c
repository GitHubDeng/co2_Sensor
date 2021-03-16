#include "TEMCU_Uart.h"
#include "TEMacro.h"
#include "ac780x_uart_reg.h"

#ifdef TE_MODULAR_UART0
static uint8_t *uart0TxDataPoint;
static uint16_t uart0TxDataLen;
static uint16_t uart0TxDataIndex;
static void (*Uart0Rev_CallBack)(uint8_t *data,uint16_t len);
static void Uart0_Task(void *device, uint32_t wpara, uint32_t lpara)
{
    
    UART_Type *uart_Device = (UART_Type *)device;
    uint8_t data;
    /* RX */
    if ((uart_Device->IER & UART_IER_ERXNE_Msk) && (wpara & UART_LSR0_DR_Msk))
    {
        data = uart_Device->RBR;
        Uart0Rev_CallBack(&data,1);
    }
    /* TX */
    if ((uart_Device->IER & UART_IER_ETXE_Msk) && (wpara & UART_LSR0_THRE_Msk))
    {
        uart_Device->RBR = uart0TxDataPoint[uart0TxDataIndex++];
        if (uart0TxDataIndex >= uart0TxDataLen )
        {
            UART_SetTXEInterrupt(device, DISABLE);
        }
    }
}
#endif

#ifdef TE_MODULAR_UART1
static uint8_t *uart1TxDataPoint;
static uint16_t uart1TxDataLen;
static uint16_t uart1TxDataIndex;
static void (*Uart1Rev_CallBack)(uint8_t *data,uint16_t len);
static void Uart1_Task(void *device, uint32_t wpara, uint32_t lpara)
{
    
    UART_Type *uart_Device = (UART_Type *)device;
    uint8_t data;
    /* RX */
    if ((uart_Device->IER & UART_IER_ERXNE_Msk) && (wpara & UART_LSR0_DR_Msk))
    {
        data = uart_Device->RBR;
        Uart1Rev_CallBack(&data,1);
    }
    /* TX */
    if ((uart_Device->IER & UART_IER_ETXE_Msk) && (wpara & UART_LSR0_THRE_Msk))
    {
        uart_Device->RBR = uart1TxDataPoint[uart1TxDataIndex++];
        if (uart1TxDataIndex >= uart1TxDataLen )
        {
            UART_SetTXEInterrupt(device, DISABLE);
        }
    }
}
#endif

#ifdef TE_MODULAR_UART2
static uint8_t *uart2TxDataPoint;
static uint16_t uart2TxDataLen;
static uint16_t uart2TxDataIndex;
static void (*Uart2Rev_CallBack)(uint8_t *data,uint16_t len);
static void Uart2_Task(void *device, uint32_t wpara, uint32_t lpara)
{
    
    UART_Type *uart_Device = (UART_Type *)device;
    uint8_t data;
    /* RX */
    if ((uart_Device->IER & UART_IER_ERXNE_Msk) && (wpara & UART_LSR0_DR_Msk))
    {
        data = uart_Device->RBR;
        Uart2Rev_CallBack(&data,1);
    }
    /* TX */
    if ((uart_Device->IER & UART_IER_ETXE_Msk) && (wpara & UART_LSR0_THRE_Msk))
    {
        uart_Device->RBR = uart2TxDataPoint[uart2TxDataIndex++];
        if (uart2TxDataIndex >= uart2TxDataLen )
        {
            UART_SetTXEInterrupt(device, DISABLE);
        }
    }
}
#endif

void TEMCU_Uart_RevStart(TEMCU_Uart_Type uart)
{
    switch(uart)
    {
#ifdef  TE_MODULAR_UART0
        case TEUart_0:
        UART_SetRXNEInterrupt(UART0,ENABLE);
        break;
#endif
#ifdef  TE_MODULAR_UART1
        case TEUart_1:
        UART_SetRXNEInterrupt(UART1,ENABLE);
        break;
#endif
#ifdef  TE_MODULAR_UART2
        case TEUart_2:
        UART_SetRXNEInterrupt(UART2,ENABLE);
        break;
#endif
        default:
        break;       
    }
}

void TEMCU_Uart_RevStop(TEMCU_Uart_Type uart)
{
    switch(uart)
    {
#ifdef  TE_MODULAR_UART0
        case TEUart_0:
        UART_SetRXNEInterrupt(UART0,DISABLE);
        break;
#endif
#ifdef  TE_MODULAR_UART1
        case TEUart_1:
        UART_SetRXNEInterrupt(UART1,DISABLE);
        break;
#endif
#ifdef  TE_MODULAR_UART2
        case TEUart_2:
        UART_SetRXNEInterrupt(UART2,DISABLE);
        break;
#endif
        default:
        break;       
    }
}

int TEMCU_Uart_Send(TEMCU_Uart_Type uart,uint8_t *data,uint16_t len)
{
    switch(uart)
    {
#ifdef  TE_MODULAR_UART0
        case TEUart_0:
        if(uart0TxDataIndex < uart0TxDataLen)
        {
            return -1;  
        }
        uart0TxDataPoint = data;
        uart0TxDataLen = len;
        uart0TxDataIndex = 0;
        UART_SetTXEInterrupt(UART0, ENABLE);
        return 0;
#endif
#ifdef  TE_MODULAR_UART1
        case TEUart_1:
        if(uart1TxDataIndex < uart1TxDataLen)
        {
            return -1;  
        }
        uart1TxDataPoint = data;
        uart1TxDataLen = len;
        uart1TxDataIndex = 0;
        UART_SetTXEInterrupt(UART1, ENABLE);
        return 0;
#endif
#ifdef  TE_MODULAR_UART2
        case TEUart_2:
        if(uart2TxDataIndex < uart2TxDataLen)
        {
            return -1;  
        }
        uart2TxDataPoint = data;
        uart2TxDataLen = len;
        uart2TxDataIndex = 0;
        UART_SetTXEInterrupt(UART2, ENABLE);
        return 0;
#endif
        default:
        return -1;        
    }
}

void TEMCU_Uart_RevCallBack_Hook(TEMCU_Uart_Type uart,void (*UartRev_CallBack)(uint8_t *data,uint16_t len))
{
    switch(uart)
    {
#ifdef  TE_MODULAR_UART0
        case TEUart_0:
        Uart0Rev_CallBack = UartRev_CallBack;
        break;
#endif
#ifdef  TE_MODULAR_UART1
        case TEUart_1:
        Uart1Rev_CallBack = UartRev_CallBack;
        break;
#endif
#ifdef  TE_MODULAR_UART2
        case TEUart_2:
        Uart2Rev_CallBack = UartRev_CallBack;
        break;
#endif
        default:
        break;
    }
}

void TEMCU_Uart_Init(void)
{
#if defined TE_MODULAR_UART0 || defined TE_MODULAR_UART1 || defined TE_MODULAR_UART2 
    UART_ConfigType  uartConfig = {0};
/* uart0初始化 */
#ifdef  TE_MODULAR_UART0
    uartConfig.baudrate = 115200;
    uartConfig.dataBits = UART_WORD_LEN_8BIT;
    uartConfig.stopBits = UART_STOP_1BIT;
    uartConfig.parity = UART_PARI_NO;
    uartConfig.fifoByteEn = ENABLE;
    uartConfig.dmaEn = UART_DMA_TXRX_EN;
    uartConfig.callBack = Uart0_Task;
    UART_Init(UART0, &uartConfig);
#endif

/* uart1初始化 */
#ifdef  TE_MODULAR_UART1
    uartConfig.baudrate = 115200;
    uartConfig.dataBits = UART_WORD_LEN_8BIT;
    uartConfig.stopBits = UART_STOP_1BIT;
    uartConfig.parity = UART_PARI_NO;
    uartConfig.fifoByteEn = ENABLE;
    uartConfig.dmaEn = UART_DMA_TXRX_EN;
    uartConfig.callBack = Uart1_Task;
    UART_Init(UART1, &uartConfig);
#endif

/* uart2初始化 */
#ifdef  TE_MODULAR_UART2
    uartConfig.baudrate = 115200;               //波特率
    uartConfig.dataBits = UART_WORD_LEN_8BIT;   //数据位
    uartConfig.stopBits = UART_STOP_1BIT;       //停止位
    uartConfig.parity = UART_PARI_NO;           //校验位
    uartConfig.fifoByteEn = ENABLE;             //fifo使能 （使用DMA时必须使能）
    uartConfig.dmaEn = UART_DMA_TXRX_NONE;      //DMA使能
    uartConfig.callBack = Uart2_Task;           //回调
    UART_Init(UART2, &uartConfig);
#endif
#endif
}

