#include <algorithm>
#include <string>


// from https://stackoverflow.com/questions/865668/how-to-parse-command-line-arguments-in-c
// BOOST provides a command line parser, but I don't want to add a dependencey on BOOST

char* getCmdOption(char ** begin, char ** end, const std::string& option)
{
    char ** itr = std::find(begin, end, option);
    if (itr != end && ++itr != end)
    {
        return *itr;
    }
    return 0;
}

bool cmdOptionExists(char** begin, char** end, const std::string& option)
{
    return std::find(begin, end, option) != end;
}
