//Based on https://gitee.com/maji19971221/lwip-routine
// Modified by mztulip
#include "netif/etharp.h"
#include "lwip/dhcp.h"
#include "lwip/mem.h"
#include "lwip/memp.h"
#include "lwip/init.h"
#include "lwip/ip4_frag.h"
#include "lwip/tcpip.h" 
#include "lwip/timeouts.h"
#include "lwip/netif.h"
#include "lwip/init.h"
#include "lwip/def.h"
#include "lwip/stats.h"
#include "ethernetif.h" 
#include "lwipcomm.h"

#include <stdio.h>
#include "parameter_setting.h"
#include "eth_mac.h"
#include "ip4_addr.h"

struct netif lwip_netif;

UINT8 g_eth_phy_link_state = 0;
static UINT8 phyflag = 0;
ip4_addr_t ipaddr;  			
ip4_addr_t netmask; 			
ip4_addr_t gw;

void eth_green_led_on(void)
{
	GPIOB_ResetBits(GPIO_Pin_4);
}

void eth_green_led_off(void)
{
	GPIOB_SetBits(GPIO_Pin_4);
}

void eth_amber_led_on(void)
{
	GPIOB_SetBits(GPIO_Pin_7);
}

void eth_amber_led_off(void)
{
	GPIOB_SetBits(GPIO_Pin_4);
}

static void  IP4_ADDR_X(struct ip4_addr *ipaddr,u32_t ipaddrx)
{
	ipaddr->addr = htonl(ipaddrx);
}

void NETLed_Init(void)
{
	GPIOB_ModeCfg(GPIO_Pin_4, GPIO_ModeOut_PP_20mA);
	GPIOB_ModeCfg(GPIO_Pin_7, GPIO_ModeOut_PP_20mA);

	eth_green_led_off();
	eth_green_led_off();
}

uint8_t lwip_comm_init(void)
{
	struct netif *Netif_Init_Flag;	

	NETLed_Init();
	lwip_init();
   
	IP4_ADDR_X(&ipaddr, 0xC0A802EB);  //192.168.2.235 
	IP4_ADDR_X(&netmask, 0xFFFFFF00);  
	IP4_ADDR_X(&gw, 0xC0A80201); //192.168.2.1

	printf("\n\rIP:%ld.%ld.%ld.%ld\n\r",  \
        ((ipaddr.addr)&0x000000ff),       \
        (((ipaddr.addr)&0x0000ff00)>>8),  \
        (((ipaddr.addr)&0x00ff0000)>>16), \
        ((ipaddr.addr)&0xff000000)>>24);
	Netif_Init_Flag=netif_add(&lwip_netif,&ipaddr,&netmask,&gw,NULL,&ethernetif_init,&ethernet_input);

	if(Netif_Init_Flag==NULL)
	{
		printf("\n\rnetif_add fail");
		return 4;
	}
	else
	{
		
		netif_set_default(&lwip_netif); 
		netif_set_up(&lwip_netif);
		netif_set_link_up(&lwip_netif); 
	}

	printf("\n\rlwip initialised sucessfully.");
	return 0;
	
}

void lwip_pkt_handle(void)
{
    ethernetif_input(&lwip_netif);
}

void lwip_periodic_handle(void)
{
	static UINT32 last_timestamp = 0;
	static UINT8  last_phy_link_state = 0;
	UINT8 phy_link_state = 0;
	UINT32 cur_timestamp = GET_SYS_TICK();

	if(cur_timestamp < last_timestamp)
	{
		last_timestamp = 0;

		return;
	}

	if(cur_timestamp - last_timestamp < 50)
	{
		//500ms
		return;
	}

	//500ms
	last_timestamp = cur_timestamp;

	phy_link_state = GetPhyLinkState();
	
	if(phy_link_state == 0)
	{
		phy_change(phyflag);
		phyflag += 1;
		DelayUs(phyflag);
		if(phyflag == 200)
			phyflag=0;
	}
	
	g_eth_phy_link_state = phy_link_state;

	if(phy_link_state != last_phy_link_state)
	{
		if(phy_link_state)
		{
			eth_green_led_on();
			
		}
		else
		{
			eth_green_led_off();
		}
	}

	last_phy_link_state = phy_link_state;

    sys_check_timeouts();	
	
}

