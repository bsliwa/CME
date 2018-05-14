#include <string>
#include <sys/types.h>
#include <sys/socket.h>
#include <iostream>

#include "cmechatUser.h"
#include "cmechatInterface.h"

cmechatUser::cmechatUser() :
    _fd(0),
    _username("")
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
		std::cout << "Message received on server (" << numRx << ") " << readArr << std::endl;
	}

}
