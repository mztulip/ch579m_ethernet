
#ifndef 	__PARAMETER_SETTING_H__
#define   __PARAMETER_SETTING_H__

#include "CH57x_common.h"
#include "core_cm0.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define  SET_NET_LED_ON()		 GPIOB_ResetBits(GPIO_Pin_4)
#define  SET_NET_LEDX_ON()		 GPIOB_ResetBits(GPIO_Pin_7)


#define  SET_NET_LED_OFF()		 GPIOB_SetBits(GPIO_Pin_4)
#define  SET_NET_LEDX_OFF()		 GPIOB_SetBits(GPIO_Pin_7)

#define  SET_NET_LEDX_FLASH()	 GPIOB_InverseBits(GPIO_Pin_7)
	
								
#define  	GET_SYS_TICK()			gSysTicket

extern 	 UINT8  			gMyPeerMac[];
extern   UINT32   			gSysTicket ;

void  InitTimer0(void);
void  InitAppCommon(void);
void  NETLed_Init(void);

#endif 
