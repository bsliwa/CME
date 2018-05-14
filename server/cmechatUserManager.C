#include "cmechatDefines.h"
#include "cmechatUser.h"
#include "cmechatUserManager.h"

cmechatUserManager::cmechatUserManager()
{
    for(int i=0;  i<MAX_CONNECTIONS;  i++)
    {
        _userVector.push_back(new cmechatUser(*this));
    }
}

cmechatUser* cmechatUserManager::getNewUser()
{
    cmechatUser *user = _userVector.back();
    _userVector.pop_back();
    return user;
}
