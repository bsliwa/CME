#include "../common/cmechatLogger.h"

#define MAX_MSG_LEN 280
#define MAX_USERNAME_LEN 28

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

    private:
        cmechatLogger _logger;  //file logger object
        char* _port;            //port to connect to
        char* _host;            //host to connect to
        int _myFd;
        std::string _username;
};