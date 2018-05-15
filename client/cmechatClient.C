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
#include <algorithm>

#include "cmechatClient.h"
#include "../common/cmechatCommandLineParser.h"
#include "../common/cmechatInterface.h"

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
        fprintf(stdout, "Could not connect to server at %s port %s\n", _host, _port);
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

void cmechatClient::registerUsername()
{
    struct cmechatMessageNewuser msg;
    
    memset(&msg, 0, sizeof(msg));
    msg.opcode = CMECHAT_OPCODE_NEWUSER;
    strcpy(&msg.username[0], _username.c_str());
    send(_myFd, &msg, sizeof(msg), 0); 
}

void cmechatClient::getUsername()
{
    std::string username;
    std::cout << "Welcome to CMEChat.  Please enter a username: ";
    
    getline(std::cin, username);

    while (username.length() > MAX_USERNAME_LEN)
    {
        std::cout << "Sorry, the maximum length of a username is " 
            << MAX_USERNAME_LEN 
            << " characters.  Please enter a shorter name: ";
        std::cin >> username;
    }

    std::cout << "Welcome " 
        << username 
        << "!"
        << std::endl;

    std::string logme;
    logme = "Username is ";
    logme += username;
    _logger.log(logme);

    _username = username;
}

void cmechatClient::sendUserName()
{
    struct cmechatMessageNewuser newUserMsg;
    unsigned int msgLen=0;

    newUserMsg.opcode = CMECHAT_OPCODE_NEWUSER;           msgLen+= sizeof(newUserMsg.opcode);
    strcpy(newUserMsg.username, this->_username.c_str()); msgLen += strlen(newUserMsg.username)+1;

    int sentBytes = send(_myFd, (void *)&newUserMsg, msgLen, 0);
    if (sentBytes < 0)
    {
        std::cout << "Could not send username to server.  Exiting..." << std::endl;
        exit(1);
    }
}

void cmechatClient::decodeMsg(char *msg, int numRx)
{
    if (msg[0] == CMECHAT_OPCODE_BROADCAST_MESSAGE)
    {
        struct cmechatMessageBroadcastMessage *bcast = (struct cmechatMessageBroadcastMessage*)msg;
        std::string sentFromUser = ((struct cmechatMessageBroadcastMessage*)(msg))->sourceUsername;
        // origin is another user.  print the message
        if (sentFromUser != _username)
        {
             std::cout << "Received message from: " << sentFromUser << "-->" << bcast->body << std::endl;
        }
    }

}

void cmechatClient::parseUserInput(std::string &usermsg)
{
    struct cmechatMessageBroadcastMessage msg;
    int msglen;

    if (usermsg == "1")
    {
        std::cout << "Enter message: " ;
        std::string bcastmsg;
        getline(std::cin, bcastmsg);

        msg.opcode = CMECHAT_OPCODE_BROADCAST_MESSAGE; msglen += sizeof(msg.opcode);
        strcpy(msg.sourceUsername, _username.c_str()); msglen += sizeof(msg.sourceUsername);
        strcpy(msg.body, bcastmsg.c_str()); 	       msglen += bcastmsg.length()+1;
        msg.body[bcastmsg.length()] = '\0';
        int sentBytes = send(_myFd, (char*)&msg, msglen, 0);
    }
}

void cmechatClient::runChat()
{
    std::string userMessage;
    fd_set readSet;
    ssize_t n;
    char readArr[MAX_MSG_LEN];
    int maxFd;
    std::string usermsg;

    sendUserName();

    FD_ZERO(&readSet);
    maxFd = std::max(_myFd, 0) + 1;

    while (true)
    {
        
        FD_ZERO(&readSet);

        FD_SET(0, &readSet);
        FD_SET(_myFd, &readSet);
        select(maxFd, &readSet, 0, 0, 0);

        if (FD_ISSET(0, &readSet))
        {
            int msglen=0;
            getline(std::cin, usermsg);
            parseUserInput(usermsg);
        }

        if (FD_ISSET(_myFd, &readSet))
        {
            int numRx = recv(_myFd, readArr, sizeof(readArr), 0);
            if (numRx == 0) 
            {
                 std::cout << "Server disconnted. Exiting..." << std::endl;
                 exit(0);
            }
            decodeMsg(readArr, numRx);
            std::cout << "Menu" << std::endl
                      << "1) Send broadcast message" << std::endl
                      << "2) Send unicast message" << std::endl
                      << "3) Block user" << std::endl;
        }
        
    }
}
