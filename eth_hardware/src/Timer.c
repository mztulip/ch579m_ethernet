#include "Timer.h"

uint32_t lwip_localtime;		//lwip����ʱ�������,��λ:ms
/*******************************************************************************
* Function Name  :  Timer0Init
* Description    : ��ʱ��1��ʼ��
* Input          : time ��ʱʱ��
* Output         : None
* Return         : None
*******************************************************************************/
void Timer0Init(UINT32 time)
{
	R8_TMR0_CTRL_MOD = RB_TMR_ALL_CLEAR;		                               /* ������м���ֵ */
	R8_TMR0_CTRL_MOD = 0;						                               /* ���ö�ʱ��ģʽ */
	R32_TMR0_CNT_END = FREQ_SYS/1000000*time;	                               /* ���ö�ʱʱ�� */
	R8_TMR0_INT_FLAG = R8_TMR0_INT_FLAG;		                               /* �����־ */
	R8_TMR0_INTER_EN = RB_TMR_IE_CYC_END;	                                   /* ��ʱ�ж� */
	R8_TMR0_CTRL_MOD |= RB_TMR_COUNT_EN;
	NVIC_EnableIRQ(TMR0_IRQn);	
}

void CH57xNET_TimeIsr(UINT8 time)
{
	lwip_localtime +=time; //��10 +=10; //��10
}

void TMR0_IRQHandler( void ) 					                             	/* ��ʱ���ж� */
{
	CH57xNET_TimeIsr(CH57xNETTIMEPERIOD);                                       /* ��ʱ���жϷ����� */
	R8_TMR0_INT_FLAG |= 0xff;					                             	/* �����ʱ���жϱ�־ */
}

