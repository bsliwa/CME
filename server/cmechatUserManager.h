#ifndef _CMECHAT_USER_MANAGER_
#define _CMECHAT_USER_MANAGER_

#include <vector>

#include "cmechatServer.h"
#include "cmechatUser.h"

class cmechatUser;
class cmechatServer;

class cmechatUserManager
{
    public:
        cmechatUserManager(cmechatServer &myserver);
        cmechatUser* getNewUser();
        cmechatServer& myServer(return _myServer);

    private:
        std::vector<cmechatUser *> _userVector;
        cmechatServer &_myServer;

};

#endif
