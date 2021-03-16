#include "TEMCU_ADC.h"
#include "ac780x_adc.h"
#include "ac780x_adc_reg.h"


void TEMCU_ADC_Init(void)
{
    ADC_ConfigType adcConfig;

    /* 
        ADC总线分频 
            ADC系统时钟 = APB时钟/分频
    */
    adcConfig.clkPsc = ADC_CLK_PRESCALER_1; 

    /* 
        工作模式
            手册p142
            根据SCAN CONT DISCEN IDISEN IAUTO 5位确定工作模式

                mode1:  0000x   规则触发                单通道、单次触发单次采样
                mode2:  0100x   规则触发                单通道、单次触发连续采样
                mode3:  10000   规则\注入触发            多通道、单次触发连续采样、注入组有优先级
                mode4:  10001   规则触发+自动注入触发    多通道、单次触发连续采样、先规则组后注入组
                mode5:  11000   规则\注入触发           多通道、连续触发连续采样、注入组有优先级
                mode6:  11001   规则+自动注入触发       多通道、连续触发连续采样、先规则组后注入组
                mode7:  1x10x   规则触发                多通道、单次触发分组采样
                mode8:  1x01x   注入触发                多通道、单次触发单次采样

                                                        单通道：只采样规则组第一个通道
                                                        多通道：根据RSQL\ISQL的值采样对应组的通道数
                                                        单次触发：每个采样周期都需要触发
                                                        连续触发：每个采样周期结束后自动触发下一个采样周期
                                                        单次采样：每次只采样一个通道
                                                        连续采样：每次采样整个规则\注入组
                                                        分组采样：依据DISCNUM对每个通道分组，每次采样一个组
     */
    adcConfig.scanModeEn = ENABLE;                  //SCAN
    adcConfig.continousModeEn = DISABLE;            //CONT    
    adcConfig.regularDiscontinousModeEn = DISABLE;  //DISCEN
    adcConfig.injectDiscontinousModeEn = DISABLE;   //IDISEN
    adcConfig.injectAutoModeEn = DISABLE;           //IAUTO

    /* 
        间隔模式
            只在mode3、mode5使用
                ENABLE:注入组间隔模式，每次注入触发只会转换注入组序列的下一个通道
                DISABLE:注入组扫描模式，每次注入触发会转换注入组内的所有通道
     */
    adcConfig.intervalModeEn = DISABLE;    

    /* 分组采样时，每个组的长度（只在mode7使用） */
    adcConfig.regularDiscontinousNum = 0;           

    /* 
        中断使能
            mode1~6
                EOC：每个规则组或注入组通道转换结束都会触发
                IEOC：所有注入组通道转换完成触发
            mode7:
                EOC:所有规则组通道转换完成
                IEOC：始终不触发
            mode8:
                EOC:每个注入组通道转换结束都会触发
                IEOC：所有注入组通道转换完成触发

     */
    adcConfig.EOCInterruptEn = DISABLE;             //EOC
    adcConfig.IEOCInterruptEn = DISABLE;            //IEOC
    adcConfig.interruptEn = DISABLE;                //中断

    /* ADC DMA使能 */
    adcConfig.regularDMAEn = ENABLE;     

    /* 
        触发源
            规则组：外部触发\内部触发
            注入组：外部触发\内部触发
     */           
    adcConfig.regularTriggerMode = ADC_TRIGGER_INTERNAL;    //规则组
    adcConfig.injectTriggerMode = ADC_TRIGGER_INTERNAL;     //注入组

    /* 规则组序列长度 0~11 */
    adcConfig.regularSequenceLength = 1; 

    /* 注入组序列长度 0~3 */            
    adcConfig.injectSequenceLength = 0;    

    /* 数据对齐 */         
    adcConfig.dataAlign = ADC_DATA_ALIGN_RIGHT;     

    /* ADC上电 断电后ADC复位（配置寄存器不会被重置） */
    adcConfig.powerMode = ADC_POWER_ON;   

    ADC_Init(ADC0, &adcConfig);                      


    /* 
        设置采样时间和转换序列
        采样时间每个通道单独设置
        转换序列每个序列单独设置
        转换率
            转换时间 = 采样时间 + 转换时间 + 同步时间（5个APB时钟周期）
            转换时间 =（SPT + 12）/ADC时钟 + 5/APB时钟
     */
    /* 规则组 */
    // ADC_SetChannelSampleTime(ADC0, ADC_CH_8, ADC_SPT_CLK_7);        //采样时间     
    // ADC_SetRegularSequence(ADC0, ADC_CH_8, 0);                      //规则组序列    0~11
    ADC_SetRegularGroupChannel(ADC0, ADC_CH_8, ADC_SPT_CLK_7, 0);   
    /* 注入组 */
    // ADC_SetChannelSampleTime(ADCx, channel, spt);                   //采样时间
    // ADC_SetInjectSequence(ADCx, channel, seq);                      //注入组序列     0~3
    // ADC_SetInjectGroupChannel(ADC0, ADC_CH_8, ADC_SPT_CLK_7, 0);
}
