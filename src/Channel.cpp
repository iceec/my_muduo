#include "EventLoop.h"
#include "Channel.h"
#include"Log.hpp"
void Channel::update()
{
    // add code ...
    // _loop->update_channel(*this);
}

// 删除掉当前的channel
// 这个地方可能已经remove了 所以 tie也就没有用处了
void Channel::remove()
{
    // add code ...
    // _loop->remove_channel(*this);
}

// 什么时候调用呢
void Channel::tie(const std::shared_ptr<void> &obj)
{
    _tie = obj;
    _tied = true;
}


// 查看是不是tie 主要的接口为了 提供events 
// 所有的evetns 都是guard
void 
Channel::handle_event(TimeStamp receivetime)
{
    if (_tied)
    {
        auto guard = _tie.lock();
        if (guard)
        {
            handle_event_with_guard(receivetime);
        }
    }
    else
        handle_event_with_guard(receivetime);
}
void
Channel::handle_event_with_guard(TimeStamp receivetime)
{
#ifdef DB
LOG_INFO("channel handle events revent :%d\n",_revents);
#endif


    // 发生错误
    if((_revents & EPOLLHUP) && !(_revents & EPOLLIN))
    {
        if(_close_cb)
            _close_cb();
    }


    if(_revents & EPOLLERR)
    {
        if(_error_cb)
            _error_cb();
    }

    if(_revents & k_read_event)
    {
        if(_read_cb)
            _read_cb(receivetime);
    }

    if(_revents & k_write_event)
    {
        if(_write_cb)
            _write_cb();
    }


}