

/*
 Tcp Server 服务器代码
*/


#include  "mytcpserver.h"
#include "parameter_setting.h"
#include  "mych579mac.h"
#include  "mybufque.h"




//新客户端TCP连接,用于向客户端发送数据,类似socket,
static struct tcp_pcb *gNewClientPcb = NULL;


err_t  raw_mac_recv(struct pbuf *p, struct netif *netif)
{
	u8_t *pu8data = NULL;
	struct pbuf *px = NULL;

	e_buf_t *p_in_ebuf = NULL;
	
	u16_t curDataLen = 0;
	u16_t copy_len = 0;

	px  = p;
	while(px != NULL)
	{
		pu8data = (u8_t *)px->payload;

		if(px == p)
		{
			p_in_ebuf = get_idle_e_from_que_tail(&g_eth_buf_que);

			if(p_in_ebuf != NULL)
			{
				memcpy(gMyPeerMac, &pu8data[6],6);
				
				curDataLen = px->len - 16;

				copy_len=(curDataLen < MQE_BUF_SIZE)?curDataLen:MQE_BUF_SIZE;
				memcpy((u8 *)p_in_ebuf->buf,(u8 *)(pu8data+16), copy_len);
				p_in_ebuf->buf_len = copy_len;
			}

		}
		else
		{	

			if(p_in_ebuf != NULL)
			{
				copy_len=(px->len < MQE_BUF_SIZE)?px->len:MQE_BUF_SIZE;
				memcpy((u8 *)&p_in_ebuf->buf[p_in_ebuf->buf_len],pu8data, copy_len);
				p_in_ebuf->buf_len += copy_len;
			}

		}

		px = px->next;
		
	}

	g_cur_eth_comm_chan = PROT_DATA_FROM_MAC;
	e_buf_num_add_one(&g_eth_buf_que);
	

	
	return ERR_OK;
}



/***********************************************************************
函数名称：tcp_server_recv(void *arg, struct tcp_pcb *pcb,struct pbuf *p,err_t err)
功    能：TCP数据接收和发送
输入参数：
输出参数：
                 pcb-->接收到数据的TCP连接
                 
编写时间：2013.4.25
编 写 人：
注    意：这是一个回调函数，当一个TCP段到达这个连接时会被调用
***********************************************************************/
static err_t tcp_server_recv(void *arg, struct tcp_pcb *pcb,struct pbuf *p,err_t err)
{
	struct pbuf *px = NULL;
	e_buf_t *p_in_ebuf = NULL;
	
	

	if(p != NULL)
	{
	
		 //将数据保存到TCP应用层缓冲区
		  px = p;
		  while(px != NULL)
		  {

				if(px == p)
				{
					//接收第1个 数据包，申请接收缓冲区
					p_in_ebuf = get_idle_e_from_que_tail(&g_eth_buf_que);
				}

				if(p_in_ebuf != NULL)
				{
					
					if((p_in_ebuf->buf_len + px->len) <= MQE_BUF_SIZE)
					{
						memcpy((u8 *)&p_in_ebuf->buf[p_in_ebuf->buf_len],(u8 *)px->payload, px->len);
						p_in_ebuf->buf_len += px->len;
					}
					
				
				}
				else
				{
					printf("p_in_ebuf is null\r\n");
					break;
				}
			
			 	px=px->next;
				
		  }

		  g_cur_eth_comm_chan = PROT_DATA_FROM_TCP;
		  e_buf_num_add_one(&g_eth_buf_que);

		  //数据接收完成，恢复原来窗口大小,数据接收完成后，必须调用该函数
		  tcp_recved(pcb, p->tot_len);  
		  	/* 释放该TCP段 */
		  pbuf_free(p); 
								
	}
	else
	{
		//p 为NULL,表示TCP连接已经关闭

		//printf("now tcp close....\r\n");
		tcp_close(pcb); 
		
		gNewClientPcb = NULL;

	}
	
	err = ERR_OK;
	return err;
}





/***********************************************************************
函数名称：tcp_server_accept(void *arg, struct tcp_pcb *pcb,struct pbuf *p,err_t err)
功    能：回调函数
输入参数：
输出参数：
编写时间：2013.4.25
编 写 人：
注    意：这是一个回调函数，当一个连接已经接受时会被调用
***********************************************************************/
static err_t tcp_server_accept(void *arg,struct tcp_pcb *pcb, err_t err)
{
	tcp_setprio(pcb, TCP_PRIO_MIN);    /* 设置回调函数优先级，当存在几个连接时特别重要,此函数必须调用*/
	tcp_recv(pcb,tcp_server_recv); 	   /* 设置TCP段到时的回调函数 */

	if(gNewClientPcb == NULL)
	{
		gNewClientPcb = pcb;
	}
	else
	{
		//上次连接还没有关闭 等待上次连接关闭
		//printf("wait tcp close..\r\n");
		tcp_close(pcb); 
		
	}

	//printf("new tcp client connect port:%d\r\n", pcb->remote_port);
	err = ERR_OK;
	return err;
}


/***********************************************************************
函数名称：TCP_server_init(void)
功    能：完成TCP服务器的初始化，主要是使得TCP通讯快进入监听状态
输入参数：
输出参数：
编写时间：2013.4.25
编 写 人：
注    意：
***********************************************************************/

static struct tcp_pcb *gServerPcb;  //相当于服务TCP SOCKET
void TCP_server_init(void)
{ 
	err_t  err;

	/*****************************************************/
	gServerPcb = tcp_new(); 								/* 建立通信的TCP控制块(pcb) */

	if(gServerPcb != NULL)
	{
		err = tcp_bind(gServerPcb,IP_ADDR_ANY, MY_ETH_PORT); 	    /* 绑定本地IP地址和端口号（作为tcp服务器）  */
		if(err == ERR_OK)
		{
			gServerPcb = tcp_listen(gServerPcb); 		/* 进入监听状态 */
			tcp_accept(gServerPcb,tcp_server_accept); 			    /* 设置有连接请求时的回调函数 */
		}
		else
		{
			printf("tcp_bind error\r\n");
		}
	}
	else
	{
		printf("tcp_new error\r\n");
	}
						
}

//通过lwip tcp发送数据
err_t  MyTcpSendData(u8 *dataBuf,     u16 dataSize)
{
	err_t err_ret = 0;
	err_ret = tcp_write(gNewClientPcb,(void *)dataBuf,dataSize,TCP_WRITE_FLAG_COPY);
	err_ret = tcp_output(gNewClientPcb);	

	return err_ret;
}


