#include "uart0.h"



UINT8 RxBuff[100];
UINT8 Txbuff[3]={0x57,0xAB,0x61};

void UART0_Init(void)
{
/* ���ô���0��������IO��ģʽ�������ô��� */   
	GPIOB_SetBits(GPIO_Pin_7);
	GPIOB_ModeCfg(GPIO_Pin_4, GPIO_ModeIN_PU);			// RXD-������������
	GPIOB_ModeCfg(GPIO_Pin_7, GPIO_ModeOut_PP_5mA);		// TXD-�������������ע������IO������ߵ�ƽ
	UART0_DefInit();
	
	UART0_ByteTrigCfg( UART_1BYTE_TRIG );
//	trigB = 7;
	UART0_INTCfg( ENABLE, RB_IER_RECV_RDY|RB_IER_LINE_STAT );
	NVIC_EnableIRQ( UART0_IRQn );
}

void GPIO_Init(void)
{
	GPIOA_ModeCfg( GPIO_Pin_6, GPIO_ModeIN_PD );
	GPIOA_ITModeCfg( GPIO_Pin_6, GPIO_ITMode_RiseEdge );        // �½��ػ���
	NVIC_EnableIRQ( GPIO_IRQn );
}


void UART0_IRQHandler(void)
{
    UINT8 i;
    
    switch( UART0_GetITFlag() )
    {
        case UART_II_LINE_STAT:        // ��·״̬����
            UART0_GetLinSTA();
            break;
        
        case UART_II_RECV_RDY:          // ���ݴﵽ���ô�����

						RxBuff[i] = UART0_RecvByte();
						UART1_SendByte(RxBuff[i]);

            break;
        
        case UART_II_RECV_TOUT:         // ���ճ�ʱ����ʱһ֡���ݽ������
            i = UART0_RecvString(RxBuff);
            UART1_SendString( RxBuff, i ); 
            break;
        
        case UART_II_THR_EMPTY:         // ���ͻ������գ��ɼ�������
            break;
        
        case UART_II_MODEM_CHG:         // ֻ֧�ִ���0
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
