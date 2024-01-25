

/****************************************************************
CH579 MAC通信头文件

*****************************************************************/
#ifndef   __MY_CH579_MAC_H__
#define   __MY_CH579_MAC_H__


#include <stdio.h>
#include <string.h>

#include "CH579SFR.h"
#include "core_cm0.h"
#include "CH57x_common.h"


extern  UINT8   myCh579MAC[];
extern u8 phy_flag;
//MAC地址
#define R8_ETH_MAADR1           (*((PUINT8V)(ETH_BASE_ADDR+0x28)))              /* RW MAC Address Byte1 */
#define R8_ETH_MAADR2           (*((PUINT8V)(ETH_BASE_ADDR+0x29)))              /* RW MAC Address Byte2 */
#define R8_ETH_MAADR3           (*((PUINT8V)(ETH_BASE_ADDR+0x2a)))              /* RW MAC Address Byte3 */
#define R8_ETH_MAADR4           (*((PUINT8V)(ETH_BASE_ADDR+0x2b)))              /* RW MAC Address Byte4 */
#define R8_ETH_MAADR5           (*((PUINT8V)(ETH_BASE_ADDR+0x2c)))              /* RW MAC Address Byte5 */
#define R8_ETH_MAADR6           (*((PUINT8V)(ETH_BASE_ADDR+0x2d))) 


#define  RB_ETH_MIREGADR_MIIWR  	0x20        /* WO MII 写命令 */
#define  RB_ETH_MIREGADR_MIRDL  	0x1f  


//PHY地址 
#define PHY_BMCR                0x00                 /* Control Register */
#define PHY_BMSR                0x01                 /* Status Register */
#define PHY_ANAR                0x04                 /* Auto-Negotiation Advertisement Register */
#define PHY_ANLPAR              0x05                 /* Auto-Negotiation Link Partner Base  Page Ability Register*/
#define PHY_ANER                0x06                 /* Auto-Negotiation Expansion Register */
#define PHY_MDIX                0x1e                 /* 自定义 MDIX 模式寄存器 */
//自定义MDIX模式寄存器  @PHY_MDIX
#define PN_NORMAL               0x04                 /* 模拟p，n极性选择 */
#define MDIX_MODE_MASK          0x03                 /* mdix设置 */
#define MDIX_MODE_AUTO          0x00                 /*  */
#define MDIX_MODE_MDIX          0x01
#define MDIX_MODE_MDI           0x02
//ECON2测试模式，待定  
#define RX_VCM_MODE_0
#define RX_VCM_MODE_1
#define RX_VCM_MODE_2
#define RX_VCM_MODE_3
//RX 参考电压值 设置  @RX_REF
#define RX_REF_25mV             (0<<2)              /* 25mV */
#define RX_REF_49mV             (1<<2)              /* 49mV */
#define RX_REF_74mV             (2<<2)              /* 74mV */
#define RX_REF_98mV             (3<<2)              /* 98mV */
#define RX_REF_123mV            (4<<2)              /* 123mV */
#define RX_REF_148mV            (5<<2)              /* 148mV */
#define RX_REF_173mV            (6<<2)              /* 173mV */
#define RX_REF_198mV            (7<<2)              /* 198mV */
//TX DRIVER 偏置电流  @TX_AMP
#define TX_AMP_0                (0<<0)              /* 43mA   / 14.5mA   (1.4V/0.7V) */
#define TX_AMP_1                (1<<0)              /* 53.1mA / 18mA     (1.8V/0.9V) */
#define TX_AMP_2                (2<<0)              /* 75.6mA / 25.6mA   (2.6V/1.3V) */
#define TX_AMP_3                (3<<0)                                          /* 122mA  / 41.45mA  (4.1V/2.3V) */
//FCEN暂停帧控制      @FCEN
#define FCEN_0_TIMER            (3<<4)              /* 发送 0 timer 暂停帧，然后停止发送 */
#define FCEN_CYCLE              (2<<4)              /* 周期性发送暂停帧 */
#define FCEN_ONCE               (1<<4)              /* 发送一次暂停帧，然后停止发送 */
#define FCEN_STOP               (0<<4)              /* 停止发送暂停帧 */
//PADCFG短包控制  @PADCFG
#define PADCFG_AUTO_0           (7<<5)              /* 所有短包填充00h至64字节，再4字节crc */
#define PADCFG_NO_ACT_0         (6<<5)              /* 不填充短包 */
#define PADCFG_DETE_AUTO        (5<<5)             /* 检测到字段为8100h的VLAN网络包自动填充00h至64字节，否则短包填充60字节0，填充后再4字节crc */
#define PADCFG_NO_ACT_1         (4<<5)             /* 不填充短包 */
#define PADCFG_AUTO_1           (3<<5)             /* 同111 */
#define PADCFG_NO_ACT_2         (2<<5)             /* 不填充短包 */
#define PADCFG_AUTO_3           (1<<5)             /* 所有短包填充00h至60字节，再4字节crc */
#define PADCFG_NO_ACT_3         (0<<5)            /* 不填充短包 */


//网络收发MAC层配置 1536
#define ETHER_HEAD_LEN          14                 /* 网络帧头长度 */
#define MACADDR_LEN             6
#define MAC_MAX_LEN             1064          /* MAC层最大包长度   576     1536 */
#define RX_MAX_BUF_LEN          1064          /* 最大接收包数据长度 */
#define RX_BUF_SIZE             1064          /* 接收缓存大小 */
#define TX_BUF_SIZE             1064          /* 发送缓存大小 */
#define RX_QUEUE_NUM            3            /* 接收缓冲区队列数 */
#define TX_QUEUE_NUM            2          


#define  MAX_FRAMELEN			(MAC_MAX_LEN-18)


//收发管理定义
typedef __packed struct _RXBUFST
{
    UINT8  RecvEn;                                  /* 使能控制 */
    UINT8  RecvIndex;                              /* MAC层接收缓冲区的索引 */
    UINT8  ReadIndex;                              /* 读取缓冲区的索引值 */
    UINT8  RemainCout;                          /* 剩余未读取的数据包 */
    UINT8  RxBufStau[RX_QUEUE_NUM];            /* 接收缓冲区的状态 */
    UINT16 RxBufLen[RX_QUEUE_NUM];            /* 接收缓冲区数据的有效长度 */
    UINT32 RxBufAddr[RX_QUEUE_NUM];           /* 接收缓冲区的起始地址 */
}RXBUFST;

typedef __packed struct _TXBUFST
{
    UINT8  SendEn;                         /* 使能控制 */
    UINT8  SendIndex;                      /* MAC层发送缓冲区的索引 */
    UINT8  WriteIndex;                     /* 发送缓冲区空闲的索引值 */
    UINT8  TxQueueCnt;                     /* 发送队列中排队的数量 */
    UINT8  TxBufStau[TX_QUEUE_NUM];         /* 发送缓冲区的状态 */
    UINT16 TxBufLen[TX_QUEUE_NUM];         /* 发送缓冲区数据的有效长度 */
    UINT32 TxBufAddr[TX_QUEUE_NUM];        /* 发送缓冲区的起始地址 */
}TXBUFST;



extern void    ETHInitX(void);
extern void    ETHParaInitX(void);
extern UINT16  ETHRecX(UINT8     *pRecvBuf, UINT16 RecvBufSize);
extern UINT8   ETHSendX(UINT8 *p_send_buf, UINT16 send_len);
extern void    ETH_IRQHandler(void);
extern UINT16  GetRecvMacDataLen(void);
extern UINT8   GetPhyLinkState(void);

extern void phy_change(UINT8 flag);
#endif 

