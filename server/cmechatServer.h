#include "../common/cmechatLogger.h"

class cmechatServer 
{
public:
	cmechatServer(const char* logFileName);
	int openServer();

private:
	cmechatLogger _logger;	

};
