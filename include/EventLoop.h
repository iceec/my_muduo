#pragma once
#include <functional>
#include <atomic>
#include <mutex>
#include <TimeStamp.hpp>
#include <memory>
#include <vector>
#include <CurrentThread.h>

class Poller;

class Channel;
/*
对应于每一个线程来说的loop 每一个线程去当前自己的loop当中去寻找fd fd自己封装了channel来保证事件的执行
但是loop对象可能处于另一个线程 
因为这里的回调函数都是void()所以主线程要拥有所有的loop * 这样去投递 时候 可以传入 loop->update之类的
*/

class EventLoop
{

public:
    using Functor = std::function<void()>; // functor表示回调函数类型
    using ChannelLists = std::vector<Channel *>;

    EventLoop();
    ~EventLoop();

    // 开启事件循坏
    void loop();

    // 退出事件循环
    void quit();

    // 把回调函数立刻执行或者放到 队列当中
    void run_in_loop(Functor);
    void queue_in_loop(Functor);

    // 唤醒 loop对应的event_fd 因为loop可能处于不同的进程
    // 但是event_fd对应的是真实的一个一个的线程
    void wake_up();

    // 为poller和channel 交互提供的接口
    void update_channel(Channel *);
    void remove_channel(Channel *);
    bool has_channel(Channel *);

    bool is_in_loop_thread() { return _thread_id == CurrentThread::gettid(); }

private:
    void wake_up_handle_read();


    // 执行 loop -> 对应的线程上面的所有回调函数
    void doPendingFunctors(); 

    std::atomic_bool _looping{false}; // 是否正在循环
    std::atomic_bool _quit{false};    // 是否退出了

    const pid_t _thread_id; // 记录eventloop是被哪个线程创建的 并不表示当前所在哪个线程

    TimeStamp _poller_return_time{}; // 记录 poller的wait之后的返回时间  给我active_channel
    std::unique_ptr<Poller> _poller; // poller指针

    int _wake_up_fd; // 本质上是文件描述符 mainloop通过这个通知eventloop该干活了
    std::unique_ptr<Channel> _wake_up_channel;

    ChannelLists _active_channel; // 表示active的channel

    std::atomic_bool _calling_prending_functor{false}; // 当前loop是否在执行回调操作
    std::vector<Functor> _pending_functor;             // 表明当前loop等待执行的func
    std::mutex _pending_lock;                          // 用来保护pending functor的
};