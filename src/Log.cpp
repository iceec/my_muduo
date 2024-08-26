#include "Log.hpp"
#include"TimeStamp.hpp"
#include<iostream>
void Log::set_level(Log_Level level)
{
    _level = level;
}

Log &Log::instance()
{
    static Log single;
    return single;
}

std::string_view Log::log_pre[Log::Log_Level::END]{
    "[INFO]",
    "[ERROR]",
    "[FATAL]",
    "[DEBUG]",
};

void
Log::log(const std::string_view  buf)
{
    std::lock_guard<std::mutex> tmp(_lock);

    std::cout<<Log::log_pre[_level]<<" "<<TimeStamp::now().to_string()<<": "<<buf<<std::endl;

}


