
/*
 TCP  server head file
*/

#ifndef  	__MY_TCP_SERVER_H__
#define 	__MY_TCP_SERVER_H__

#include 	"sys.h"
#include 	"stdio.h"
#include 	"string.h" 
#include "parameter_setting.h"
#include 	"lwip/debug.h"
#include 	"lwip/stats.h"
#include 	"lwip/tcp.h"
#include 	"lwip/memp.h"
#include 	"lwip/mem.h"



#define   __ECHO_TCP_SERVER__    		0

extern void   TCP_server_init(void);
extern err_t  MyTcpSendData(u8 *dataBuf,     u16 dataSize);
extern err_t  MyMacSendData(u8 *dataBuf,     u16 dataSize);



#endif

