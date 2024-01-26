#ifndef  	__ETHERNET_IF_H__
#define  	__ETHERNET_IF_H__


#include "lwip/err.h"
#include "lwip/def.h"
#include "lwip/mem.h"
#include "lwip/pbuf.h"
#include <lwip/stats.h>
#include "netif/etharp.h"
#include "lwip/netif.h"

//#include  "mycommon.h"
#include "parameter_setting.h"
extern   struct netif  lwip_netif;

void  ethernetif_input(struct netif *netif);
err_t ethernetif_init(struct netif *netif);

UINT8    lwip_comm_init(void);
void  lwip_pkt_handle(void);
void  lwip_periodic_handle(void);

#endif 


