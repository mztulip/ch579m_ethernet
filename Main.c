#include "CH57x_common.h"
#include <stdio.h>
#include <string.h>
#include "parameter_setting.h"
#include  "eth_mac.h"
#include  "ethernetif.h"

void uart_init(void)		
{
    GPIOA_SetBits(GPIO_Pin_9);
    GPIOA_ModeCfg(GPIO_Pin_8, GPIO_ModeIN_PU);
    GPIOA_ModeCfg(GPIO_Pin_9, GPIO_ModeOut_PP_5mA);
    UART1_DefInit();
}

int main()
{ 
	SystemInit();

    PWR_UnitModCfg(ENABLE, (UNIT_SYS_PLL|UNIT_ETH_PHY)); 
    DelayMs(3); 
    SetSysClock(CLK_SOURCE_HSE_32MHz); 

    InitTimer0();
    uart_init();
    printf("Hello\n");
    InitAppCommon();

    while(1)
    {
        lwip_pkt_handle();
        lwip_periodic_handle();
    }
}

