#include "main.h"
#include "TEMCU_IO.h"
#include "TEMCU_NVIC.h"
#include "TEEvent.h"
#include "TEMCU_Timer.h"
#include "TECom.h"

int main(void)
{
   TEMCU_IOInit();

   TEMCU_NVIC_Init();

   TEMCU_Timer_Init();
   TEMCU_Timer_CallBack_Hook(TETimer_CH0,TEEvent_Trigger1MS);

   TECom_Init();

   while(1)
   {
      TEMainThread_Event();
   }
}

