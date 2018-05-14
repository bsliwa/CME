#ifndef _CMECHATUSER_
#define _CMECHATUSER_

#include <string>

#include "cmechatServer.h"
#include "cmechatUserManager.h"

class cmechatServer;
class cmechatUserManager;

class cmechatUser
{
public:
    cmechatUser(cmechatUserManager &myUserManager);
    void runUser();
    void setup(int fd, std::string username);
    void decodeMsg(char *msg, int msglen);
    std::string username() { return _username; }

private:
    std::string _username;
    int _fd;
    cmechatServer *_myServer;
    cmechatUserManager &_myUserManager;
};

#endif
