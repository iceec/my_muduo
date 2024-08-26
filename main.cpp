#include "TimeStamp.hpp"
#include "Log.hpp"
#include <iostream>
using namespace std;
int main()
{
#ifdef DB
    LOG_INFO("%s %d", "hahaha", 1);

    LOG_WARNING("%s %d", "hahaha", 1);
    LOG_DEBUG("%s %d", "hahaha", 1);
    LOG_FATAL("%s %d", "hahaha", 1);
#endif
}