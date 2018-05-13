#include "cmechatClient.h"
#include "../common/cmechatCommandLineParser.h"

cmechatClient::cmechatClient(const char* logFileName,
                                int argc,
                                char **argv) :
                            _logger(logFileName)
{
    bool debug = cmdOptionExists(argv, argv+argc, "-d");
	_logger.setDebugMode(debug);

	// read the port from command line
	_port = getCmdOption(argv, argv+argc, "-p");
	if (_port)
	{
		std::string s;
		s += "User specified port ";
		s += _port;
		_logger.log(s.c_str());
	}
	else
	{
		_logger.log("-p <port> option is required. Exiting.");
		exit(1);
	}

	// read the ip from command line
	_host = getCmdOption(argv, argv+argc, "-h");
	if (_host)
	{
		std::string s;
		s += "User specified host ";
		s += _host;
		_logger.log(s.c_str());
	}
	else
	{
		_logger.log("-h <host> option is required. Exiting.");
		exit(1);
	}

}