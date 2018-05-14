#include <string>

#include "cmechatUser.h"

cmechatUser::cmechatUser() :
    _fd(0),
    _username("")
{

}

void cmechatUser::runUser()
{
    while(true)
	{
		int numRx = recv(myFd, readArr, sizeof(readArr), 0);
		if (numRx == 0)
		{
			std::cout << "User disconnected " << std::endl;
			break;
		}
		std::cout << "Message received on server (" << numRx << ") " << readArr << std::endl;
	}

}