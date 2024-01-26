//Author https://gitee.com/maji19971221/lwip-routine
//Modified by mztulip
#include "CH57x_common.h"
#include <stdio.h>
#include <string.h>
#include "parameter_setting.h"

#include  "mybufque.h"
#include  "mych579mac.h"
#include  "ethernetif.h"
#include  "mytcpserver.h"

UINT8  g_cur_eth_comm_chan = PROT_DATA_FROM_TCP;
UINT8 g_eth_phy_link_state = 0;

void tcp_Postback_test(void)
{
    e_buf_t *p_cur_ebuf = NULL;

    p_cur_ebuf = get_valid_e_from_que_head(&g_eth_buf_que);

    if(p_cur_ebuf!=NULL)
    {
        MyTcpSendData(p_cur_ebuf->buf, p_cur_ebuf->buf_len);
        p_cur_ebuf->buf_len = 0; 
        e_buf_num_sub_one(&g_eth_buf_que);
    }
}

int main()
{ 
	SystemInit();

    PWR_UnitModCfg(ENABLE, (UNIT_SYS_PLL|UNIT_ETH_PHY)); 
    DelayMs(3); 

    SetSysClock(CLK_SOURCE_HSE_32MHz); 
    InitTimer0();
    DebugInit();
    printf("Hello\n");
    SetDefaultEthUartCfg();
    InitAppCommon();

    lwip_comm_init(); 
    TCP_server_init();
    NETLed_Init();

    while(1)
    {
        tcp_Postback_test();
        
        lwip_pkt_handle();

        lwip_periodic_handle();
    }
}

