#pragma once
#include <chrono>
#include <string>
struct TimeStamp
{

    static TimeStamp now();

    TimeStamp() = default;

    TimeStamp(std::chrono::_V2::system_clock::time_point t) : time(t) {}

    std::string to_string();
    // 本身维持一个time时间
private:
    std::chrono::_V2::system_clock::time_point time;
};