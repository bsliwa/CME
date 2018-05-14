#ifndef _CMECHATUSER_
#define _CMECHATUSER_

#include <string>

class cmechatUser
{
public:
    cmechatUser();
    void runUser();
    void setup(int fd, std::string username);

private:
    std::string _username;
    int _fd;
};

#endif
