#include "uart0.h"



UINT8 RxBuff[100];
UINT8 Txbuff[3]={0x57,0xAB,0x61};

void UART0_Init(void)
{
/* 配置串口0：先配置IO口模式，再配置串口 */   
	GPIOB_SetBits(GPIO_Pin_7);
	GPIOB_ModeCfg(GPIO_Pin_4, GPIO_ModeIN_PU);			// RXD-配置上拉输入
	GPIOB_ModeCfg(GPIO_Pin_7, GPIO_ModeOut_PP_5mA);		// TXD-配置推挽输出，注意先让IO口输出高电平
	UART0_DefInit();
	
	UART0_ByteTrigCfg( UART_1BYTE_TRIG );
//	trigB = 7;
	UART0_INTCfg( ENABLE, RB_IER_RECV_RDY|RB_IER_LINE_STAT );
	NVIC_EnableIRQ( UART0_IRQn );
}

void GPIO_Init(void)
{
	GPIOA_ModeCfg( GPIO_Pin_6, GPIO_ModeIN_PD );
	GPIOA_ITModeCfg( GPIO_Pin_6, GPIO_ITMode_RiseEdge );        // 下降沿唤醒
	NVIC_EnableIRQ( GPIO_IRQn );
}


void UART0_IRQHandler(void)
{
    UINT8 i;
    
    switch( UART0_GetITFlag() )
    {
        case UART_II_LINE_STAT:        // 线路状态错误
            UART0_GetLinSTA();
            break;
        
        case UART_II_RECV_RDY:          // 数据达到设置触发点

						RxBuff[i] = UART0_RecvByte();
						UART1_SendByte(RxBuff[i]);

            break;
        
        case UART_II_RECV_TOUT:         // 接收超时，暂时一帧数据接收完成
            i = UART0_RecvString(RxBuff);
            UART1_SendString( RxBuff, i ); 
            break;
        
        case UART_II_THR_EMPTY:         // 发送缓存区空，可继续发送
            break;
        
        case UART_II_MODEM_CHG:         // 只支持串口0
            break;
        
        default:
            break;
    }
}
void GPIO_IRQHandler(void)
{
    GPIOA_ClearITFlagBit( GPIO_Pin_6 );
		DelayMs(100);
		UART0_SendString(Txbuff,3);
}
