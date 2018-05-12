#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>

class cmechatLogger
{
public:
	bool isGood() const { return _isGood; }

	cmechatLogger(char *destFile);
	~cmechatLogger();

private:
	int _fd;
	bool _isGood;

	cmechatLogger();
	void setIsGood(bool b) { _isGood = b; }
};
