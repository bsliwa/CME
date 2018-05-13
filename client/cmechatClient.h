#include "../common/cmechatLogger.h"

class cmechatClient
{
    public:
        cmechatClient(const char* logFileName,
                int argc,
                char** argv);
        void startClient();

    private:
        cmechatLogger _logger;  //file logger object
        char* _port;              //port to connect to
        char* _host;            //host to connect to
        

};