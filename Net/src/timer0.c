#include "timer0.h"
#include "CH57x_common.h"

UINT32  gSysTicket = 0;

extern UINT32  g_lwip_localtime;
static  UINT32  TimingDelay = 0;

/*************************************************************************
Timer0    10ms
*************************************************************************/
void InitTimer0(void)
{
  TMR0_TimerInit((10000*32));  //10ms 
  TMR0_ITCfg(ENABLE, TMR0_3_IT_CYC_END); 
  NVIC_EnableIRQ(TMR0_IRQn);
}


void TMR0_IRQHandler( void )        // TMR0
{
   TMR0_ClearITFlag( TMR0_3_IT_CYC_END );

		gSysTicket++;
		g_lwip_localtime+= 10;
		

		if (TimingDelay > 0)
		{ 
		 	TimingDelay--;
		}
}