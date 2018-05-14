#include <string>

class cmechatUser
{
public:
    cmechatUser();

private:
    std::string _username;
    int _fd;
};