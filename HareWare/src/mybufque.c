

/*************************************************
  �Լ�ʵ�ֵĻ���������

  ����һ�����ζ��У����е�ÿ��Ԫ����һ��������
  
**************************************************/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "parameter_setting.h"
#include "mybufque.h"


my_buf_que_t  g_eth_buf_que;   //�Ķ������ڻ���������


//��ʼ��һ���ջ���������
void  init_my_buf_que(my_buf_que_t *p_my_buf_que)
{
	memset((u8 *)p_my_buf_que,0, sizeof(my_buf_que_t));
}

/*
  �Ӷ�ͷ��ȡһ����Ч�Ķ���Ԫ�أ��ɹ����ض�ͷԪ��ָ��
  p_my_buf_que--��ָ���Ķ���
  
*/
e_buf_t  *get_valid_e_from_que_head(my_buf_que_t *p_my_buf_que)
{
	e_buf_t *p_ebuf=NULL;

	u8 head_index = p_my_buf_que->head_index;
	
	if(p_my_buf_que->e_buf_num > 0 )
	{
		//��������Ч�Ķ���Ԫ��
		p_ebuf = &p_my_buf_que->e_buf[head_index];
		
	}

	//û����Ч����Ԫ�أ�����NULL
	return p_ebuf;
}


/*
  �Ӷ�β��ȡһ����Ч(����)�Ķ���Ԫ�أ��ɹ����ض�βԪ��ָ��
  p_my_buf_que--��ָ���Ķ���
  
*/
e_buf_t  *get_idle_e_from_que_tail(my_buf_que_t *p_my_buf_que)
{
	e_buf_t *p_ebuf=NULL;

	u8 tail_index = p_my_buf_que->tail_index;
	
	if(p_my_buf_que->e_buf_num < BQE_SIZE)
	{
		//�����п��еĶ���Ԫ��
		p_ebuf = &p_my_buf_que->e_buf[tail_index];
		p_my_buf_que->tail_index++;

		//head_index ȡֵ��Χ: 0 ~ BQE_SIZE-1
		if(p_my_buf_que->tail_index >= BQE_SIZE )
		{
			p_my_buf_que->tail_index = 0;
		}
	}

	//û�п��еĶ���Ԫ�أ�����NULL
	return p_ebuf;
}


//������ЧԪ�ظ�����1
void   e_buf_num_add_one(my_buf_que_t *p_my_buf_que)
{
	p_my_buf_que->e_buf_num++;
}


//������ЧԪ�ظ�����1
void   e_buf_num_sub_one(my_buf_que_t *p_my_buf_que)
{
	p_my_buf_que->e_buf_num--;
	p_my_buf_que->head_index++;

	//head_index ȡֵ��Χ: 0 ~ BQE_SIZE-1
	if(p_my_buf_que->head_index >= BQE_SIZE )
	{
		p_my_buf_que->head_index = 0;
	}
}

