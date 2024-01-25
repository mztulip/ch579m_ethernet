

/*************************************************
  自己实现的缓冲区队列

  定义一个环形队列，队列的每个元素是一个缓冲区
  
**************************************************/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "parameter_setting.h"
#include "mybufque.h"


my_buf_que_t  g_eth_buf_que;   //的定义网口缓冲区队列


//初始化一个空缓冲区队列
void  init_my_buf_que(my_buf_que_t *p_my_buf_que)
{
	memset((u8 *)p_my_buf_que,0, sizeof(my_buf_que_t));
}

/*
  从队头获取一个有效的队列元素，成功返回队头元素指针
  p_my_buf_que--》指定的队列
  
*/
e_buf_t  *get_valid_e_from_que_head(my_buf_que_t *p_my_buf_que)
{
	e_buf_t *p_ebuf=NULL;

	u8 head_index = p_my_buf_que->head_index;
	
	if(p_my_buf_que->e_buf_num > 0 )
	{
		//队列有有效的队列元素
		p_ebuf = &p_my_buf_que->e_buf[head_index];
		
	}

	//没有有效队列元素，返回NULL
	return p_ebuf;
}


/*
  从队尾获取一个无效(空闲)的队列元素，成功返回队尾元素指针
  p_my_buf_que--》指定的队列
  
*/
e_buf_t  *get_idle_e_from_que_tail(my_buf_que_t *p_my_buf_que)
{
	e_buf_t *p_ebuf=NULL;

	u8 tail_index = p_my_buf_que->tail_index;
	
	if(p_my_buf_que->e_buf_num < BQE_SIZE)
	{
		//队列有空闲的队列元素
		p_ebuf = &p_my_buf_que->e_buf[tail_index];
		p_my_buf_que->tail_index++;

		//head_index 取值范围: 0 ~ BQE_SIZE-1
		if(p_my_buf_que->tail_index >= BQE_SIZE )
		{
			p_my_buf_que->tail_index = 0;
		}
	}

	//没有空闲的队列元素，返回NULL
	return p_ebuf;
}


//队列有效元素个数加1
void   e_buf_num_add_one(my_buf_que_t *p_my_buf_que)
{
	p_my_buf_que->e_buf_num++;
}


//队列有效元素个数减1
void   e_buf_num_sub_one(my_buf_que_t *p_my_buf_que)
{
	p_my_buf_que->e_buf_num--;
	p_my_buf_que->head_index++;

	//head_index 取值范围: 0 ~ BQE_SIZE-1
	if(p_my_buf_que->head_index >= BQE_SIZE )
	{
		p_my_buf_que->head_index = 0;
	}
}

