#include <fstream>
#include <iostream>

class cmechatLogger
{
public:
	bool isGood() const { return _isGood; }

	cmechatLogger(const char *destFile);
	~cmechatLogger();

	void log(const char*) ;

private:
	std::ofstream _myfile;
	bool _isGood;

	cmechatLogger();
	void setIsGood(bool b) { _isGood = b; }
};
