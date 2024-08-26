#include "EPoller.h"
#include "Log.hpp"
#include <string.h>
#include <unistd.h>
#include "Channel.h"
#include <assert.h>
#include <stdlib.h>

static constexpr int knew = -1;
static constexpr int kdeleted = 0;
static constexpr int kadded = 1;
EPoller::EPoller(EventLoop *loop) : Poller(loop), _events(init_events_size)
{
    // 表明了 if 进行exec 这个文件描述符是要关闭的
    _epoll_fd = epoll_create1(EPOLL_CLOEXEC);

    if (_epoll_fd == -1)
    {
        LOG_FATAL("EPoller: %s\n", strerror(errno));
    }
}
EPoller::~EPoller()
{
    ::close(_epoll_fd);
}
void EPoller::update_channel(Channel *channel)
{
    // 看看你要现在的type 是什么样子了
    int type = channel->type();
    int fd = channel->fd();
    LOG_DEBUG("func=%s => fd=%d events=%d index=%d\n", __FUNCTION__, channel->fd(), channel->events(), type);
    if (type == knew || type == kdeleted)
    {

        if (type == knew)
        {
            assert(Poller::_channel_map.count(fd) == 0);
            _channel_map[fd] = channel;
        }
        else // type == kdeleted
        {
            assert(Poller::_channel_map.count(fd));
        }
        channel->set_type(kadded);
        update(EPOLL_CTL_ADD, channel);
    }
    else // kadded
    {
        assert(Poller::_channel_map.count(fd));

        if (channel->is_disable_all())      // 就是判断事件
            update(EPOLL_CTL_DEL, channel); // 没事件了 就撤退
        else
            update(EPOLL_CTL_MOD, channel);
    }
}

void EPoller::update(int op, Channel *channel)
{
    epoll_event ep_event;

    memset(&ep_event, 0, sizeof(ep_event));

    ep_event.data.ptr = channel; // 这个表示 wait返回回来的时候这个ep_event的哪个有效
    ep_event.events = channel->events();

    if (::epoll_ctl(_epoll_fd, op, channel->fd(), &ep_event) < 0)
    {
        if (op == EPOLL_CTL_DEL)
        {
            LOG_WARNING("epoll_ctl del error %s\n", strerror(errno));
        }
        else
        {
            LOG_FATAL("epoll_ctl add/mod error %s\n", strerror(errno));
        }
    }
}
void EPoller::remove_channel(Channel *channel)
{

    int type = channel->type();

    LOG_DEBUG("func=%s => fd=%d\n", __FUNCTION__, channel->fd());

    assert(Poller::_channel_map.count(channel->fd()));
    assert(channel->is_disable_all());
    assert(type == kadded || type == kdeleted); // delete表示暂时消去 并不代表永久 永久表示remove

    if (type == kadded)
    {
        update(EPOLL_CTL_DEL, channel);
    }

    Poller::_channel_map.erase(channel->fd());

    channel->set_type(knew);
}
TimeStamp
EPoller::poll(int time_out_ms, ChannelList *activechannel)
{
    LOG_INFO("func=%s => fd total count:%lu\n", __FUNCTION__, _channel_map.size());

    int numsevents = ::epoll_wait(_epoll_fd, &(*_events.begin()), static_cast<int>(_events.size()), time_out_ms);

    int saveerror = errno;

    if (numsevents > 0)
    {
        if (static_cast<size_t>(numsevents) == _events.size())
            _events.resize(_events.size() * 2);
        

        fill_active_channel(numsevents,activechannel);
    }
    else if (numsevents == 0) // timeouw
    {
        LOG_INFO("%s timeout\n", __FUNCTION__);
    }
    else if (numsevents < 0)
    {
        if (saveerror != EINTR)
        {
            errno = saveerror; // dont konw why
            LOG_WARNING("%s: epoller::poll() error\n", strerror(errno));
        }
    }

    return TimeStamp::now();
}


void 
EPoller::fill_active_channel(int number ,ChannelList * activechannel)const
{
    activechannel->reserve(number);

    for(int i=0;i<number;++i)
    {
        Channel * tmp =static_cast<Channel*>( _events[i].data.ptr);
        tmp->set_revents(_events[i].events); // 设置事件
        activechannel->push_back(tmp);
    }
    
}