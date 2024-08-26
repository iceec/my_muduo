#include "Poller.h"
#include "Channel.h"
Poller::Poller(EventLoop *loop) : _loop(loop) {}

bool Poller::has_channel(Channel *channel)
{
    int fd = channel->fd();

    if (_channel_map.count(fd))
    {
        return _channel_map[fd] == channel;
    }

    return false;
}
