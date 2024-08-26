#pragma once

#include "Poller.h"
#include <sys/epoll.h>

class EPoller : public Poller
{

    using ChannelList = std::vector<Channel *>;

public:
    void update_channel(Channel *) override;                              // update 利用 epoll_ctl 进行update
    void remove_channel(Channel *) override;                              // remove 对应 epoll_ctl的 del
    TimeStamp poll(int time_out_ms, ChannelList *activechannel) override; // 对应 epoll_wait

    EPoller(EventLoop *); // 进行epoll_create  不行的话就需要exit了
    ~EPoller();

private:
    static constexpr int init_events_size = 16;
    using EventList = std::vector<epoll_event>; // 表明一个wait的最多照看的事件

    // 填写活跃的连接
    void fill_active_channel(int ,ChannelList *)const;

    void update(int,Channel *); // 更新一个channel的方法
    EventList _events; // 表明 事件的list  // 给的是返回的
    int _epoll_fd; // epoll的文件描述符
};