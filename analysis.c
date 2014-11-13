/*
 * =====================================================================================
 *
 *       Filename:  analysis.c
 *
 *    Description:  
 *
 *        Version:  1.0
 *        Created:  2014年10月26日 18时50分32秒
 *       Revision:  none
 *       Compiler:  gcc
 *
 *         Author:  qhsong (), 
 *   Organization:  
 *
 * =====================================================================================
 */
#include<assert.h>
#include<malloc.h>
#include<stdlib.h>
#include "analysis.h"
#include "common.h"

void analy_run(void *arg){
	char *buf=NULL,*index=NULL;
	int bytes=0,totalbytes=0;
	int count=0;
	URL_REQ *msg;
	THREAD_PARM *parm;
	parm = (THREAD_PARM *)arg;
	TRIE **t = parm->head;
	int sock = parm->sock;
	while(1){
	////	pthread_mutex_lock(parm->send);
		bytes = nn_recv(sock,&index,sizeof(URL_REQ *),0);
		msg=(URL_REQ *)(index);
		analy(msg->url,msg->html,t,sock,parm->recv);
	}
		//free(msg->url);
		//free(msg->html);
		//free(msg);
	return;
}

int analy(char *url,const char* html,TRIE **head,int nn_sock,pthread_mutex_t *mutex){
	char *outurl;
	int status = STATUS_0;
	int i = 0,j = 0;
	char temp[100];
	int pos=0;
	trie_add(head,url);
	while(html[i]){
		switch(status) {
		case STATUS_0:
			j = i;
			while(html[j]!='<' && html[j]!='\0') j++;
			if(html[j]=='\0'){ 
				sendurl(rsp,nn_sock);
				//pthread_mutex_unlock(mutex);
				return 0;
			}
			i = j+1;
			status = STATUS_1;
			break;
		case STATUS_1:
			if(html[i]=='a'){
				status = STATUS_2;
			}else{
				status = STATUS_0;
			}
			i++;
			break;
		case STATUS_2:
			while(1){
				if(!strncmp(html+i,"href",LEN_HREF)){	//href
					status = STATUS_3;
					i += LEN_HREF;
					break;	//break while(1)
				}else if(html[i] == '>'){
					i++;
					status = STATUS_0;
					break; //break while(1)
				}else{
					i++;
				}
			}	
			break;
		case STATUS_3:
			while(html[i]==' ') i++;
			if(html[i]=='='){
				status=STATUS_4;			
			}else{
				status = STATUS_2;
			}
			i++;
			break;
		case STATUS_4:
			while(html[i]==' ') i++;
			if(html[i]=='"'){
				status = STATUS_5;
			}else{
				status = STATUS_0;
			}
			i++;
			break;
		case STATUS_5:
			while(html[i]==' ') i++;
			if(html[i]=='#'||html[i]=='>'||html[i]=='"'){
				status = STATUS_0;
				i++;
			}else if(!strncmp(html+i,"javascript",LEN_JAVASCRIPT)){
				status = STATUS_0;
			}else{
				pos=0;
				temp[pos++]=html[i++];
				status = STATUS_6;
			}
			break;
		case STATUS_6:
			while(html[i]==' ') i++;
			while(1){
				if(html[i]=='#'||html[i]=='>'){
					status = STATUS_0;
					i++;
				}else if(html[i]=='"'){
					status = STATUS_7;
					break;
				}else{
					temp[pos++] = html[i++];
				}
			}
			break;
		case STATUS_7:
				temp[pos]=0;
				//printf("%s\n",temp);
				outurl = trans(url,temp);
			//	trans("http://192.168.0.1/a/b/c/index.html","f.html");
				if(outurl) { 
					if(!trie_check(head,outurl)){
						trie_add(head,outurl);
						//isendurl(outurl,nn_sock);
						while(nn_send(nn_sock,output,sizeof(char *),NN_DONTWAIT)==EAGAIN);
					//	printf("URL:%s\n",outurl);
					}
				}
				status = STATUS_0;
			break;
		}
	}	
}

char* trans(char *baseurl,char *url) {
	char *out = NULL,*str = url,*burl = baseurl,*index;
	if(!strncmp(str,"http://",LEN_HTTPFLAG)){
		str += LEN_HTTPFLAG;
		if(!strncmp(str,GLOBAL_BASE_URL,LEN_GLOBAL_BASE_URL)){
			index = out = (char *)malloc(1024);
			//strcat(out,"http://");
			//index += LEN_HTTPFLAG;
			*(index++)='/';
			//burl += LEN_HTTPFLAG;
			//while((*index++=*burl++)!='/');
			while(*str++!='/');
			while(*index++=*str++);
			*index = '\0';
		}else{
			//printf("Out side address:%s\n",url);
			return NULL;
		}
	}else if(!strncmp(str,"..",2)){
		char *end = burl;
		index = out = (char *)malloc(1024);
		while(*end++);
		while(*(--end)!='/');
		while(*str=='.'&&str[1]=='.'&& end != burl){
			while((*--end)!='/');
			str += 3;	//"../"length
		}
		if(end == (burl+6)) {	// http:/(/) if end point (/) then exit it.
			printf("Relative address error!%s",url);
			return NULL;	//address error
		}else{
			*(index++) = '/';
			burl += LEN_HTTPFLAG;
			while(*(burl++) != '/');
			while(burl!=end){
				*index = *burl;
				burl++;
				index++;
			}
			*index='/';
			index++;
			while(*str){
				*index = *str;
				str++;
				index++;
			}
		}
	}else if(*str=='/'){
		index = out = (char *)malloc(1024);
		//strcpy(out,"http://");
		//burl += LEN_HTTPFLAG;
		//index += LEN_HTTPFLAG;
		//while(*burl != '/'){
		//	*index = *burl;
		//	index++;
		//	burl++;
		//}
		while(*str){
			*index = *str;
			str++;
			index++;
		}
	}else{
		char *end = burl;
		index = out = (char *)malloc(1024);
		*index++ = '/';
		while(*end++);
		while(*(--end)!='/');
		end++;
		burl += LEN_HTTPFLAG;
		while(*(burl++)!='/');
		while(burl!=end){
			*index++ = *burl++;
		}
		while(*index++ = *str++);
	}
	return out;
}

int sendurl(URL_RSP *rsp,int nn_sock){
}
