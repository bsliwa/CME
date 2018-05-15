#include <string>
#include <sys/types.h>
#include <sys/socket.h>
#include <iostream>
#include <string>

#include "cmechatUser.h"
#include "cmechatInterface.h"


cmechatUser::cmechatUser(cmechatUserManager &myUserManager) :
    _fd(0),
    _username(""),
    _myServer(0),
    _myUserManager(myUserManager)
{
}

void cmechatUser::setup(int fd, std::string username)
{
	_fd = fd;
	_username = username;
}

void cmechatUser::runUser()
{
    char readArr[MAX_MSG_LEN];

    while(true)
	{
		int numRx = recv(_fd, readArr, sizeof(readArr), 0);
		if (numRx == 0)
		{
			break;
		}
		std::string logme;
		logme = "Message received on server (";
		logme += std::to_string(numRx);
		logme += ")";
		logme += readArr;
                _myUserManager.myServer().log(logme);
		decodeMsg(readArr, numRx);
	}
    
	this->_myUserManager.freeUser(this);
	std::string logme;
	logme = "User ";
	logme += this->_username;
	logme += " disconnected";
	this->_myUserManager.myServer().log(logme);
   	this->_myUserManager.freeUser(this);

}

void cmechatUser::decodeMsg(char *msg, int msgLen)
{
	char opcode;


	//get the opcode by mapping the first 8 bytes into opcode
	if (msgLen > sizeof(int))
	{
		opcode = ((char)*msg);
	}

	if (opcode == CMECHAT_OPCODE_NEWUSER)
	{
		struct cmechatMessageNewuser *newUserMsg;
		newUserMsg = (struct cmechatMessageNewuser *)msg;
		this->_username = newUserMsg->username;
	}

	if (opcode == CMECHAT_OPCODE_BROADCAST_MESSAGE || opcode == CMECHAT_OPCODE_UNICAST_MESSAGE) 
	{
		struct cmechatMessageBroadcastMessage *broadcastMsg;
		broadcastMsg = (struct cmechatMessageBroadcastMessage *) msg;
		this->_myUserManager.myServer().fanOutBroadcast(broadcastMsg, msgLen);
	}
}
