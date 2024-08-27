#include <Acceptor.h>
#include <sys/types.h> /* See NOTES */
#include <sys/socket.h>
#include <Log.hpp>
static int create_nonblock_listenfd()
{
    int fd = ::socket(AF_INET, SOCK_STREAM | SOCK_CLOEXEC | SOCK_NONBLOCK, IPPROTO_TCP);

    if (fd < 0)
    {
        LOG_FATAL("create listen fd bad\n");
    }

    return fd;
}

Acceptor::Acceptor(EventLoop *basic_loop, InetAddress &listenaddr, bool reuse) : _accept_loop(basic_loop),
                                                                                        _accept_socket(create_nonblock_listenfd()),
                                                                                        _accept_channel(basic_loop, _accept_channel.fd())
{

    _accept_socket.bind_address(listenaddr);
    _accept_socket.set_keep_alive(reuse);
    _accept_socket.set_reuse_addr(reuse);
    _accept_socket.set_reuse_port(reuse);

    // 设置发生channel时候的函数
    _accept_channel.set_read_call_back(std::bind(&Acceptor::handle_read, this));
}
void Acceptor::listen()
{

    _accept_socket.listen();
    _accept_channel.enable_read();
    _listenning = true;
}

Acceptor::~Acceptor()
{
    _accept_channel.disable_all();
    _accept_channel.remove();
}
// poller 这个里面发挥了
void Acceptor::handle_read()
{
    InetAddress peer_address;

    int c_fd = _accept_socket.accept(peer_address);

    if (c_fd >= 0)
    {
        if (_cb)
        {
            // 这个里面会对 c_fd做出判断
            _cb(c_fd, peer_address);
        }
        else
        {
            ::close(c_fd); // 关闭的是处理连接的
        }
    }
    else
    {
        LOG_WARNING("handle read accept bad\n");
        if (errno == EMFILE)
        {
            LOG_WARNING("accept no more fd\n");
        }
    }
}
