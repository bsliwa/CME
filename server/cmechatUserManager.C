#include "cmechatDefines.h"
#include "cmechatUser.h"
#include "cmechatUserManager.h"

cmechatUserManager::cmechatUserManager(cmechatServer &myServer)
: _myServer(myServer)
{
    for(int i=0;  i<MAX_CONNECTIONS;  i++)
    {
        _freeUserVector.push_back(new cmechatUser(*this));
    }
}

cmechatUser* cmechatUserManager::getNewUser()
{
    cmechatUser *user = _freeUserVector.back();
    _freeUserVector.pop_back();

    _inUseVector.push_back(user);

    return user;
}
/*
void cmechatUserManager::freeUser(cmechatUser *user)
{
    for (std::vector<cmechatUser*>::iterator it = _inUseVector.begin();
          it != _inUseVector.end();
          ++it)
          {
              if (*it == user)
              {
                  _inUseVector.erase()
              }
          }
}
*/
