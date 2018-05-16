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
        ~cmechatUserManager();
        
        cmechatUser* getNewUser();
        cmechatServer& myServer() {return _myServer;}
        std::vector<cmechatUser *> &getInUseVector() { return _inUseVector; }

        void freeUser(cmechatUser *user);

    private:
        std::vector<cmechatUser *> _freeUserVector; //has a vector of available users
        std::vector<cmechatUser *> _inUseVector;   //has a list of in use users
        cmechatServer &_myServer;

};

#endif
