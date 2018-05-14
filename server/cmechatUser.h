#ifndef _CMECHATUSER_
#define _CMECHATUSER_

#include <string>

class cmechatUser
{
public:
    cmechatUser();

private:
    std::string _username;
    int _fd;
};

#endif