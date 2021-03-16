#ifndef TECOM_H
#define TECOM_H

#include "TETypes.h"

void TECom_Init(void);
void TECom_RevCallBack_Request_Hook(void (*func)(uint8_t op,uint16_t addr));

#endif
