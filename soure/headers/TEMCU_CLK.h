#ifndef TEMCU_CLK_H
#define TEMCU_CLK_H

/*
     系统时钟配置
     系统时钟在system_ac780x.h中配置
     步骤：
        1、选择时钟源（69行）
           a、HSI->选择内部HSI作为时钟源，系统选择PLL输出
           b、HSE->选择外部晶体作为时钟源，系统选择PLL输出
           c、XOSC->选择外部晶振作为时钟源，并直接作为系统时钟输出
           d、XOSC_BYPASS->直接使用XI脚上的clock输入作为时钟源，系统时钟选择PLL输出
        2、选择时钟源频率（78行）
           选择内部时钟或者根据外部晶振频率选择相应的选项
        3、设置PLL参数（81~100行）
           需要设置的值有3个：PREDIV、FBKDIV、POSDIV
           根据时钟源的不同分别在82~84/86~88/90~92/94~96/98~100行设置（XTAL_8M等同于HSI_FREQ）
        4、设置系统时钟分频（109行）
           需要设置的值SYSCLK_DIV
        5、设置APB总线分频（110行）
           需要设置的值APBCLK_DIV
        6、根据设置计算时钟频率，并修改相应的宏的值（111~112行）
           ****此设置非常重要，计算结果必须准确****

    其中PREDIV、FBKDIV、POSDIV、SYSCLK_DIV四个值可根据时钟源频率和需要的系统频率通过下表查表获得
     fPLL_IN  PREDIV  FBKDIV  POSDIV  Frequency  Sysclk_div  System Clock
        8        1     108      6        144          2           72
        8        1     96       16       48           1           48
        8        1     96       24       32           1           32
        8        1     96       16       48           2           24
        8        1     96       16       48           3           16
        8        1     96       16       48           4           12
        4        1     216      12       72           1           72
        4        1     192      16       48           1           48
        4        1     192      24       32           1           32
        4        1     192      16       48           2           24
        4        1     192      16       48           3           16
        4        1     192      16       48           4           12
        30       2     48       10       72           1           72
        30       2     64       20       48           1           48
        30       2     64       30       32           1           32
        30       2     64       20       48           2           24
        30       2     64       20       48           3           16
        30       2     64       20       48           4           12
        12       2     144      12       72           1           72
        12       2     96       12       48           1           48
        12       2     96       18       32           1           32
        12       2     96       24       24           1           24
        12       2     96       18       32           2           16
        12       2     96       24       24           2           12
        16       2     144      16       72           1           72
        16       2     96       16       48           1           48
        16       2     96       24       32           1           32
        16       2     96       16       48           2           24
        16       2     96       24       32           2           16
        16       2     96       16       48           4           12
*/

#endif