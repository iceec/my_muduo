#include "EventLoopThread.h"
#include <EventLoop.h>
EventLoopThread::~EventLoopThread()
{
    _exiting = true;

    if (_loop)
    {
        _loop->quit();
        _thread.join(); // 这里会join等待线程的完成
    }
}

EventLoop *EventLoopThread::start_loop()
{
    EventLoop *result;
    _thread.start(); // 已经开始了另一个线程
    {
        std::unique_lock<std::mutex> tmp(_lock);
        _cv.wait(tmp, [&]()
                 { return _loop != nullptr; });
        result = _loop;
    }
    return result;
}

// 留给开启的线程去进行执行的
void EventLoopThread::thread_func()
{
    EventLoop loop;
    // 先执行初始化函数
    if (_init_cb)
        _init_cb(&loop);

    
    {
        std::unique_lock<std::mutex> tmp(_lock);
        _loop = &loop;
        _cv.notify_one(); // 通知相应的线程
    }

    _loop->loop(); // 执行循环把  poller -> poll  一个线程执行自己的poll
    // 只能是别人调用了quit  自己线程thread的析构 或者直接操作 了loop 

    // loop  推出了
    std::lock_guard<std::mutex> tmp(_lock);
    _loop = nullptr;
}