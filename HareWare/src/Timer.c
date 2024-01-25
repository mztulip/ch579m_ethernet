#include "Timer.h"

uint32_t lwip_localtime;		//lwip本地时间计数器,单位:ms
/*******************************************************************************
* Function Name  :  Timer0Init
* Description    : 定时器1初始化
* Input          : time 定时时间
* Output         : None
* Return         : None
*******************************************************************************/
void Timer0Init(UINT32 time)
{
	R8_TMR0_CTRL_MOD = RB_TMR_ALL_CLEAR;		                               /* 清除所有计数值 */
	R8_TMR0_CTRL_MOD = 0;						                               /* 设置定时器模式 */
	R32_TMR0_CNT_END = FREQ_SYS/1000000*time;	                               /* 设置定时时间 */
	R8_TMR0_INT_FLAG = R8_TMR0_INT_FLAG;		                               /* 清除标志 */
	R8_TMR0_INTER_EN = RB_TMR_IE_CYC_END;	                                   /* 定时中断 */
	R8_TMR0_CTRL_MOD |= RB_TMR_COUNT_EN;
	NVIC_EnableIRQ(TMR0_IRQn);	
}

void CH57xNET_TimeIsr(UINT8 time)
{
	lwip_localtime +=time; //加10 +=10; //加10
}

void TMR0_IRQHandler( void ) 					                             	/* 定时器中断 */
{
	CH57xNET_TimeIsr(CH57xNETTIMEPERIOD);                                       /* 定时器中断服务函数 */
	R8_TMR0_INT_FLAG |= 0xff;					                             	/* 清除定时器中断标志 */
}

