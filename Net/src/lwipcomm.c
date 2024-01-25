//https://gitee.com/maji19971221/lwip-routine
#include "netif/etharp.h"
#include "lwip/dhcp.h"
#include "lwip/mem.h"
#include "lwip/memp.h"
#include "lwip/init.h"

//#include "lwip/tcp_impl.h"
#include "lwip/ip4_frag.h"
#include "lwip/tcpip.h" 
//#include "lwip/timers.h"
#include "lwip/timeouts.h"
#include "lwip/netif.h"
#include "lwip/init.h"
#include "lwip/def.h"
#include "lwip/stats.h"
#include "ethernetif.h" 

#include <stdio.h>
#include "parameter_setting.h"

#include "mych579mac.h"


#include "ip4_addr.h"

struct netif lwip_netif;


static UINT8 phyflag = 0;
ip4_addr_t ipaddr;  			
ip4_addr_t netmask; 			
ip4_addr_t gw;      			 
static void  IP4_ADDR_X(struct ip4_addr *ipaddr,u32_t ipaddrx)
{
	ipaddr->addr = htonl(ipaddrx);
}

UINT8  lwip_comm_init(void)
{

	struct netif *Netif_Init_Flag;	

	lwip_init();

	IP4_ADDR_X(&ipaddr, MY_ETH_IP);  
	IP4_ADDR_X(&netmask, MY_ETH_MASK);  
	IP4_ADDR_X(&gw, MY_ETH_GW); 
	printf("IP:%ld.%ld.%ld.%ld\n\n",  \
        ((MY_ETH_IP)&0x000000ff),       \
        (((MY_ETH_IP)&0x0000ff00)>>8),  \
        (((MY_ETH_IP)&0x00ff0000)>>16), \
        ((MY_ETH_IP)&0xff000000)>>24);
	Netif_Init_Flag=netif_add(&lwip_netif,&ipaddr,&netmask,&gw,NULL,&ethernetif_init,&ethernet_input);

	if(Netif_Init_Flag==NULL)
	{
		printf("netif_add fail\r\n");
		return 4;
	}
	else
	{
		printf("netif_add success\r\n");
		netif_set_default(&lwip_netif); 
		netif_set_up(&lwip_netif);
		netif_set_link_up(&lwip_netif); 
	}

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
			SET_NET_LED_ON();
			
		}
		else
		{
			SET_NET_LED_OFF();
		}
	}

	last_phy_link_state = phy_link_state;

    sys_check_timeouts();	
	
}

