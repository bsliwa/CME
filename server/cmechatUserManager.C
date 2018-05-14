#include "cmechatUserManager.h"
#include "cmechatDefines.h"
#include "cmechatUser.h"

cmechatUserManager::cmechatUserManager()
{
    for(int i=0;  i<MAX_CONNECTIONS;  i++)
    {
        _userVector.push_back(new cmechatUser());
    }
}

cmechatUser* cmechatUserManager::getNewUser()
{
    cmechatUser *user = _userVector.back();
    _userVector.pop_back();
    return user;
}