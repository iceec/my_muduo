#pragma once
#include "NoCopy.hpp"
#include <vector>
#include <unordered_map>
#include "TimeStamp.hpp"
class Channel;
class EventLoop;

/*
本身是对于IO多路复用的抽象 本身的维护 并不是一个 liost

维护一个map  用于fd -> channel * 的查询
对于输入的list 可以进行poll
poller只管把你送来的fd 给到IO复用执行器 
poller的设计还要再往下看
*/
class Poller : NoCopy
{

    using ChannelList = std::vector<Channel *>;

public:
    virtual void updata_channel(Channel *) = 0;
    virtual void remove_channel(Channel *) = 0;
    virtual TimeStamp poll(int time_out_ms, ChannelList *activechannel) = 0;


    bool has_channel(Channel *);

    Poller(EventLoop *);

    // 建立一个新的poller
    static Poller *new_default_poller(EventLoop *); 

// 子类是可以继续访问的
protected:
    // fd -> 对应的channel
    using ChannelMap = std::unordered_map<int, Channel *>;
    ChannelMap _channel_map;
    
private:
    EventLoop *_loop; // 所属的线程
};