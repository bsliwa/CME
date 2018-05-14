#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <errno.h>
#include <string.h>
#include <netdb.h>
#include <sys/types.h>
#include <netinet/in.h>
#include <sys/socket.h>
#include <arpa/inet.h>

#include "cmechatClient.h"
#include "../common/cmechatCommandLineParser.h"

// get_in_addr is from Beej's Guide to Network Programming
void *get_in_addr(struct sockaddr *sa)
{
    if (sa->sa_family == AF_INET) {
        return &(((struct sockaddr_in*)sa)->sin_addr);
    }

    return &(((struct sockaddr_in6*)sa)->sin6_addr);
}

cmechatClient::cmechatClient(const char* logFileName,
                                int argc,
                                char **argv) :
                            _logger(logFileName),
                            _myFd(0)
{
    bool debug = cmdOptionExists(argv, argv+argc, "-d");
	_logger.setDebugMode(debug);

	// read the port from command line
	_port = getCmdOption(argv, argv+argc, "-p");
	if (_port)
	{
		std::string s;
		s += "User specified port ";
		s += _port;
		_logger.log(s.c_str());
	}
	else
	{
		_logger.log("-p <port> option is required. Exiting.");
		exit(1);
	}

	// read the ip from command line
	_host = getCmdOption(argv, argv+argc, "-h");
	if (_host)
	{
		std::string s;
		s += "User specified host ";
		s += _host;
		_logger.log(s.c_str());
	}
	else
	{
		_logger.log("-h <host> option is required. Exiting.");
		exit(1);
	}
}

void cmechatClient::connectToServer()
{
    int ret;
    struct addrinfo hints;
    struct addrinfo *serverinfo;
    struct addrinfo *p;
   	char s[INET_ADDRSTRLEN];
    int mysock;

    memset(&hints, 0, sizeof(hints));
    hints.ai_family = AF_INET;
    hints.ai_socktype = SOCK_STREAM;

    ret = getaddrinfo(_host, _port, &hints, &serverinfo);
    if (ret < 0)
    {
        perror("getaddrinfo failed.  Exiting...");
        exit(1);
    }
    for (p = serverinfo; p != 0; p = p->ai_next)
    {
        mysock = socket(p->ai_family, 
            p->ai_socktype,
            p->ai_protocol);
       if(mysock < 0)
        {
            _logger.log("Call to socket failed.  Continuing...");
        }
        
        ret = connect(mysock, p->ai_addr, p->ai_addrlen);
 
        if (ret < 0)
        {
            close(mysock);
            _logger.log("Connect failed.  Continuing...");
            continue;
        }

        break;
    }

    if (p == 0)
    {
        fprintf(stdout, "Could not connect to server at %s port %s", _host, _port);
        exit(1);
    }

    inet_ntop(p->ai_family,
            get_in_addr((struct sockaddr*)p->ai_addr),
            s, 
            sizeof(s));

    freeaddrinfo(serverinfo);
    
    {
        std::string logme;
        logme = "Connected to ";
        logme += s;
        _logger.log(logme.c_str());
    }

    _myFd = mysock;
}