#ifndef TEMCU_UART_H
#define TEMCU_UART_H

#include <stdint.h>


typedef enum
{
    TEUart_0,
    TEUart_1,
    TEUart_2
}TEMCU_Uart_Type;

void TEMCU_Uart_RevStart(TEMCU_Uart_Type uart);
void TEMCU_Uart_RevStop(TEMCU_Uart_Type uart);

int TEMCU_Uart_Send(TEMCU_Uart_Type uart,uint8_t *data,uint16_t len);
void TEMCU_Uart_RevCallBack_Hook(TEMCU_Uart_Type uart,void (*Uart2Rev_CallBack)(uint8_t *data,uint16_t len));

void TEMCU_Uart_Init(void);


#endif
