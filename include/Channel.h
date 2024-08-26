

/*

对于文件描述符的一系列的封装 主要保证了 每一个sockfd会有自己的事件管理 状态管理 ...
提供一定接口给poller(多路复用处理器)使得poller能够设计一些事情

有自己的fd  
有自己的读写函数设置 
可以确定events
供他人设置revents


hanle_events 真正 处理事件
tie 绑定自己


*/
#pragma once
#include "NoCopy.hpp"
#include "TimeStamp.hpp"
#include <functional>
#include <sys/epoll.h>
#include<memory>

class EventLoop;

class Channel : NoCopy
{
public:
    using Event_Call_Back = std::function<void()>;
    using Event_Read_Call_Back = std::function<void(TimeStamp)>; // 可以接受 时间戳 但是不知道为什么

    Channel(EventLoop *loop, int fd) : _loop(loop), _fd(fd) {} // 对应于每一个channel都要有sock和工作线程
    ~Channel() {}

    // 当poller告诉我你有事件的时候 处理事件 ？？？？
    void handle_event(TimeStamp receivetime);

    void set_read_call_back(Event_Read_Call_Back cb) { _read_cb = std::move(cb); }
    void set_write_call_back(Event_Call_Back cb) { _write_cb = std::move(cb); }
    void set_error_call_back(Event_Call_Back cb) { _error_cb = std::move(cb); }
    void set_close_call_back(Event_Call_Back cb) { _close_cb = std::move(cb); }



    int fd(){return _fd;}
    void set_revents(int revents){_revents = revents;}

    void enable_read(){_events |= k_read_event; update();}
    void enable_write(){_events |= k_write_event; update();}
    void disable_read(){_events &= ~(k_read_event); update();}
    void disable_write(){_events &= ~(k_write_event); update();}
    void disable_all(){_events = k_none_event ; update();}

    bool is_disable_all(){return _events == k_none_event;}
    int events(){return _events;}


    // 用于自己绑定自己的 
    void tie(const  std::shared_ptr<void> &);
    int type(){return _type;}
    void set_type(int type){_type = type;}

    EventLoop * ownerloop(){return _loop;}
    void remove();  // 移除 本身 从loop当中移除自己

    




private:
    // 最终是会去 多路复用 去实现的update
    // eventloop自己里面有这个poller 这个可能是多个eventlopp指向同一个poller
    void update();
    //处理特殊任务的 不知道? 
    void handle_event_with_guard(TimeStamp recvivetime);


    
    EventLoop *_loop; // 当前fd所处于的loop 可以简单理解为线程
    int _fd;          // sock fd


    int _events{0};       // 需要监听的事件
    int _revents{0};      // 返回回来的事件

    int _type{-1}; // ?? 不知道是什么用

    Event_Call_Back _write_cb{};
    Event_Call_Back _close_cb{};
    Event_Read_Call_Back _read_cb{};
    Event_Call_Back _error_cb{};

    std::weak_ptr<void> _tie{};
    bool _tied{false};


    static constexpr int k_read_event = EPOLLIN | EPOLLPRI; 
    static constexpr int k_write_event = EPOLLOUT;
    static constexpr int k_none_event =  0 ;
};