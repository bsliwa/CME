#include <mutex>

#include "cmechatDefines.h"
#include "cmechatUser.h"
#include "cmechatUserManager.h"

// mutex prevents contention from multiple theads modifying 
// the free pool and in use pool 
std::mutex g_user_mutex;

// create a pool of users, during init
// allocate max # of users up front
cmechatUserManager::cmechatUserManager(cmechatServer &myServer)
: _myServer(myServer)
{
    for(int i=0;  i<MAX_CONNECTIONS;  i++)
    {
        _freeUserVector.push_back(new cmechatUser(*this));
    }
}

cmechatUserManager::~cmechatUserManager()
{
    
}

// return one user from the free pool, or null if there are
// no users available.  user goes from the free pool to the
// in use pool
cmechatUser* cmechatUserManager::getNewUser()
{
    std::lock_guard<std::mutex> lock(g_user_mutex);

    cmechatUser *user = _freeUserVector.back();
    _freeUserVector.pop_back();

    _inUseVector.push_back(user);

    return user;
}

// move user from the inUse pool to the free poll
void cmechatUserManager::freeUser(cmechatUser *user)
{
    std::lock_guard<std::mutex> lock(g_user_mutex);

    for (std::vector<cmechatUser*>::iterator it = _inUseVector.begin();
          it != _inUseVector.end();
          ++it)
          {
              if (*it == user)
              {
                  _inUseVector.erase(it);
                  break;
              }
          }
}

