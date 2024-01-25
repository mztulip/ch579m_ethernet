#include "parameter_setting.h"


u8  gMyPeerMac[6];
u32  gSysTicket = 0;
__attribute__((aligned (4))) eth_uart_cfg_t  gEthUartCfgInfo;    //网络串口转接板配置信息


extern u32  g_lwip_localtime;

static  u32  TimingDelay = 0;

/* 打印配置 */
void DebugInit(void)		
{
    GPIOA_SetBits(GPIO_Pin_9);
    GPIOA_ModeCfg(GPIO_Pin_8, GPIO_ModeIN_PU);
    GPIOA_ModeCfg(GPIO_Pin_9, GPIO_ModeOut_PP_5mA);
    UART1_DefInit();
}



/*  网口灯IO参数配置*/

//初始化运行LED GPIO
void NETLed_Init(void)
{

	//配置PB4 PB7为输出模式 初始化为低电平
	GPIOB_ModeCfg(GPIO_Pin_4, GPIO_ModeOut_PP_20mA);
	GPIOB_ModeCfg(GPIO_Pin_7, GPIO_ModeOut_PP_20mA);

	//初始化网络状态为断开状态
	SET_NET_LED_OFF();
	SET_NET_LEDX_OFF();
}



/*************************************************************************
 初始化Timer0    10ms产生1次中断
*************************************************************************/
void InitTimer0(void)
{
	TMR0_TimerInit((10000*32));  //10ms 产生1次中断
  TMR0_ITCfg(ENABLE, TMR0_3_IT_CYC_END);  // 开启中断
  NVIC_EnableIRQ(TMR0_IRQn);
}



//设置默认配置信息
void  SetDefaultEthUartCfg(void)
{
	MY_ETH_IP = LED_CARD_DEFAULT_IP;		
	MY_ETH_GW  = LED_CARD_DEFAULT_GW;		
	MY_ETH_MASK = LED_CARD_DEFAULT_MASK;			
	MY_ETH_PORT = LED_CARD_DEFAULT_PORT;  			
}

/**********************************************
 初始化公共部分代码
***********************************************/
void  InitAppCommon(void)
{	
	//初始化phy 链路状态为断开连接状态
	g_eth_phy_link_state = 0; 

	init_my_buf_que(&g_eth_buf_que);  //网络接口接收缓冲区
}


void TMR0_IRQHandler( void )        // TMR0 定时中断
{
   TMR0_ClearITFlag( TMR0_3_IT_CYC_END );   // 清除中断标志

		gSysTicket++;
		g_lwip_localtime+= 10;
		

		if (TimingDelay > 0)
		{ 
		 	TimingDelay--;
		}
}

