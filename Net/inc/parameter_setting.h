#ifndef 	__PARAMETER_SETTING_H__
#define   __PARAMETER_SETTING_H__

#include "CH57x_common.h"
#include "core_cm0.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>


	
								
#define  	GET_SYS_TICK()			gSysTicket

extern 	 UINT8  			gMyPeerMac[];
extern   UINT32   			gSysTicket ;

void  InitTimer0(void);
void  InitAppCommon(void);
void  NETLed_Init(void);

#endif 
