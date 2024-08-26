#pragma once
#include "NoCopy.hpp"
#include <string>
#include <mutex>
#include <string_view>

class Log : NoCopy // 私有继承 保证了 构造函数变成私有的了
{

public:
    enum Log_Level
    {
        INFO = 0,
        WARNING,
        FATAL,
        DEBUG,
        END
    };

    static Log &instance();

    void set_level(Log_Level);

    void log(const std::string_view buf);

private:
    Log() = default;
    Log_Level _level;
    static std::string_view log_pre[Log_Level::END];
    std::mutex _lock{};
};
// 用do while 是一个很好的方法
#define LOG_INFO(msgformat, ...)                       \
    do                                                 \
    {                                                  \
        auto &single = Log::instance();                \
        single.set_level(Log::Log_Level::INFO);        \
        char buf[1024]{0};                             \
        snprintf(buf, 1024, msgformat, ##__VA_ARGS__); \
        single.log(buf);                               \
    } while (0)

#define LOG_WARNING(msgformat, ...)                    \
    do                                                 \
    {                                                  \
        auto &single = Log::instance();                \
        single.set_level(Log::Log_Level::WARNING);     \
        char buf[1024]{0};                             \
        snprintf(buf, 1024, msgformat, ##__VA_ARGS__); \
        single.log(buf);                               \
    } while (0)

#define LOG_FATAL(msgformat, ...)                      \
    do                                                 \
    {                                                  \
        auto &single = Log::instance();                \
        single.set_level(Log::Log_Level::FATAL);       \
        char buf[1024]{0};                             \
        snprintf(buf, 1024, msgformat, ##__VA_ARGS__); \
        single.log(buf);                               \
        exit(EXIT_FAILURE);                            \
    } while (0)

#define LOG_DEBUG(msgformat, ...)                      \
    do                                                 \
    {                                                  \
        auto &single = Log::instance();                \
        single.set_level(Log::Log_Level::DEBUG);       \
        char buf[1024]{0};                             \
        snprintf(buf, 1024, msgformat, ##__VA_ARGS__); \
        single.log(buf);                               \
    } while (0)
