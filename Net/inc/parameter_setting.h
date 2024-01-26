
#ifndef 	__PARAMETER_SETTING_H__
#define   __PARAMETER_SETTING_H__

#include "CH57x_common.h"
#include "core_cm0.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef struct  _eth_uart_cfg
{
	UINT16   save_flag;    
	UINT16   eth_port; 
	UINT32   eth_ip;
	UINT32   eth_gw;
	UINT32   eth_mask;
}eth_uart_cfg_t;

extern  eth_uart_cfg_t   gEthUartCfgInfo;

extern UINT8  g_eth_phy_link_state;



// #define   	PROT_DATA_FROM_TCP			1
// #define   	PROT_DATA_FROM_UDP			2
// #define   	PROT_DATA_FROM_MAC			3
// #define   	PROT_DATA_FROM_UDP3W	  4   //udp 30000 
// #define   	PROT_DATA_FROM_TCP3W		5   //tcp 30000 

extern  UINT8  g_cur_eth_comm_chan;   

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
