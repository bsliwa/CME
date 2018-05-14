#include <stack>

#include "cmechatUser.h"

class cmechatUserManager
{
    public:
        cmechatUserManager();

    private:
        std::stack<cmechatUser *> _userStack;

};