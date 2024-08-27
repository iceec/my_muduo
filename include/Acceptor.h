#pragma once

class EventLoop;

#include <Channel.h>
#include <NoCopy.hpp>
#include <Socket.h>
#include <functional>
#include<InetAddress.h>

class Acceptor
{

public:
    using NewConnecionCallBack = std::function<void(int, InetAddress &)>;
    Acceptor(EventLoop *basic_loop, InetAddress & listenaddr ,bool reuse = true);
    ~Acceptor();

    void listen();
    bool listenning(){return _listenning;}

    void set_connection_call_back(NewConnecionCallBack func){_cb = func;}

private:
    void handle_read();       // 监听套接字自己本身的回调函数 poller出来执行这个
    EventLoop *_accept_loop;  // 就是basic
    Socket _accept_socket;    // 监听套接字
    Channel _accept_channel;  // 监听的channel
    NewConnecionCallBack _cb{}; //  监听套接字构造真实套接字的fuc
    bool _listenning{false};
};