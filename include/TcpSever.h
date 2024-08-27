#pragma once

class TcpConnection;

#include <functional>
#include <CallBack.h>
#include <Buffer.hpp>
#include <TimeStamp.hpp>
#include <TcpConnection.h>
#include <string>
#include <Acceptor.h>
#include <EventLoopThreadPool.h>
#include <atomic>
#include <unordered_map>
#include<EventLoopThread.h>
class TcpSever
{
public:
    using Thread_Init_Call_Back = std::function<void(EventLoop *)>;
    using Connection_Map = std::unordered_map<std::string, TcpConnectionPtr>;
    enum option
    {
        noreuse = 0,
        reuse
    };

    TcpSever(EventLoop *basic_loop, InetAddress &listenaddr, std::string_view pool_name, option op = reuse);

    ~TcpSever();

    void set_thread_init_call_back(const Thread_Init_Call_Back &cb) { _thread_init_cb = cb; }
    void set_connection_call_back(const Connection_Call_Back &cb) { _connect_cb = cb; }
    void set_message_call_back(const Message_Call_Back &cb) { _message_cb = cb; }
    void set_write_complete_call_back(const Write_Complete_Call_Back &cb) { _write_complete_cb = cb; }

    void set_thread_pool_number(int nums) { _event_thread_pool->set_thread_poll_number(nums); }

    // 开启服务器
    void start();

private:
    void new_connection(int, InetAddress &);

    void remove_connection(TcpConnectionPtr &);

    void remove_connection_in_loop(TcpConnection &);

    EventLoop *_basic_loop;

    const std::string _ip_and_port; // 表明监听端口的ip port
    std::unique_ptr<Acceptor> _acceptor;

    const std::string _pool_name; // 给到pool的
    std::shared_ptr<EventLoopThreadPool> _event_thread_pool;

    Connection_Call_Back _connect_cb;
    Message_Call_Back _message_cb;
    Write_Complete_Call_Back _write_complete_cb; // 消息发送完的
    Thread_Init_Call_Back _thread_init_cb;

    int _next_connd_id; // ??

    std::atomic_int32_t _started; // why ??

    Connection_Map _connection_map{}; // 维护所有的连接的哈希白哦
};
