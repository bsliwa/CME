#include "../common/cmechatLogger.h"
#include "cmechatUserManager.h"

class cmechatServer 
{
public:
	cmechatServer(const char* logFileName, 
			int argc,
			char** argv);
	void openServer();
	void listen();

	static cmechatUserManager userManager;


private:
	cmechatLogger _logger;	//file logger object
	char *_port;		//tcp port server will open
	char *_node; 		//ip socket will use
	int _listenFd;
	
};
