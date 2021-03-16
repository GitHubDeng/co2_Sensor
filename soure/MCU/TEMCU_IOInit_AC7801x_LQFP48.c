/*
 * 2021/3/9 v1.0
 * Deng
 * 
 * MCU引脚布局
 * 
 *           PA14 RESET PA13 PB2 PB10 PB9 PC9 PC8 PC7 PC6 PA11 PA10
 *           48   47    46   45  44   43  42  41  40  39  35   37
 *   PB11 1                                                       36 PB8
 *   PB12 2                                                       35 PB7
 *    PB0 3                                                       34 PC5
 *    PB1 4                                                       33 PB6
 *   VDD1 5                                                       32 PC4
 *   VDDA 6                 Autochips_AC7801x_LQFP48              31 VDD2
 *   VSS1 7                                                       30 VSS2
 *   PA12 8                                                       29 PA9
 *   PA15 9                                                       28 PC3
 *    PA0 10                                                      27 PC2
 *    PA1 11                                                      26 PA8
 *   PB13 12                                                      25 PA7
 *           13   14  15   16  17  18  19   20   21   22  23   24
 *           PB3  PA2 PA3  PA4 PA5 PA6 PB14 PB15 PC0  PC1 PB4  PB5
 *
 *
 * MCU引脚复用功能
 *
 *   Pin NO.   name  |  func0        func1        func2        func3
 *      1      PB11  |  PB11         PWM0_CH3     PB11         SPI1_MOSI
 *      2      PB12  |  PB12         PWM0_CH2     PB12         SPI1_SCK
 *      3      PB0   |  PB0          CAN_TX       PWM0_CH7     SPI1_MISO     
 *      4      PB1   |  PB1          CAN_RX       PWM0_CH6     SPI1_NSS
 *      5      VDD1  |  VDD1--------------------------------------------
 *      6      VDDA  |  VDDA--------------------------------------------
 *      7      VSS1  |  VSS1--------------------------------------------
 *      8      PA12  |  -----        -----        OSC_OUT      -----         
 *      9      PA15  |  PA15         I2C0_SDA     OSC_IN       PWDT0_IN0     
 *      10     PA0   |  PA0          PWM0_CH1     UART0_RTS    I2C0_SCL
 *      11     PA1   |  PA1          PWM0_CH0     UART0_CTS    I2C0_SDA
 *      12     PB13  |  PB13         PWM0_CH7     PB13         I2C1_SCL
 *      13     PB3   |  PB3          PWM0_CH6     PWM1_CH7     SPI0_MOSI
 *      14     PA2   |  PA2          PWM0_CH5     ADC_IN8      SPI0_MISO
 *      15     PA3   |  PA3          PWM0_CH4     ADC_IN7      SPI0_SCK
 *      16     PA4   |  PA4          PWM0_CH3 ADC_IN6/ACMP_IN6 UART1_TX
 *      17     PA5   |  PA5          PWM0_CH2 ADC_IN5/ACMP_IN5 UART1_RX
 *      18     PA6   |  PA6          BOOT         PA6          PA6
 *      19     PB14  |  PB14         PWM0_CH1     PB14         SPI1_MOSI
 *      20     PB15  |  PB15         PWM1_FLT0    ADC_IN11     SPI1_SCK
 *      21     PC0   |  PC0          PWM1_CH3     ADC_IN10     SPI1_MISO
 *      22     PC1   |  PC1          PWM1_CH2     ADC_IN9      SPI1_NSS
 *      23     PB4   |  PB4          PWM1_CH1     ADC_IN8      SPI0_MISO
 *      24     PB5   |  PB5          PWM1_CH0     ADC_IN7      SPI0_SCK
 *      25     PA7   |  PA7          UART0_TX ADC_IN4/ACMP_IN4 SPI0_MOSI
 *      26     PA8   |  PA8          UART0_RX ADC_IN3/ACMP_IN3 SPI0_NSS
 *      27     PC2   |  PC2          UART1_TX     PWM0_FLT1    UART0_TX
 *      28     PC3   |  PC3          UART1_RX     PWM1_FLT1    UART0_RX
 *      29     PA9   |  PA9         PWM0_FLT0 ADC_IN2/ACMP_IN2 RTC_CLKIN
 *      30     VSS2  |  VSS2--------------------------------------------
 *      31     VDD2  |  VDD2--------------------------------------------
 *      32     PC4   |  PC4          PWM0_CH1     PC4          I2C1_SDA
 *      33     PB6   |  PB6          PWM1_CH6     PWM1_FLT0    CAN_STDBY
 *      34     PC5   |  PC5          PC5          PWDT0_IN1    SPI0_NSS
 *      35     PB7   |  PB7          PWM1_CH3     ACMP_IN3     I2C0_SCL
 *      36     PB8   |  PB8          PWM1_CH2     PWDT0_IN2    I2C0_SDA
 *      37     PA10  |  PA10         PWM0_CH7 ADC_IN1/ACMP_IN1 PWDT0_IN2
 *      38     PA11  |  PA11         PWM0_CH6 ADC_IN0/ACMP_IN0 PWDT0_IN1
 *      39     PC6   |  PC6          UART1_TX     PC6          PWDT1_IN2
 *      40     PC7   |  PC7          UART1_RX     PC7          PWDT1_IN1
 *      41     PC8   |  PC8          PWM1_CH7     CAN_STDBY    PWDT1_IN0
 *      42     PC9   |  PC9          PWM1_CH6     PC9          ACMP_OUT
 *      43     PB9   |  PB9          PWM1_CH5     I2C1_SCL     UART2_TX
 *      44     PB10  |  PB10         PWM1_CH4     I2C1_SDA     UART2_RX
 *      45     PB2   |  PB2          NMI_B1       PWM0_FLT0    PWDT0_IN0
 *      46     PA13  |  PA13         SWD_CLK1     ----         RTC_CLKOUT
 *      47    RESET_B| RESET_B------------------------------------------
 *      48     PA14  |  PA14         SWD_DIO1     ACMP_OUT     PWM1_CH0
 *
 *
 * 函数
 * GPIO_SetFunc(GPIOx,GPIO_Pinx,GPIO_FUNx)
 * GPIO_SetDir(GPIOx,GPIO_Pinx,GPIO_OUT/GPIO_IN)
 * GPIO_SetPinLevel(GPIOx,GPIO_Pinx,GPIO_LEVEL_HIGH/GPIO_LEVEL_LOW)
 *
 *
 */
#include "TEMCU_IO.h"

struct TEGPIO{
	GPIO_Type *type;
	GPIO_PinType num;
}Pin;

#define	GPIOSET(m,n)	Pin.type = GPIO##m;\
                      Pin.num =  GPIO_PIN##n
#define	FUN(n)				GPIO_SetFunc(Pin.type,Pin.num,GPIO_FUN##n)
#define	OUTPUT        GPIO_SetDir(Pin.type,Pin.num,GPIO_OUT)
#define	INPUT         GPIO_SetDir(Pin.type,Pin.num,GPIO_IN)
#define	HIGH          GPIO_SetPinLevel(Pin.type,Pin.num,GPIO_LEVEL_HIGH)
#define	LOW           GPIO_SetPinLevel(Pin.type,Pin.num,GPIO_LEVEL_LOW)

void TEMCU_IOInit(void)
{
 /**********************************************************************************
	*   Pin Num:1
	*        IO:PB11
	*/
	/*
	*  Function:
	*      User:
	*   
	*/
	
 /**********************************************************************************
	*   Pin Num:2
	*        IO:PB12
	*/
	/*
	*  Function:
	*      User:
	*/

 /**********************************************************************************
	*   Pin Num:3
	*        IO:PB0
	*/
	/*
	*  Function:
	*      User:
	*/
	
 /**********************************************************************************
	*   Pin Num:4
	*        IO:PB1
	*/
	/*
	*  Function:
	*      User:
	*/
	
 /**********************************************************************************
	*   Pin Num:5
	*        IO:VDD1
	*/
	
 /**********************************************************************************
	*   Pin Num:6
	*        IO:VDDA
	*/

 /**********************************************************************************
	*   Pin Num:7
	*        IO:VSS1
	*/
	
 /**********************************************************************************
	*   Pin Num:8
	*        IO:PA12
	*/
	/*
	*  Function:
	*      User:
	*/

 /**********************************************************************************
	*   Pin Num:9
	*        IO:PA15.
	*/
	/*
	*  Function:
	*      User:
	*/
	
 /**********************************************************************************
	*   Pin Num:10
	*        IO:PA0
	*/
	/*
	*  Function:
	*      User:
	*/
	
 /**********************************************************************************
	*   Pin Num:11
	*        IO:PA1
	*/
	/*
	*  Function:
	*      User:
	*/
	
 /**********************************************************************************
	*   Pin Num:12
	*        IO:PB13
	*/
	/*
	*  Function:
	*      User:
	*/
	
 /**********************************************************************************
	*   Pin Num:13
	*        IO:PB3
	*/
	/*
	*  Function:
	*      User:
	*/
	
 /**********************************************************************************
	*   Pin Num:14
	*        IO:PA2
	*/
	GPIOSET(A,2);
	/*
	*  Function:
	*      User:
	*/
	FUN(2);
 /**********************************************************************************
	*   Pin Num:15
	*        IO:PA3
	*/
	/*
	*  Function:
	*      User:
	*/
	
 /**********************************************************************************
	*   Pin Num:16
	*        IO:PA4
	*/
	/*
	*  Function:
	*      User:
	*/
	
 /**********************************************************************************
	*   Pin Num:17
	*        IO:PA5
	*/
	/*
	*  Function:
	*      User:
	*/
	
 /**********************************************************************************
	*   Pin Num:18
	*        IO:PA6
	*/
	/*
	*  Function:
	*      User:
	*/
	
 /**********************************************************************************
	*   Pin Num:19
	*        IO:PB14
	*/
	/*
	*  Function:
	*      User:
	*/
	
 /**********************************************************************************
	*   Pin Num:20
	*        IO:PB15
	*/
	/*
	*  Function:
	*      User:
	*/

 /**********************************************************************************
	*   Pin Num:21
	*        IO:PC0
	*/
	/*
	*  Function:
	*      User:
	*/
	
 /**********************************************************************************
	*   Pin Num:22
	*        IO:PC1
	*/
	/*
	*  Function:
	*      User:
	*/
	
 /**********************************************************************************
	*   Pin Num:23
	*        IO:PB4
	*/
	/*
	*  Function:
	*      User:
	*/
	
 /**********************************************************************************
	*   Pin Num:24
	*        IO:PB5
	*/
	/*
	*  Function:
	*      User:
	*/
	
 /**********************************************************************************
	*   Pin Num:25
	*        IO:PA7
	*/
	/*
	*  Function:
	*      User:
	*/
	
 /**********************************************************************************
	*   Pin Num:26
	*        IO:PA8
	*/
	/*
	*  Function:
	*      User:
	*/

 /**********************************************************************************
	*   Pin Num:27
	*        IO:PC2
	*/
	/*
	*  Function:
	*      User:
	*/
	
 /**********************************************************************************
	*   Pin Num:28
	*        IO:PC3
	*/
	/*
	*  Function:
	*      User:
	*/
	
 /**********************************************************************************
	*   Pin Num:29
	*        IO:PA9
	*/
	/*
	*  Function:
	*      User:
	*/
	
 /**********************************************************************************
	*   Pin Num:30
	*        IO:VSS2
	*/
	
 /**********************************************************************************
	*   Pin Num:31
	*        IO:VDD2
	*/
	
 /**********************************************************************************
	*   Pin Num:32
	*        IO:PC4
	*/
	/*
	*  Function:
	*      User:
	*/

 /**********************************************************************************
	*   Pin Num:33
	*        IO:PB6
	*/
	/*
	*  Function:
	*      User:
	*/
	
 /**********************************************************************************
	*   Pin Num:34
	*        IO:PC5
	*/
	/*
	*  Function:
	*      User:
	*/
	
 /**********************************************************************************
	*   Pin Num:35
	*        IO:PB7
	*/
	/*
	*  Function:
	*      User:
	*/
	
 /**********************************************************************************
	*   Pin Num:36
	*        IO:PB8
	*/
	/*
	*  Function:
	*      User:
	*/
	
 /**********************************************************************************
	*   Pin Num:37
	*        IO:PA10
	*/
	/*
	*  Function:
	*      User:
	*/
	
 /**********************************************************************************
	*   Pin Num:38
	*        IO:PA11
	*/
	/*
	*  Function:
	*      User:
	*/

 /**********************************************************************************
	*   Pin Num:39
	*        IO:PC6
	*/
	/*
	*  Function:
	*      User:
	*/
	
 /**********************************************************************************
	*   Pin Num:40
	*        IO:PC7
	*/
	GPIOSET(C,7);
 /*
	*  Function:
	*      User:
	*/
	FUN(0);
	OUTPUT;
	LOW;

 /**********************************************************************************
	*   Pin Num:41
	*        IO:PC8
	*/
	/*
	*  Function:
	*      User:
	*/
	
 /**********************************************************************************
	*   Pin Num:42
	*        IO:PC9
	*/
	GPIOSET(C,9);
 /*
	*  Function:
	*      User:
	*/
	FUN(0);
	OUTPUT;
	LOW;
	
 /**********************************************************************************
	*   Pin Num:43
	*        IO:PB9
	*/
	GPIOSET(B,9);
	/*
	*  Function:
	*      User:
	*/
	FUN(3);
	
 /**********************************************************************************
	*   Pin Num:44
	*        IO:PB10
	*/
	GPIOSET(B,10);
	/*
	*  Function:
	*      User:
	*/
	FUN(3);
 /**********************************************************************************
	*   Pin Num:45
	*        IO:PB2
	*/
	/*
	*  Function:
	*      User:
	*/
	
 /**********************************************************************************
	*   Pin Num:46
	*        IO:PA13
	*/
 /*
	*  Function:
	*      User:
	*/
	
 /**********************************************************************************
	*   Pin Num:47
	*        IO:RESET
	*/
	
 /**********************************************************************************
	*   Pin Num:48
	*        IO:PA14
	*/
	/*
	*  Function:
	*      User:
	*/
}
