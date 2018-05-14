#ifndef _CMECHAT_OPTIONS_
#define _CMECHAT_OPTIONS_


char* getCmdOption(char ** begin, char ** end, const std::string& option);
bool cmdOptionExists(char** begin, char** end, const std::string& option);

#endif
