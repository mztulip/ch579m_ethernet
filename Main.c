//Author mztulip
//mateusz@tulip.lol

#include "CH57x_common.h"
#include <stdio.h>


int main()
{ 
	SystemInit();
	
	/* PB0-LED */
    GPIOB_ModeCfg(GPIO_Pin_0, GPIO_ModeOut_PP_20mA);
    GPIOA_ModeCfg(GPIO_Pin_8, GPIO_ModeIN_PU);
    GPIOA_ModeCfg(GPIO_Pin_9, GPIO_ModeOut_PP_5mA);

    UART1_DefInit();
    UART1_BaudRateCfg(9600);

    while(1)
    {
        mDelaymS(500);
        GPIOB_SetBits( GPIO_Pin_0 ); 
        mDelaymS(500);
        GPIOB_ResetBits( GPIO_Pin_0 );
    }
}

