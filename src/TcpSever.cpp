#include <TcpSever.h>
#include <Log.hpp>

static EventLoop *is_nullptr(EventLoop *p)
{
    if (p == nullptr)
    {
        LOG_FATAL("tcp sever basic loop is nullptr");
    }
    return p;
}
TcpSever::TcpSever(EventLoop *basic_loop,
                   InetAddress &listenaddr,
                   std::string_view pool_name,
                   option op) : _basic_loop(basic_loop),
                                _ip_and_port(listenaddr.to_ip_and_port()),
                                _acceptor(std::make_unique<Acceptor>(is_nullptr(_basic_loop), listenaddr)),
                                _pool_name(pool_name),
                                _event_thread_pool(std::make_shared<EventLoopThreadPool>(_basic_loop, pool_name)),
                                _connect_cb(),
                                _message_cb(),
                                _write_complete_cb(),
                                _thread_init_cb(),
                                _next_connd_id(1),
                                _started(0)
{
}

TcpSever::~TcpSever()
{
}

void TcpSever::new_connection(int, InetAddress &)
{
}

void TcpSever::remove_connection(TcpConnectionPtr &)
{
}

void TcpSever::remove_connection_in_loop(TcpConnection &)
{
}

void TcpSever::start()
{
}