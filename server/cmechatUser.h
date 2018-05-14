#ifndef _CMECHATUSER_
#define _CMECHATUSER_

#include <string>

#include "cmechatServer.h"

class cmechatServer;

class cmechatUser
{
public:
    cmechatUser(cmechatUserManager *myUserManager);
    void runUser();
    void setup(int fd, std::string username);
    void decodeMsg(char *msg, int msglen);

private:
    std::string _username;
    int _fd;
    cmechatServer *_myServer;
    cmechatUserManager *_myUserManager;
};

#endif
