#include "cmechatUserManager.h"

cmechatUserManager::cmechatUserManager()
{
    _userStack.push(new cmechatUser());
}