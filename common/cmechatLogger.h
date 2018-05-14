#ifndef _CMECHAT_LOGGER_
#define _CMECHAT_LOGGER_

#include <fstream>
#include <iostream>

class cmechatLogger
{
public:
	bool isGood() const { return _isGood; }

	cmechatLogger(const char *destFile);
	~cmechatLogger();

	void log(const char*, bool isDebug=false);
	void log(std::string &s, bool isDebug=false);

	void setDebugMode(bool b) { _debugMode = b; }


private:
	std::ofstream _myfile;
	bool _isGood;
	bool _debugMode;

	cmechatLogger();
	void setIsGood(bool b) { _isGood = b; }
};

#endif
