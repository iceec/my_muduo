#pragma once

#include "NoCopy.hpp"
#include <Thread.h>
#include <mutex>
#include <condition_variable>
#include <functional>
#include <string>

class EventLoop;

class EventLoopThread : NoCopy
{
public:
    using CallBack = std::function<void(EventLoop *)>;
    EventLoopThread(CallBack func = CallBack(), std::string name = "") : _thread(std::bind(&EventLoopThread::thread_func, this), name), _init_cb(func) {}
    ~EventLoopThread();
    EventLoop *start_loop(); // 给主线程调用的而且要返回evloop 留给主线程去操作
private:
    void thread_func();
    Thread _thread; // 有一个thread
    EventLoop *_loop{nullptr};
    std::condition_variable _cv{}; // 用来处理loop的
    std::mutex _lock{};
    CallBack _init_cb; // 用于开启线程之后要执行的第一个任务
    bool _exiting{false};
};