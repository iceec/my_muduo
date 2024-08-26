#include "TimeStamp.hpp"
#include <ctime>

std::string
TimeStamp::to_string(void)
{
    time_t tt = std::chrono::system_clock::to_time_t(time);

    tm *ptm = localtime(&tt);

    char buf[100] = {0};

    snprintf(buf, 99, "%04d-%02d-%02d %02d:%02d:%02d", ptm->tm_year + 1900, ptm->tm_mon + 1, ptm->tm_mday, ptm->tm_hour, ptm->tm_min, ptm->tm_sec);

    return buf;
}


TimeStamp 
TimeStamp::now()
{
  
    return std::chrono::system_clock::now();
}