#pragma once
#include "NoCopy.hpp"
#include <vector>
#include <string>
#include <memory>
#include<functional>
class EventLoopThread;
class EventLoop;

class EventLoopThreadPool : NoCopy
{
public:
    using CallBack = std::function<void(EventLoop *)>;  // 目的是初始化工作线程
    EventLoopThreadPool(EventLoop *base_loop, const std::string_view pool_name) : _base_loop(base_loop), _thread_pool_name(pool_name) {}
    ~EventLoopThreadPool() = default;
    void set_thread_poll_number(int nums) { _thread_cnt = nums; }

    void start(CallBack func);
    EventLoop *get_next_loop();
    std::vector<EventLoop *> get_all_loop();

    bool started() { return _started; }
    const std::string &name() { return _thread_pool_name; }

private:
    std::vector<std::unique_ptr<EventLoopThread>> _thread_pool; // 存放线程
    std::vector<EventLoop *> _event_loop_pool;                  // 存放线程对应的loop 不用手动释放都是在站上 的
    EventLoop *_base_loop;
    bool _started{false};
    int _thread_cnt{0};
    int _next_idx{0};
    std::string _thread_pool_name;
};