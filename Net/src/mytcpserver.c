

/*
 Tcp Server
 https://gitee.com/maji19971221/lwip-routine
*/


#include  "mytcpserver.h"
#include "parameter_setting.h"
#include  "mych579mac.h"
#include  "mybufque.h"




//�¿ͻ���TCP����,������ͻ��˷�������,����socket,
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
				memcpy((UINT8*)p_in_ebuf->buf,(UINT8*)(pu8data+16), copy_len);
				p_in_ebuf->buf_len = copy_len;
			}

		}
		else
		{	

			if(p_in_ebuf != NULL)
			{
				copy_len=(px->len < MQE_BUF_SIZE)?px->len:MQE_BUF_SIZE;
				memcpy((UINT8*)&p_in_ebuf->buf[p_in_ebuf->buf_len],pu8data, copy_len);
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
�������ƣ�tcp_server_recv(void *arg, struct tcp_pcb *pcb,struct pbuf *p,err_t err)
��    �ܣ�TCP���ݽ��պͷ���
���������
���������
                 pcb-->���յ����ݵ�TCP����
                 
��дʱ�䣺2013.4.25
�� д �ˣ�
ע    �⣺����һ���ص���������һ��TCP�ε����������ʱ�ᱻ����
***********************************************************************/
static err_t tcp_server_recv(void *arg, struct tcp_pcb *pcb,struct pbuf *p,err_t err)
{
	struct pbuf *px = NULL;
	e_buf_t *p_in_ebuf = NULL;
	
	

	if(p != NULL)
	{
	
		 //�����ݱ��浽TCPӦ�ò㻺����
		  px = p;
		  while(px != NULL)
		  {

				if(px == p)
				{
					//���յ�1�� ���ݰ���������ջ�����
					p_in_ebuf = get_idle_e_from_que_tail(&g_eth_buf_que);
				}

				if(p_in_ebuf != NULL)
				{
					
					if((p_in_ebuf->buf_len + px->len) <= MQE_BUF_SIZE)
					{
						memcpy((UINT8*)&p_in_ebuf->buf[p_in_ebuf->buf_len],(UINT8*)px->payload, px->len);
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

		  //���ݽ�����ɣ��ָ�ԭ�����ڴ�С,���ݽ�����ɺ󣬱�����øú���
		  tcp_recved(pcb, p->tot_len);  
		  	/* �ͷŸ�TCP�� */
		  pbuf_free(p); 
								
	}
	else
	{
		//p ΪNULL,��ʾTCP�����Ѿ��ر�

		//printf("now tcp close....\r\n");
		tcp_close(pcb); 
		
		gNewClientPcb = NULL;

	}
	
	err = ERR_OK;
	return err;
}





/***********************************************************************
�������ƣ�tcp_server_accept(void *arg, struct tcp_pcb *pcb,struct pbuf *p,err_t err)
��    �ܣ��ص�����
���������
���������
��дʱ�䣺2013.4.25
�� д �ˣ�
ע    �⣺����һ���ص���������һ�������Ѿ�����ʱ�ᱻ����
***********************************************************************/
static err_t tcp_server_accept(void *arg,struct tcp_pcb *pcb, err_t err)
{
	tcp_setprio(pcb, TCP_PRIO_MIN);    /* ���ûص��������ȼ��������ڼ�������ʱ�ر���Ҫ,�˺����������*/
	tcp_recv(pcb,tcp_server_recv); 	   /* ����TCP�ε�ʱ�Ļص����� */

	if(gNewClientPcb == NULL)
	{
		gNewClientPcb = pcb;
	}
	else
	{
		//�ϴ����ӻ�û�йر� �ȴ��ϴ����ӹر�
		//printf("wait tcp close..\r\n");
		tcp_close(pcb); 
		
	}

	//printf("new tcp client connect port:%d\r\n", pcb->remote_port);
	err = ERR_OK;
	return err;
}


/***********************************************************************
�������ƣ�TCP_server_init(void)
��    �ܣ����TCP�������ĳ�ʼ������Ҫ��ʹ��TCPͨѶ��������״̬
���������
���������
��дʱ�䣺2013.4.25
�� д �ˣ�
ע    �⣺
***********************************************************************/

static struct tcp_pcb *gServerPcb;  //�൱�ڷ���TCP SOCKET
void TCP_server_init(void)
{ 
	err_t  err;

	/*****************************************************/
	gServerPcb = tcp_new(); 								/* ����ͨ�ŵ�TCP���ƿ�(pcb) */

	if(gServerPcb != NULL)
	{
		err = tcp_bind(gServerPcb,IP_ADDR_ANY, MY_ETH_PORT); 	    /* �󶨱���IP��ַ�Ͷ˿ںţ���Ϊtcp��������  */
		if(err == ERR_OK)
		{
			gServerPcb = tcp_listen(gServerPcb); 		/* �������״̬ */
			tcp_accept(gServerPcb,tcp_server_accept); 			    /* ��������������ʱ�Ļص����� */
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

//ͨ��lwip tcp��������
err_t  MyTcpSendData(UINT8*dataBuf,     UINT16 dataSize)
{
	err_t err_ret = 0;
	err_ret = tcp_write(gNewClientPcb,(void *)dataBuf,dataSize,TCP_WRITE_FLAG_COPY);
	err_ret = tcp_output(gNewClientPcb);	

	return err_ret;
}


