#ifndef _CMECHAT_USER_MANAGER_
#define _CMECHAT_USER_MANAGER_

#include <vector>

#include "cmechatUser.h"

class cmechatUser;

class cmechatUserManager
{
    public:
        cmechatUserManager(cmechatServer &myserver);
        cmechatUser* getNewUser();

    private:
        std::vector<cmechatUser *> _userVector;
        cmechatServer _myServer;

};

#endif
