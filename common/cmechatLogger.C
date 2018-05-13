#include <iostream>
#include <ios>
#include <ctime>

#include "cmechatLogger.h"

cmechatLogger::cmechatLogger(const char *destFileName) :
	_myfile(0),
	_isGood(false)
{
	if (destFileName == 0)
	{
		std::cout << "ERROR: destFileName is NULL. Could not start the file logger." << std::endl;
		return;
	}

	_myfile.open (destFileName, std::fstream::out | std::fstream::app );  //output and append mode
	if (!_myfile.is_open())
	{
		std::cout << "ERROR: Could not start the file logger." << std::endl;
		return;
	}

	_isGood = true;

	log("Opened log file");
}

cmechatLogger::~cmechatLogger()
{
	if (_myfile.is_open()) 
		_myfile.close();
}

void cmechatLogger::log(const char *logme) 
{
	// get the time and print it to the log file
	std::time_t t = std::time(0);
	char mbstr[100];
	if (std::strftime(mbstr, sizeof(mbstr), "%c", std::localtime(&t))) 
		_myfile << mbstr << " - ";

	// now send the user's text to the log file
	_myfile << logme
		<< std::endl;
}
