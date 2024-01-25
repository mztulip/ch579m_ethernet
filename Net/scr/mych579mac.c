

/************************************************************
CH579 mac 通信

*************************************************************/

#include "mych579mac.h"

#include "parameter_setting.h"
static  RXBUFST   ETHRxMagPara;   //接收管理参数
static  TXBUFST   ETHTxMagPara;   //发送管理参数


static  __align(4) UINT8     MACRxBuf[RX_QUEUE_NUM][RX_BUF_SIZE];   /* MAC接收缓冲区，4字节对齐 */ 
static  __align(4) UINT8     MACTxBuf[TX_QUEUE_NUM][TX_BUF_SIZE];   /* MAC发送缓冲区，4字节对齐 */ 


UINT8   myCh579MAC[MACADDR_LEN] = {0x84,0xc2,0xe4,0x02,0x03,0x04};

u8 phy_flag=0;


/*******************************************************************************
 * @fn          WritePHYReg
 *
 * @brief       写PHY寄存器
 *
 * input parameters
 *
 * @param       reg_add. 寄存器地址
 * @param       reg_val. 寄存器的值
 *
 * output parameters
 *
 * @param       None.
 *
 * @return      None.
 */
static void WritePHYRegX(UINT8 reg_add,UINT16 reg_val)
{
	UINT8 reg_op=0;
	
	R16_ETH_MIWR = reg_val;
	reg_op = (reg_add&RB_ETH_MIREGADR_MIRDL);
	R8_ETH_MIREGADR = reg_op;   
}

/*******************************************************************************
 * @fn          ReadPHYReg
 *
 * @brief       读PHY寄存器
 *
 * input parameters
 *
 * @param       reg_add. 寄存器地址
 *
 * output parameters
 *
 * @param       None.
 *
 * @return      寄存器读取的值.
 */
static UINT16 ReadPHYRegX(UINT8 reg_add)
{
	UINT8 reg_op=0;
	UINT16 read_reg_val = 0xffff;
	
	reg_op = reg_add&RB_ETH_MIREGADR_MIRDL;
	R8_ETH_MIREGADR = RB_ETH_MIREGADR_MIIWR|reg_op;
	read_reg_val = R16_ETH_MIRD;
	
	return read_reg_val;
}


//初始化网络参数
void ETHParaInitX(void)
{
	UINT8 i = 0;
	
	memset((char *)&MACRxBuf[0][0],0,sizeof(MACRxBuf));
	memset((char *)&MACTxBuf[0][0],0,sizeof(MACTxBuf));
	memset((char *)&ETHRxMagPara,0,sizeof(ETHRxMagPara));
	memset((char *)&ETHTxMagPara,0,sizeof(ETHTxMagPara));
	
	for(i=0; i<RX_QUEUE_NUM; i++) 
	{
		ETHRxMagPara.RxBufAddr[i] = (UINT32)(&MACRxBuf[i][0]);
	}
	
	for(i=0; i<TX_QUEUE_NUM; i++)
	{
		ETHTxMagPara.TxBufAddr[i] = (UINT32)(&MACTxBuf[i][0]);
	}

	//发送接收使能关闭
	ETHRxMagPara.RecvEn = 0;   
	ETHTxMagPara.SendEn = 0;


	//获取唯一MAC地址
	GetMACAddress(myCh579MAC);
//	myCh579MAC[5]=myCh579MAC[0]+myCh579MAC[1]+myCh579MAC[2]+myCh579MAC[3]+myCh579MAC[4]+myCh579MAC[5];
//	myCh579MAC[0]=0x00; //确保MAC地址第1个字节为0
	

	printf("ETHParaInitX mac: %x:%x:%x:%x:%x:%x\r\n",myCh579MAC[5], myCh579MAC[4],myCh579MAC[3],myCh579MAC[2],myCh579MAC[1],myCh579MAC[0]);
	

}





//网络接口初始化
void ETHInitX(void)
{

	R8_SAFE_ACCESS_SIG = SAFE_ACCESS_SIG1;
	R8_SAFE_ACCESS_SIG = SAFE_ACCESS_SIG2;
	R8_SLP_CLK_OFF1 &= ~RB_SLP_CLK_ETH;	      //开启ETH控制器时钟
	R8_SLP_POWER_CTRL &= ~RB_SLP_ETH_PWR_DN;  //开启ETH控制器电源 
	R8_SAFE_ACCESS_SIG = 0;	

	R8_ETH_EIE = 0;
	R8_ETH_EIE |= RB_ETH_EIE_INTIE |
                  RB_ETH_EIE_RXIE  |
	              RB_ETH_EIE_LINKIE|
	              RB_ETH_EIE_TXIE  |
	              RB_ETH_EIE_TXERIE|
	              RB_ETH_EIE_RXERIE;   //开启所有中断
	
	
	R8_ETH_EIE |= RB_ETH_EIE_R_EN50;     //开启50欧上拉

	//写1清0
	R8_ETH_EIR = 0xff;               			//清除中断标志
	R8_ETH_ESTAT |= RB_ETH_ESTAT_INT | RB_ETH_ESTAT_BUFER;  //清除状态
	
	//收发模块复位
	R8_ETH_ECON1 |= (RB_ETH_ECON1_TXRST|RB_ETH_ECON1_RXRST);   
	R8_ETH_ECON1 &= ~(RB_ETH_ECON1_TXRST|RB_ETH_ECON1_RXRST);
	
	//接收使能在link中断里写

	//过滤模式，接收包类型
	R8_ETH_ERXFCON = 0;  //不过滤 接收所有包
	
	//过滤模式，限制包类型	
	R8_ETH_MACON1 |= RB_ETH_MACON1_MARXEN;       //MAC接收使能
	R8_ETH_MACON2 &= ~RB_ETH_MACON2_PADCFG;

	R8_ETH_MACON2 |= PADCFG_AUTO_3;               //所有短包自动填充到60
	
	R8_ETH_MACON2 |= RB_ETH_MACON2_TXCRCEN;          //硬件填充CRC
	R8_ETH_MACON2 &= ~RB_ETH_MACON2_HFRMEN;          //不接收巨型帧
	

	R8_ETH_MACON2 |= RB_ETH_MACON2_FULDPX;    //全双工
	R16_ETH_MAMXFL = MAC_MAX_LEN;            //MAC层最大接收包长度
	
	R8_ETH_MAADR1 = myCh579MAC[5];           //MAC赋值
	R8_ETH_MAADR2 = myCh579MAC[4];
	R8_ETH_MAADR3 = myCh579MAC[3];
	R8_ETH_MAADR4 = myCh579MAC[2];
	R8_ETH_MAADR5 = myCh579MAC[1];
	R8_ETH_MAADR6 = myCh579MAC[0];
	
	//使能ETH引脚
	R16_PIN_ANALOG_IE |= RB_PIN_ETH_IE;
	
	R16_ETH_ERXST = (UINT16)ETHRxMagPara.RxBufAddr[ETHRxMagPara.RecvIndex];     //当前接收缓存

	R8_ETH_ECON1 |= RB_ETH_ECON1_RXEN;                                      //接收使能

	NVIC_EnableIRQ(ETH_IRQn); 

	//printf("ETHInitX OK \r\n");
}


/*******************************************************************************
 * @fn          ETH_IRQ_ERR_Deal
 *
 * @brief       中断收发错误处理
 *
 * input parameters
 *
 * @param       err_sta. 当前错误状态
 *
 * output parameters
 *
 * @param       None.
 *
 * @return      None.
 */
static void ETH_IRQ_ERR_Deal(UINT8 err_sta)
{
	if(err_sta&RB_ETH_ESTAT_BUFER)    printf("err:BUFER\r\n");
	if(err_sta&RB_ETH_ESTAT_RXCRCER)  printf("err:RXCRCER\r\n");
	if(err_sta&RB_ETH_ESTAT_RXNIBBLE) printf("err:RXNIBBLE\r\n");
	if(err_sta&RB_ETH_ESTAT_RXMORE)   printf("err:RXMORE\r\n");
	if(err_sta&RB_ETH_ESTAT_RXBUSY)   printf("err:RXBUSY\r\n");
	if(err_sta&RB_ETH_ESTAT_TXABRT)   printf("err:TXABRT\r\n");
}



/*************************************************************
将待发送的数据放入MAC发送队列

send_len <= MAC_MAX_LEN(576 字节)

成功返回 0，失败返回 非0值

*************************************************************/
UINT8 ETHSendX(UINT8 *pSendBuf, UINT16 send_len)
{
	UINT16 len;
	UINT8 *p_data,*p_tx_buf;

	len = send_len;
	p_data = pSendBuf;
	
	if(!ETHTxMagPara.SendEn)  
	{
		return 0xff;
	}

	
	if(ETHTxMagPara.TxQueueCnt>=TX_QUEUE_NUM)
	{
		//发送队列满
		return 0xfe;
	}


	//标记发送缓冲区中有数据及数据长度
	ETHTxMagPara.TxBufStau[ETHTxMagPara.WriteIndex] = 1;
	ETHTxMagPara.TxBufLen[ETHTxMagPara.WriteIndex] = len;
	p_tx_buf = (UINT8 *)ETHTxMagPara.TxBufAddr[ETHTxMagPara.WriteIndex];
	

	memcpy(p_tx_buf, p_data, len);

	
	ETHTxMagPara.WriteIndex++;
	if(ETHTxMagPara.WriteIndex>=TX_QUEUE_NUM)
	{
		ETHTxMagPara.WriteIndex = 0;
	}

	                                            
	ETHTxMagPara.TxQueueCnt++;
	if(ETHTxMagPara.TxQueueCnt==1)   //只有一个包的时候，启动发送
	{
		R16_ETH_ETXLN = ETHTxMagPara.TxBufLen[ETHTxMagPara.SendIndex];          //写入长度
		R16_ETH_ETXST = (UINT16)ETHTxMagPara.TxBufAddr[ETHTxMagPara.SendIndex]; //写入地址         
		ETHTxMagPara.TxBufStau[ETHTxMagPara.SendIndex] = 0;                     //空闲
		R8_ETH_ECON1 |= RB_ETH_ECON1_TXRTS;                                     //开始发送
		ETHTxMagPara.SendIndex++;
		if(ETHTxMagPara.SendIndex>=TX_QUEUE_NUM) ETHTxMagPara.SendIndex = 0;
		ETHTxMagPara.TxQueueCnt--;
	}
	
	return 0;
}


/***************************************************************
接收以太网MAC层数据

pRecvBuf: 保存接收数据的缓冲区

pRecvBuf >= MAC缓冲区  MAC_MAX_LEN(576 字节)

RecvBufSize: 接收缓冲区大小

返回数据包的长度

1次调用只读取MAC接收缓冲区队列里面的一个接收缓冲区数据

//pRecvBuf  --> MAX512

****************************************************************/
UINT16 ETHRecX(UINT8     *pRecvBuf, UINT16 RecvBufSize)
{
	UINT16  recv_len = 0;
	UINT8  *p_data = NULL;
	UINT8  *p_rx_buf = NULL;

	
	if(!ETHRxMagPara.RemainCout) 
	{
		//ETHRxMagPara.RemainCout == 0 没有数据可接收
		return 0;  
	}
	
	
	p_data = pRecvBuf;
	p_rx_buf = (UINT8 *)ETHRxMagPara.RxBufAddr[ETHRxMagPara.ReadIndex];
	recv_len = (ETHRxMagPara.RxBufLen[ETHRxMagPara.ReadIndex]<RecvBufSize)?ETHRxMagPara.RxBufLen[ETHRxMagPara.ReadIndex]:RecvBufSize ;
	
	memcpy(p_data, p_rx_buf, recv_len);
	
	ETHRxMagPara.RxBufStau[ETHRxMagPara.ReadIndex] = 0;
	ETHRxMagPara.ReadIndex++;
	if(ETHRxMagPara.ReadIndex>=RX_QUEUE_NUM) ETHRxMagPara.ReadIndex = 0;

	ETHRxMagPara.RemainCout--;
	
	return recv_len;
	
}


/*
 获取MAC层接收到的数据的长度

*/
UINT16 GetRecvMacDataLen(void)
{
	UINT16  recv_len = 0;

	if(!ETHRxMagPara.RemainCout) 
	{
		//ETHRxMagPara.RemainCout == 0 没有数据可接收
		return 0;  
	}

	recv_len = ETHRxMagPara.RxBufLen[ETHRxMagPara.ReadIndex];

	return recv_len;
}


extern void  lwip_pkt_handle(void);

void ETH_IRQHandler(void)
{
	UINT8   eth_irq_flag = 0;
	UINT8   eth_irq_status = 0;
	UINT16  phy_reg = 0;
	UINT16  rec_len = 0;

	eth_irq_flag = R8_ETH_EIR;
	
	if(eth_irq_flag&RB_ETH_EIR_RXIF)                                           
	{	
		//接收完成中断	

		//硬件自动覆盖
		rec_len = R16_ETH_ERXLN;
		if(ETHRxMagPara.RemainCout<(RX_QUEUE_NUM-1))  
		{
			 //有队列空闲，保留最后一个随便覆盖，也使用这个队列里的数据
			ETHRxMagPara.RxBufStau[ETHRxMagPara.RecvIndex] = 1;
			ETHRxMagPara.RxBufLen[ETHRxMagPara.RecvIndex] = rec_len;
			ETHRxMagPara.RemainCout++;
			ETHRxMagPara.RecvIndex++;
			if(ETHRxMagPara.RecvIndex>=RX_QUEUE_NUM) ETHRxMagPara.RecvIndex = 0;
			R16_ETH_ERXST = (UINT16)ETHRxMagPara.RxBufAddr[ETHRxMagPara.RecvIndex]; //更新下一个接收地址
		}
		else                                                                        
		{
			//覆盖最新的包
			/************************************************************************
				所有队列都有数据时，最后一个队列是不用的，直到一个新的空闲队列产生后，
				并接收一个新的数据包才会读取这个数据。
			*************************************************************************/
			ETHRxMagPara.RxBufStau[ETHRxMagPara.RecvIndex] = 1;
			ETHRxMagPara.RxBufLen[ETHRxMagPara.RecvIndex] = rec_len;
	
		}

		//printf("recv finish:%d\r\n", rec_len);
		//lwip_pkt_handle();
		R8_ETH_EIR = RB_ETH_EIR_RXIF; //清除接收完成中断标志
	}	
	if(eth_irq_flag&RB_ETH_EIR_LINKIF)                                         
	{
		 //Link 变化中断
		phy_reg = ReadPHYRegX(PHY_BMSR);                                         //读取PHY状态寄存器
		if(phy_reg&0x04) 
		{
			//printf("link connect ok....\r\n");
			ETHTxMagPara.SendEn = 1;
			
		}
		else 
		{
			//printf("link disconnect ok....\r\n");
			ETHTxMagPara.SendEn = 0;
		}

		R8_ETH_EIR = RB_ETH_EIR_LINKIF;
	}
	if(eth_irq_flag&RB_ETH_EIR_TXIF)                                           
	{
		//printf("send finish interrupt\r\n");
		 //发送完成中断
		if(ETHTxMagPara.TxQueueCnt)   //队列里还有数据包没有发送
		{
			R16_ETH_ETXLN = ETHTxMagPara.TxBufLen[ETHTxMagPara.SendIndex];
			R16_ETH_ETXST = ETHTxMagPara.TxBufAddr[ETHTxMagPara.SendIndex];
			ETHTxMagPara.TxBufStau[ETHTxMagPara.SendIndex] = 0;  //空闲
			
			R8_ETH_ECON1 |= RB_ETH_ECON1_TXRTS;        //开始发送
			ETHTxMagPara.SendIndex++;
			if(ETHTxMagPara.SendIndex>=TX_QUEUE_NUM) ETHTxMagPara.SendIndex = 0;
			ETHTxMagPara.TxQueueCnt--;
		}                                                  //发送处理
		R8_ETH_EIR = RB_ETH_EIR_TXIF;
	}

	
	if(eth_irq_flag&(RB_ETH_EIR_TXERIF|RB_ETH_EIR_RXERIF))   //收发出错中断
	{
		eth_irq_status = R8_ETH_ESTAT;
		ETH_IRQ_ERR_Deal(eth_irq_status);  //出错处理
		//清除错误中断标志
		R8_ETH_EIR = (RB_ETH_EIR_TXERIF|RB_ETH_EIR_RXERIF);

		//发送接收出错后，重启CH579 
		NVIC_SystemReset();
	}
	
}

void phy_change(UINT8 flag)
{
	if((flag%2)==0)
		R32_ETH_MIWR=0x0004011e;
	else if(flag%2)
   	R32_ETH_MIWR=0x0000011e; 	
}
/*
 获取物理层链路状态

	1--》链路连接
	0--》链路断开

*/
UINT8 GetPhyLinkState(void)
{
	UINT16  phy_reg = 0;
	phy_reg = ReadPHYRegX(PHY_BMSR);                                         //读取PHY状态寄存器
	if(phy_reg&0x04) 
	{
//		printf("link connect ok....\r\n");
		phy_flag = 1;
		return 1;
	}
	//printf("link disconnect ok....\r\n");
	return 0;
	
}

