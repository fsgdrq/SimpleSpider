/*
 * =====================================================================================
 *
 *       Filename:  common.h
 *
 *    Description:  A common struct of my program
 *
 *        Version:  1.0
 *        Created:  2014年10月31日 11时36分10秒
 *       Revision:  none
 *       Compiler:  gcc
 *
 *         Author:  qhsong (), 
 *   Organization:  
 *
 * =====================================================================================
 */
#ifndef COMMON_H
#define COMMON_H
#include<event2/event.h>
#include<event2/bufferevent.h>
#include<event2/buffer.h>
#include<event2/thread.h>
#include<errno.h>
#include<event2/util.h>

#include<nanomsg/nn.h>
#include<nanomsg/pair.h>
#include<pthread.h>
#include<malloc.h>


#include "trie.h"

typedef struct url_rsp_s{
	int size;
	char *url[300];
}URL_RSP;

typedef struct url_req_s{
	char *url;
	struct evbuffer *html;
}URL_REQ;

typedef struct thread_param_s{
	TRIE **head;
	pthread_mutex_t *send;
	pthread_mutex_t *recv;
	int sock;
}THREAD_PARM;

typedef struct http_response_s{
	int status;
	int http_status_code;
	int clength;
	int nowlength;
	int conn;
	int ihead;
	char base_url[1024];
	struct evbuffer *html;
}HTTP_RES;

typedef struct start_point_st{
	char ip[16];
	int port;
	char s_add[1024];
}START_POINT;

typedef struct connser_thread_s{
	START_POINT *s;
	FILE *wr_file;	
}CONNSER_THREAD;

typedef struct event_parm_s{
	HTTP_RES *t;
	START_POINT *st;
	struct event_base *base; 
	int sock;
	FILE *wr_file;
}EVENT_PARM;

#define END_ADDRESS "inproc://spider"
#define GLOBAL_BASE_URL "tech.qq.com"
#define LEN_GLOBAL_BASE_URL 11
#define THREAD_NUM 1

#endif
