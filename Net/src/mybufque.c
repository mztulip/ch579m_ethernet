#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "parameter_setting.h"
#include "mybufque.h"

my_buf_que_t  g_eth_buf_que;   

void  init_my_buf_que(my_buf_que_t *p_my_buf_que)
{
	memset((UINT8 *)p_my_buf_que,0, sizeof(my_buf_que_t));
}


e_buf_t  *get_valid_e_from_que_head(my_buf_que_t *p_my_buf_que)
{
	e_buf_t *p_ebuf=NULL;

	UINT8 head_index = p_my_buf_que->head_index;
	
	if(p_my_buf_que->e_buf_num > 0 )
	{
		p_ebuf = &p_my_buf_que->e_buf[head_index];
	}
	return p_ebuf;
}


e_buf_t  *get_idle_e_from_que_tail(my_buf_que_t *p_my_buf_que)
{
	e_buf_t *p_ebuf=NULL;

	UINT8 tail_index = p_my_buf_que->tail_index;
	
	if(p_my_buf_que->e_buf_num < BQE_SIZE)
	{
	
		p_ebuf = &p_my_buf_que->e_buf[tail_index];
		p_my_buf_que->tail_index++;

		//head_index ȡֵ��Χ: 0 ~ BQE_SIZE-1
		if(p_my_buf_que->tail_index >= BQE_SIZE )
		{
			p_my_buf_que->tail_index = 0;
		}
	}

	return p_ebuf;
}

void   e_buf_num_add_one(my_buf_que_t *p_my_buf_que)
{
	p_my_buf_que->e_buf_num++;
}

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

