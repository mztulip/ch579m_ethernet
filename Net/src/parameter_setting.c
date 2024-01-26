#include "parameter_setting.h"


UINT32  gSysTicket = 0;
// __attribute__((aligned (4))) eth_uart_cfg_t  gEthUartCfgInfo;


extern UINT32  g_lwip_localtime;
static  UINT32  TimingDelay = 0;

void NETLed_Init(void)
{
	GPIOB_ModeCfg(GPIO_Pin_4, GPIO_ModeOut_PP_20mA);
	GPIOB_ModeCfg(GPIO_Pin_7, GPIO_ModeOut_PP_20mA);

	SET_NET_LED_OFF();
	SET_NET_LEDX_OFF();
}

/*************************************************************************
Timer0    10ms
*************************************************************************/
void InitTimer0(void)
{
  TMR0_TimerInit((10000*32));  //10ms 
  TMR0_ITCfg(ENABLE, TMR0_3_IT_CYC_END); 
  NVIC_EnableIRQ(TMR0_IRQn);
}

void  InitAppCommon(void)
{	 	
	g_eth_phy_link_state = 0; 
	NETLed_Init();
	lwip_comm_init(); 
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

