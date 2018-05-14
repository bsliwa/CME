#ifndef _CHECHAT_USER_MANAGER_
#define _CHECHAT_USER_MANAGER_

#include <vector>

#include "cmechatUser.h"

class cmechatUser;

class cmechatUserManager
{
    public:
        cmechatUserManager();
        cmechatUser* getNewUser();

    private:
        std::vector<cmechatUser *> _userVector;

};

#endif
