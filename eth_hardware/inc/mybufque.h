

#ifndef   __MY_BUF_QUE_H__
#define   __MY_BUF_QUE_H__

#include "parameter_setting.h"

#define   MQE_BUF_SIZE    1064 //1088

#define   BQE_SIZE     2 


struct  _e_buf
{
	UINT8    buf[MQE_BUF_SIZE];  
	UINT16   buf_len;  
	UINT32   timestamp; 
	//UINT8    data_source; 
};

typedef struct  _e_buf   e_buf_t;


typedef struct  _my_buf_que
{
	e_buf_t  e_buf[BQE_SIZE];   

	UINT8  head_index;  
	UINT8  tail_index;  

	UINT8  e_buf_num; 
}my_buf_que_t;


extern my_buf_que_t  g_eth_buf_que;

extern  void     init_my_buf_que(my_buf_que_t *p_my_buf_que);
extern  e_buf_t  *get_valid_e_from_que_head(my_buf_que_t *p_my_buf_que);
extern  e_buf_t  *get_idle_e_from_que_tail(my_buf_que_t *p_my_buf_que);
extern  void     e_buf_num_add_one(my_buf_que_t *p_my_buf_que);
extern  void     e_buf_num_sub_one(my_buf_que_t *p_my_buf_que);

#endif 


