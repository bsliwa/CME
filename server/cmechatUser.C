#include <string>
#include <sys/types.h>
#include <sys/socket.h>
#include <iostream>

#include "cmechatUser.h"
#include "cmechatInterface.h"


cmechatUser::cmechatUser() :
    _fd(0),
    _username(""),
    _myServer(0)
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
			std::cout << "User disconnected " << std::endl;
			break;
		}
		std::cout <<std::endl <<std::endl<< "Message received on server (" << numRx << ") " << readArr << std::endl;
		decodeMsg(readArr, numRx);
	}

}

void cmechatUser::decodeMsg(char *msg, int msgLen)
{
	char opcode;

	std::cout << "decodeMsg decoding " << msgLen << " bytes " << std::endl;

	//get the opcode by mapping the first 8 bytes into opcode
	if (msgLen > sizeof(int))
	{
		opcode = ((char)*msg);
		std::cout << "opcode is: " << (int)opcode << std::endl;
	}

	if (opcode == CMECHAT_OPCODE_NEWUSER)
	{
		struct cmechatMessageNewuser *newUserMsg;
		newUserMsg = (struct cmechatMessageNewuser *)msg;
		this->_username = newUserMsg->username;
		std::cout << "the username is " << this->_username << ".." << newUserMsg->username <<std::endl;
	}

	if (opcode == CMECHAT_OPCODE_BROADCAST_MESSAGE)
	{
		struct cmechatMessageBroadcastMessage *broadcastMsg;
		broadcastMsg = (struct cmechatMessageBroadcastMessage *) msg;

	}
}
