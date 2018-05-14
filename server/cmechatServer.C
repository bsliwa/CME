#include <iostream>
#include <sys/types.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <netdb.h>
#include <cstring>
#include <cstdlib>
#include <thread>

#include "cmechatServer.h"
#include "../common/cmechatCommandLineParser.h"


// get_in_addr is from Beej's Guide to Network Programming
void *get_in_addr(struct sockaddr *sa)
{
    if (sa->sa_family == AF_INET) {
        return &(((struct sockaddr_in*)sa)->sin_addr);
    }

    return &(((struct sockaddr_in6*)sa)->sin6_addr);
}

cmechatServer::cmechatServer(const char* logFileName,
			int argc,
			char **argv) :
	_logger(logFileName)
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
	_node = getCmdOption(argv, argv+argc, "-h");
	if (_node)
	{
		std::string s;
		s += "User specified node ";
		s += _node;
		_logger.log(s.c_str());
	}
	else
	{
		_logger.log("-h <host> option is required. Exiting.");
		exit(1);
	}
}

void cmechatServer::openServer()
{
	int status;
	struct addrinfo hints;
	struct addrinfo *serverinfo;
	struct addrinfo *p;
	int sockfd;

	memset(&hints, 0, sizeof(hints));
	hints.ai_family = AF_INET; 		//ipv4 only
	hints.ai_socktype = SOCK_STREAM;	//tcp
	hints.ai_flags = AI_PASSIVE;		//auto fill IP
	
	status = getaddrinfo(_node, _port, &hints, &serverinfo);

	for (p = serverinfo; p != 0; p = p->ai_next)
	{
		sockfd = socket(p->ai_family, p->ai_socktype, p->ai_protocol);
		if (sockfd < 0)
		{
			_logger.log("socket fail...", true);
		}
		else
		{
			_logger.log("socket success", true);
		}

		int yes = 1;
		int ret = setsockopt(sockfd, SOL_SOCKET, SO_REUSEADDR, &yes, sizeof(yes));
		if (ret < 0)
		{
			perror("Unable to set SO_REUSEADDR.  Exiting...");
			exit(1);
		}

		ret = bind(sockfd, p->ai_addr, p->ai_addrlen);
		if (ret < 0)
		{
			perror("Unable to bind.  Exiting...");
			exit(1);
		}

		break;
	}

	freeaddrinfo(serverinfo);

	if (p == 0)
	{
		_logger.log("Unable to bind.  Exiting...");
		exit(1);
	}

	_listenFd = sockfd;
}

int runUserThread(int myFd)
{
	std::cout << "in thread" << std::endl;
	int numSent = send(myFd, "Accepted connection\0", strlen("Accepted connection")+1, 0);
	std::cout << "sent " << numSent << " bytes" << std::endl;

}

void cmechatServer::listen()
{
	int ret;
	struct sockaddr_storage clientAddr;
	char s[INET_ADDRSTRLEN];

	ret = ::listen(_listenFd, 10);  // allow backlog of 10

	while(1)
	{
		int sinSize = sizeof(clientAddr);
		int clientFd = accept(_listenFd,
						(struct sockaddr*)&clientAddr,
						(socklen_t*) &sinSize);
		if (clientFd < 0)
		{
			perror("Error with accept.  Continuing...");
			continue;
		}

		inet_ntop(clientAddr.ss_family,
				get_in_addr((struct sockaddr *)&clientAddr),
				s,
				sizeof(s));

		{
			std::string logme;
			logme = "Accepted connection from ";
			logme += s;
			_logger.log(logme.c_str());
		}

std::cout << "creatign thread" << std::endl;
		std::thread newThread;
		newThread = std::thread(runUserThread, clientFd);
		newThread.join();
std::cout << "after thread" << std::endl;
		

	}
}
