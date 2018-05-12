#include <iostream>

#include "cmechatLogger.h"

cmechatLogger::cmechatLogger(char *destFileName) :
	_fd(0),
	_isGood(false)
{
	if (destFileName == 0)
	{
		std::cout << "ERROR: destFileName is NULL. Could not start the file logger." << std::endl;
		return;
	}

	_fd = open(destFileName, O_APPEND | O_CREAT | O_WRONLY);
	if (_fd <= 0)
	{
		std::cout << "ERROR: Name is NULL. Could not init the file logger." << std::endl;
		return;
	}

	_isGood = true;

	
}


cmechatLogger::~cmechatLogger()
{
//	if (
}
