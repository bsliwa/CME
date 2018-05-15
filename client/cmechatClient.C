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

// Constructor.  It reads the command line arguments and stores them.
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
        std::string logme("-p <port> option is required. Exiting.");
        std::cout << logme << std::endl;
        _logger.log(logme);
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
        std::string logme("-h <host> option is required. Exiting.");
        std::cout << logme << std::endl;
        _logger.log(logme);
        exit(1);
    }
}

// Attempts to connect to server via tcp port and ip provided on command
// line by user
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
    
    std::string logme;
    logme = "Connected to ";
    logme += s;
    _logger.log(logme.c_str());

    _myFd = mysock;
}

// Gets user name from stdin
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

bool userIsBlocked(std::vector<std::string> &userVector, std::string &user)
{
    for (std::vector<std::string>::iterator it = userVector.begin();
           it != userVector.end(); 
           ++it)
    {
        if (*it == user) 
            return true;
    }
    return false;
}

//return true if message was printed
bool cmechatClient::decodeMsg(char *msg, int numRx)
{
    if (msg[0] == CMECHAT_OPCODE_BROADCAST_MESSAGE)
    {
        struct cmechatMessageBroadcastMessage *bcast = (struct cmechatMessageBroadcastMessage*)msg;
        std::string sentFromUser = ((struct cmechatMessageBroadcastMessage*)(msg))->sourceUsername;
        
        // if origin is from another user, and that user isn't blocked, print the message
        if (sentFromUser != _username
                && userIsBlocked(this->_blockedUsers, sentFromUser) == false)
        {
             std::cout << "Received message from: " << sentFromUser << "-->" << bcast->body << std::endl;
        }
    }

    if (msg[0] == CMECHAT_OPCODE_UNICAST_MESSAGE)
    {
        struct cmechatMessageUnicastMessage *bcast = (struct cmechatMessageUnicastMessage*)msg;
        std::string sentFromUser = ((struct cmechatMessageUnicastMessage*)(msg))->sourceUsername;
        std::string sentToUser = ((struct cmechatMessageUnicastMessage*)(msg))->targetUsername;

        // if origin is another user and destined for me, and I haven't blocked the user, print it
        if (sentFromUser != _username 
                && this->_username == sentToUser
                && userIsBlocked(this->_blockedUsers, sentFromUser) == false)
        {
             std::cout << "Received message from: " << sentFromUser << "-->" << bcast->body << std::endl;
        }
    }


}

void cmechatClient::parseUserInput(std::string &usermsg)
{
    int msglen;

    // 1 - send broadcast msg.  
    // ask user for the message, put it in a cmechatMessageBroadcastMessage struct, 
    //  then send it
    if (usermsg == "1")
    {
        struct cmechatMessageBroadcastMessage msg;

        std::cout << "Enter message: " ;
        std::string bcastmsg;
        getline(std::cin, bcastmsg);

        // create the output msg
        msg.opcode = CMECHAT_OPCODE_BROADCAST_MESSAGE; msglen += sizeof(msg.opcode);
        strcpy(msg.sourceUsername, _username.c_str()); msglen += sizeof(msg.sourceUsername);
        strcpy(msg.body, bcastmsg.c_str());            msglen += bcastmsg.length()+1;
        msg.body[bcastmsg.length()] = '\0';
        int sentBytes = send(_myFd, (char*)&msg, msglen, 0);
    }

    // 2 - send unicast message
    // ask user for the target user.  then ask for the actual message.  then put it in 
    //  a cmechatMessageUnicastMessage, and send it out
    if (usermsg == "2")
    {
        struct cmechatMessageUnicastMessage msg;

        std::cout << "Who do you want to send to? " ;
        std::string who;
        getline(std::cin, who);

        std::cout << "Enter message: " ;
        std::string ucastmsg;
        getline(std::cin, ucastmsg);

        // create the output msg
        msg.opcode = CMECHAT_OPCODE_UNICAST_MESSAGE;   msglen += sizeof(msg.opcode);
        strcpy(msg.sourceUsername, _username.c_str()); msglen += sizeof(msg.sourceUsername);
        strcpy(msg.targetUsername, who.c_str());       msglen += sizeof(msg.targetUsername);
        strcpy(msg.body, ucastmsg.c_str());            msglen += ucastmsg.length()+1;
        msg.body[ucastmsg.length()] = '\0';
        int sentBytes = send(_myFd, (char*)&msg, msglen, 0);
    }

    // 3 - block a user.  ask for the username to block. store the username.
    if (usermsg == "3")
    {
        struct cmechatMessageUnicastMessage msg;

        std::cout << "Who do you want to block? " ;
        std::string who;
        getline(std::cin, who);

        _blockedUsers.push_back(who);
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
        bool printmenu;
        FD_ZERO(&readSet);

        FD_SET(0, &readSet);
        FD_SET(_myFd, &readSet);
        select(maxFd, &readSet, 0, 0, 0);

        if (FD_ISSET(0, &readSet))
        {
            int msglen=0;
            getline(std::cin, usermsg);
            parseUserInput(usermsg);
            printmenu=true;
        }

        if (FD_ISSET(_myFd, &readSet))
        {
            int numRx = recv(_myFd, readArr, sizeof(readArr), 0);
            if (numRx == 0) 
            {
                 std::cout << "Server disconnted. Exiting..." << std::endl;
                 exit(0);
            }
            printmenu = decodeMsg(readArr, numRx);

        }
        if (printmenu)
                std::cout << "Menu" << std::endl
                      << "1) Send broadcast message" << std::endl
                      << "2) Send unicast message" << std::endl
                      << "3) Block user" << std::endl;
        
    }
}
