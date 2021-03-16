#include "TEMCU_DMA.h"  
#include "TEMacro.h"
#include "ac780x_dma.h"
#include <string.h>

#ifdef TE_DMA_CH0
static uint32_t *memStartAddr_ch0;
static uint32_t *memEndAddr_ch0;
static uint16_t memLen_ch0;
static void (*DMA_CH0_CallBack)(TEDMA_INT_Type intType);
#endif
#ifdef TE_DMA_CH1
static uint32_t *memStartAddr_ch1;
static uint32_t *memEndAddr_ch1;
static void (*DMA_CH1_CallBack)(TEDMA_INT_Type intType);
#endif
#ifdef TE_DMA_CH2
static uint32_t *memStartAddr_ch2;
static uint32_t *memEndAddr_ch2;
static void (*DMA_CH2_CallBack)(TEDMA_INT_Type intType);
#endif
#ifdef TE_DMA_CH3
static uint32_t *memStartAddr_ch3;
static uint32_t *memEndAddr_ch3;
static void (*DMA_CH3_CallBack)(TEDMA_INT_Type intType);
#endif

void TEDMA_CallBack_Hook(TEDMA_CH_Type ch,void (*CallBack)(TEDMA_INT_Type intType))
{
    switch (ch)
    {
#ifdef TE_DMA_CH0
    case DMA_CH0:
        DMA_CH0_CallBack = CallBack;
        break;
#endif  
#ifdef TE_DMA_CH1
    case DMA_CH1:
        DMA_CH1_CallBack = CallBack;
        break;
#endif 
#ifdef TE_DMA_CH2
    case DMA_CH2:
        DMA_CH2_CallBack = CallBack;
        break;
#endif 
#ifdef TE_DMA_CH3
    case DMA_CH3:
        DMA_CH3_CallBack = CallBack;
        break;
#endif  
    default:
        break;
    }
}

void TEDMA_MemBuf_Set(TEDMA_CH_Type ch,uint32_t *buf,uint16_t len)
{
    switch (ch)
    {
#ifdef TE_DMA_CH0
    case DMA_CH0:
        memStartAddr_ch0 = buf;
        memEndAddr_ch0 = &buf[len];
        memLen_ch0 = len;
        break;
#endif  
#ifdef TE_DMA_CH1
    case DMA_CH1:
        memStartAddr_ch1 = buf;
        memEndAddr_ch1 = &buf[len];
        break;
#endif 
#ifdef TE_DMA_CH2
    case DMA_CH2:
        memStartAddr_ch2 = buf;
        memEndAddr_ch2 = &buf[len];
        break;
#endif 
#ifdef TE_DMA_CH3
        memStartAddr_ch3 = buf;
        memEndAddr_ch3 = &buf[len];
        break;
#endif  
    default:
        break;
    }
}

#if defined TE_DMA_CH0 || defined TE_DMA_CH1 || defined TE_DMA_CH2 || defined TE_DMA_CH3
static void DMA_Task(void *device, uint32_t wpara, uint32_t lpara)
{
    /* 
        wpara:通道状态        
     */

    TEDMA_INT_Type intType;
    /* 传输错误 */
    if ((wpara & 0x04) == 0x4)
    {
        intType = DMA_INT_Error;
    }
    /* 传输完成 */
    else if ((wpara & 0x01) == 0x1)
    {
        intType = DMA_INT_Complete;
    }
    /* 传输一半 */
    else if ((wpara & 0x02) == 0x2)
    {
        intType = DMA_INT_Half;
    }
    /* 回调 */
    if(device == DMA0_CHANNEL0)
    {
        #ifdef TE_DMA_CH0
        DMA_CH0_CallBack(intType);
        #endif
    }
    else if(device == DMA0_CHANNEL1)
    {
        #ifdef TE_DMA_CH1
        DMA_CH1_CallBack(intType);
        #endif
    }
    else if(device == DMA0_CHANNEL2)
    {
        #ifdef TE_DMA_CH2
        DMA_CH2_CallBack(intType);
        #endif
    }
    else if(device == DMA0_CHANNEL3)
    {
        #ifdef TE_DMA_CH3
        DMA_CH3_CallBack(intType);
        #endif
    }
}
#endif

void TEMCU_DMA_Init(void)
{
#if defined TE_DMA_CH0 || defined TE_DMA_CH1 || defined TE_DMA_CH2 || defined TE_DMA_CH3
    DMA_ConfigType DMAConfig;
#endif

#ifdef TE_DMA_CH0
    memset(&DMAConfig, 0x00, sizeof(DMA_ConfigType));

    /* memory设置 */
    DMAConfig.memStartAddr = (uint32_t)memStartAddr_ch0;          //memory起始地址
    DMAConfig.memEndAddr = (uint32_t)memEndAddr_ch0;              //memor结束地址

    /* 外设数据寄存器地址 */
    DMAConfig.periphStartAddr = (uint32_t)(&(ADC0->RDR));

    /* 通道使能 */
    DMAConfig.channelEn = ENABLE;

    /* 中断 */
    DMAConfig.finishInterruptEn = ENABLE;                         //传输完成
    DMAConfig.halfFinishInterruptEn = DISABLE;                    //传输一半
    DMAConfig.errorInterruptEn = ENABLE;                          //传输错误

    /* 
        通道优先级 
            DMA_PRIORITY_LOW
            DMA_PRIORITY_MEDIUM
            DMA_PRIORITY_HIGH
            DMA_PRIORITY_VERY_HIGH
    */
    DMAConfig.channelPriority = DMA_PRIORITY_VERY_HIGH;

    /* 循环传输 */
    DMAConfig.circular = ENABLE;
    
    /* 
        传输方向 
            DMA_READ_FROM_PERIPH        从外设读取
            DMA_READ_FROM_MEM           从内存读取
    */
    DMAConfig.direction = DMA_READ_FROM_PERIPH;

    /* 是否内存传内存 */
    DMAConfig.MEM2MEM = DISABLE;

    /* 
        内存每次传输数据大小 
            DMA_MEM_BYTE_MODE_1TIME         每次传输一个字（32bit）
            DMA_MEM_BYTE_MODE_2TIME         每次传输半个字（16bit）
            DMA_MEM_BYTE_MODE_4TIME         每次传输一个字节（8bit）
    */
    DMAConfig.memByteMode = DMA_MEM_BYTE_MODE_1TIME;

    /* 内存地址自增 */
    DMAConfig.memIncrement = ENABLE;                              

    /* 外设数据寄存器地址自增 */
    DMAConfig.periphIncrement = DISABLE;

    /* 
        内存数据格式 
            DMA_MEM_SIZE_8BIT
            DMA_MEM_SIZE_16BIT
            DMA_MEM_SIZE_32BIT
    */
    DMAConfig.memSize = DMA_MEM_SIZE_32BIT;

    /* 
        外设寄存器数据格式 
            DMA_PERIPH_SIZE_8BIT
            DMA_PERIPH_SIZE_16BIT
            DMA_PERIPH_SIZE_32BIT
    */
    DMAConfig.periphSize = DMA_PERIPH_SIZE_16BIT;

    /* DMA通道传输长度 */
    DMAConfig.transferNum = memLen_ch0;                                    //DMA通道搬运的次数，与每次传输的数据大小和总的字节数有关

    /* 
        外设选择 
            DMA_PEPIRH_UART0_TX
            DMA_PEPIRH_UART0_RX
            DMA_PEPIRH_UART1_TX
            DMA_PEPIRH_UART1_RX
            DMA_PEPIRH_UART2_TX
            DMA_PEPIRH_UART2_RX
            DMA_PEPIRH_SPI0_TX
            DMA_PEPIRH_SPI0_RX
            DMA_PEPIRH_SPI1_TX
            DMA_PEPIRH_SPI1_RX
            DMA_PEPIRH_I2C0_TX
            DMA_PEPIRH_I2C0_RX
            DMA_PEPIRH_I2C1_TX
            DMA_PEPIRH_I2C1_RX
            DMA_PEPIRH_ADC0
    */
    DMAConfig.periphSelect = DMA_PEPIRH_ADC0;   

    /* 中断回调 */       
    DMAConfig.callBack = DMA_Task;     

    DMA_Init(DMA0_CHANNEL0, &DMAConfig);   
#endif
#ifdef TE_DMA_CH1
    memset(&DMAConfig, 0x00, sizeof(DMA_ConfigType));

    /* memory设置 */
    DMAConfig.memStartAddr = (uint32_t)memStartAddr_ch1;          //memory起始地址
    DMAConfig.memEndAddr = (uint32_t)memEndAddr_ch1;              //memor结束地址

    /* 外设数据寄存器地址 */
    DMAConfig.periphStartAddr = (uint32_t)(&(ADC0->RDR));

    /* 通道使能 */
    DMAConfig.channelEn = ENABLE;

    /* 中断 */
    DMAConfig.finishInterruptEn = ENABLE;                         //传输完成
    DMAConfig.halfFinishInterruptEn = DISABLE;                    //传输一半
    DMAConfig.errorInterruptEn = ENABLE;                          //传输错误

    /* 
        通道优先级 
            DMA_PRIORITY_LOW
            DMA_PRIORITY_MEDIUM
            DMA_PRIORITY_HIGH
            DMA_PRIORITY_VERY_HIGH
    */
    DMAConfig.channelPriority = DMA_PRIORITY_VERY_HIGH;

    /* 循环传输 */
    DMAConfig.circular = ENABLE;
    
    /* 
        传输方向 
            DMA_READ_FROM_PERIPH        从外设读取
            DMA_READ_FROM_MEM           从内存读取
    */
    DMAConfig.direction = DMA_READ_FROM_PERIPH;

    /* 是否内存传内存 */
    DMAConfig.MEM2MEM = DISABLE;

    /* 
        内存每次传输数据大小 
            DMA_MEM_BYTE_MODE_1TIME         每次传输一个字（32bit）
            DMA_MEM_BYTE_MODE_2TIME         每次传输半个字（16bit）
            DMA_MEM_BYTE_MODE_4TIME         每次传输一个字节（8bit）
    */
    DMAConfig.memByteMode = DMA_MEM_BYTE_MODE_1TIME;

    /* 内存地址自增 */
    DMAConfig.memIncrement = ENABLE;                              

    /* 外设数据寄存器地址自增 */
    DMAConfig.periphIncrement = DISABLE;

    /* 
        内存数据格式 
            DMA_MEM_SIZE_8BIT
            DMA_MEM_SIZE_16BIT
            DMA_MEM_SIZE_32BIT
    */
    DMAConfig.memSize = DMA_MEM_SIZE_32BIT;

    /* 
        外设寄存器数据格式 
            DMA_PERIPH_SIZE_8BIT
            DMA_PERIPH_SIZE_16BIT
            DMA_PERIPH_SIZE_32BIT
    */
    DMAConfig.periphSize = DMA_PERIPH_SIZE_16BIT;

    /* DMA通道传输长度 */
    DMAConfig.transferNum = 8;                                    //DMA通道搬运的次数，与每次传输的数据大小和总的字节数有关

    /* 
        外设选择 
            DMA_PEPIRH_UART0_TX
            DMA_PEPIRH_UART0_RX
            DMA_PEPIRH_UART1_TX
            DMA_PEPIRH_UART1_RX
            DMA_PEPIRH_UART2_TX
            DMA_PEPIRH_UART2_RX
            DMA_PEPIRH_SPI0_TX
            DMA_PEPIRH_SPI0_RX
            DMA_PEPIRH_SPI1_TX
            DMA_PEPIRH_SPI1_RX
            DMA_PEPIRH_I2C0_TX
            DMA_PEPIRH_I2C0_RX
            DMA_PEPIRH_I2C1_TX
            DMA_PEPIRH_I2C1_RX
            DMA_PEPIRH_ADC0
    */
    DMAConfig.periphSelect = DMA_PEPIRH_ADC0;   

    /* 中断回调 */       
    DMAConfig.callBack = DMA_Task;     

    DMA_Init(DMA0_CHANNEL1, &DMAConfig); 
#endif
#ifdef TE_DMA_CH2
    memset(&DMAConfig, 0x00, sizeof(DMA_ConfigType));

    /* memory设置 */
    DMAConfig.memStartAddr = (uint32_t)memStartAddr_ch2;          //memory起始地址
    DMAConfig.memEndAddr = (uint32_t)memEndAddr_ch2;              //memor结束地址

    /* 外设数据寄存器地址 */
    DMAConfig.periphStartAddr = (uint32_t)(&(ADC0->RDR));

    /* 通道使能 */
    DMAConfig.channelEn = ENABLE;

    /* 中断 */
    DMAConfig.finishInterruptEn = ENABLE;                         //传输完成
    DMAConfig.halfFinishInterruptEn = DISABLE;                    //传输一半
    DMAConfig.errorInterruptEn = ENABLE;                          //传输错误

    /* 
        通道优先级 
            DMA_PRIORITY_LOW
            DMA_PRIORITY_MEDIUM
            DMA_PRIORITY_HIGH
            DMA_PRIORITY_VERY_HIGH
    */
    DMAConfig.channelPriority = DMA_PRIORITY_VERY_HIGH;

    /* 循环传输 */
    DMAConfig.circular = ENABLE;
    
    /* 
        传输方向 
            DMA_READ_FROM_PERIPH        从外设读取
            DMA_READ_FROM_MEM           从内存读取
    */
    DMAConfig.direction = DMA_READ_FROM_PERIPH;

    /* 是否内存传内存 */
    DMAConfig.MEM2MEM = DISABLE;

    /* 
        内存每次传输数据大小 
            DMA_MEM_BYTE_MODE_1TIME         每次传输一个字（32bit）
            DMA_MEM_BYTE_MODE_2TIME         每次传输半个字（16bit）
            DMA_MEM_BYTE_MODE_4TIME         每次传输一个字节（8bit）
    */
    DMAConfig.memByteMode = DMA_MEM_BYTE_MODE_1TIME;

    /* 内存地址自增 */
    DMAConfig.memIncrement = ENABLE;                              

    /* 外设数据寄存器地址自增 */
    DMAConfig.periphIncrement = DISABLE;

    /* 
        内存数据格式 
            DMA_MEM_SIZE_8BIT
            DMA_MEM_SIZE_16BIT
            DMA_MEM_SIZE_32BIT
    */
    DMAConfig.memSize = DMA_MEM_SIZE_32BIT;

    /* 
        外设寄存器数据格式 
            DMA_PERIPH_SIZE_8BIT
            DMA_PERIPH_SIZE_16BIT
            DMA_PERIPH_SIZE_32BIT
    */
    DMAConfig.periphSize = DMA_PERIPH_SIZE_16BIT;

    /* DMA通道传输长度 */
    DMAConfig.transferNum = 8;                                    //DMA通道搬运的次数，与每次传输的数据大小和总的字节数有关

    /* 
        外设选择 
            DMA_PEPIRH_UART0_TX
            DMA_PEPIRH_UART0_RX
            DMA_PEPIRH_UART1_TX
            DMA_PEPIRH_UART1_RX
            DMA_PEPIRH_UART2_TX
            DMA_PEPIRH_UART2_RX
            DMA_PEPIRH_SPI0_TX
            DMA_PEPIRH_SPI0_RX
            DMA_PEPIRH_SPI1_TX
            DMA_PEPIRH_SPI1_RX
            DMA_PEPIRH_I2C0_TX
            DMA_PEPIRH_I2C0_RX
            DMA_PEPIRH_I2C1_TX
            DMA_PEPIRH_I2C1_RX
            DMA_PEPIRH_ADC0
    */
    DMAConfig.periphSelect = DMA_PEPIRH_ADC0;   

    /* 中断回调 */       
    DMAConfig.callBack = DMA_Task;     

    DMA_Init(DMA0_CHANNEL2, &DMAConfig); 
#endif
#ifdef TE_DMA_CH3
    memset(&DMAConfig, 0x00, sizeof(DMA_ConfigType));

    /* memory设置 */
    DMAConfig.memStartAddr = (uint32_t)memStartAddr_ch3;          //memory起始地址
    DMAConfig.memEndAddr = (uint32_t)memEndAddr_ch3;              //memor结束地址

    /* 外设数据寄存器地址 */
    DMAConfig.periphStartAddr = (uint32_t)(&(ADC0->RDR));

    /* 通道使能 */
    DMAConfig.channelEn = ENABLE;

    /* 中断 */
    DMAConfig.finishInterruptEn = ENABLE;                         //传输完成
    DMAConfig.halfFinishInterruptEn = DISABLE;                    //传输一半
    DMAConfig.errorInterruptEn = ENABLE;                          //传输错误

    /* 
        通道优先级 
            DMA_PRIORITY_LOW
            DMA_PRIORITY_MEDIUM
            DMA_PRIORITY_HIGH
            DMA_PRIORITY_VERY_HIGH
    */
    DMAConfig.channelPriority = DMA_PRIORITY_VERY_HIGH;

    /* 循环传输 */
    DMAConfig.circular = ENABLE;
    
    /* 
        传输方向 
            DMA_READ_FROM_PERIPH        从外设读取
            DMA_READ_FROM_MEM           从内存读取
    */
    DMAConfig.direction = DMA_READ_FROM_PERIPH;

    /* 是否内存传内存 */
    DMAConfig.MEM2MEM = DISABLE;

    /* 
        内存每次传输数据大小 
            DMA_MEM_BYTE_MODE_1TIME         每次传输一个字（32bit）
            DMA_MEM_BYTE_MODE_2TIME         每次传输半个字（16bit）
            DMA_MEM_BYTE_MODE_4TIME         每次传输一个字节（8bit）
    */
    DMAConfig.memByteMode = DMA_MEM_BYTE_MODE_1TIME;

    /* 内存地址自增 */
    DMAConfig.memIncrement = ENABLE;                              

    /* 外设数据寄存器地址自增 */
    DMAConfig.periphIncrement = DISABLE;

    /* 
        内存数据格式 
            DMA_MEM_SIZE_8BIT
            DMA_MEM_SIZE_16BIT
            DMA_MEM_SIZE_32BIT
    */
    DMAConfig.memSize = DMA_MEM_SIZE_32BIT;

    /* 
        外设寄存器数据格式 
            DMA_PERIPH_SIZE_8BIT
            DMA_PERIPH_SIZE_16BIT
            DMA_PERIPH_SIZE_32BIT
    */
    DMAConfig.periphSize = DMA_PERIPH_SIZE_16BIT;

    /* DMA通道传输长度 */
    DMAConfig.transferNum = 8;                                    //DMA通道搬运的次数，与每次传输的数据大小和总的字节数有关

    /* 
        外设选择 
            DMA_PEPIRH_UART0_TX
            DMA_PEPIRH_UART0_RX
            DMA_PEPIRH_UART1_TX
            DMA_PEPIRH_UART1_RX
            DMA_PEPIRH_UART2_TX
            DMA_PEPIRH_UART2_RX
            DMA_PEPIRH_SPI0_TX
            DMA_PEPIRH_SPI0_RX
            DMA_PEPIRH_SPI1_TX
            DMA_PEPIRH_SPI1_RX
            DMA_PEPIRH_I2C0_TX
            DMA_PEPIRH_I2C0_RX
            DMA_PEPIRH_I2C1_TX
            DMA_PEPIRH_I2C1_RX
            DMA_PEPIRH_ADC0
    */
    DMAConfig.periphSelect = DMA_PEPIRH_ADC0;   

    /* 中断回调 */       
    DMAConfig.callBack = DMA_Task;     

    DMA_Init(DMA0_CHANNEL3, &DMAConfig); 
#endif
}



