#ifndef _CMECHAT_CLIENT__
#define _CMECHAT_CLIENT__

#include <vector>

#include "../common/cmechatLogger.h"
#include "../common/cmechatInterface.h"


class cmechatClient
{
    public:
        cmechatClient(const char* logFileName,
                int argc,
                char** argv);
        void startClient();
        void connectToServer();
        void runChat();
        void getUsername();
        void sendUserName();

    private:
        cmechatLogger _logger;  //file logger object
        char* _port;            //port to connect to
        char* _host;            //host to connect to
        int _myFd;
        std::string _username;
        // blockedUsers could be optimized for performance with a map, 
        //  or other data structure.
        std::vector<std::string> _blockedUsers;

        bool decodeMsg(char *msg, int numRx);
        void parseUserInput(std::string &s);

};

#endif
