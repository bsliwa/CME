#include "../common/cmechatLogger.h"

class cmechatServer 
{
public:
	cmechatServer(const char* logFileName, 
			int argc,
			char** argv);
	void openServer();
	void listen();


private:
	cmechatLogger _logger;	//file logger object
	char *_port;		//tcp port server will open
	char *_node; 		//ip socket will use
	int _listenFd;
	
};
