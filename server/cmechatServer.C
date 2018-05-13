#include <iostream>
#include <sys/types.h>
#include <sys/socket.h>
#include <netdb.h>
#include <cstring>


#include "cmechatServer.h"

cmechatServer::cmechatServer(const char* logFileName) :
	_logger(logFileName)
{
}

int openServer()
{
	int status;
	struct addrinfo hints;
	struct addrinfo *serverinfo;

	memset(&hints, 0, sizeof(hints));
	hints.ai_family = AF_INET; 		//ipv4 only
	hints.ai_socktype = SOCK_STREAM;	//tcp
	hints.ai_flags = AI_PASSIVE;		//auto fill IP
	
	status = getaddrinfo(0, "14002", &hints, &serverinfo);
}
