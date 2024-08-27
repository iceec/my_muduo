#include <Socket.h>
#include <sys/types.h> /* See NOTES */
#include <sys/socket.h>
#include "Log.hpp"
#include <strings.h>
#include <netinet/tcp.h>
void Socket::bind_address(InetAddress &local_addr)
{
    if (::bind(_sockfd, (sockaddr *)local_addr.get_sockaddr(), sizeof(sockaddr_in)) < 0)
    {
        LOG_FATAL("socket bind error %d\n", _sockfd);
    }
}

void Socket::listen()
{
    if (::listen(_sockfd, SOMAXCONN) < 0)
    {
        LOG_FATAL("socket listen error %d\n", _sockfd);
    }
}
// 返回的是 连接的套接字
// 但是可能这里的监听套接字是阻塞的 最好是非阻塞的
int Socket::accept(InetAddress &peer_addr)
{
    sockaddr_in addr;

    socklen_t len = sizeof(addr);

    bzero(&addr, sizeof(addr));
    // accept4可以将最后一个参数设置为 no block  accept返回的 就是阻塞的套接字
    int ac_fd = ::accept4(_sockfd, (sockaddr *)&addr, &len, SOCK_CLOEXEC | SOCK_NONBLOCK);

    if (ac_fd > 0) // 确实有一个
    {
        peer_addr.set_sockaddr(addr); // 对端的端口和 IP
    }

    return ac_fd;
}
// shutdown read 就是你不能主动在调用读取了 但是 write就是发送fin  
// 对端还可以发数据 close的话 再发的话得到rst 
void Socket::shut_down_write()
{

    if (::shutdown(_sockfd, SHUT_WR) < 0)
    {
        LOG_WARNING(" %d sock shut down send fin bad\n", _sockfd);
    }
}

void Socket::set_tcp_no_delay(bool flags)
{
    int op = flags ? 1 : 0;
    ::setsockopt(_sockfd, IPPROTO_TCP, TCP_NODELAY, &op, sizeof(op));
}
void Socket::set_reuse_addr(bool flags)
{
    int op = flags ? 1 : 0;
    ::setsockopt(_sockfd, SOL_SOCKET, SO_REUSEADDR, &op, sizeof(op));
}
void Socket::set_reuse_port(bool flags)
{
    int op = flags ? 1 : 0;
    ::setsockopt(_sockfd, SOL_SOCKET, SO_REUSEPORT, &op, sizeof(op));
}
void Socket::set_keep_alive(bool flags)
{
    int op = flags ? 1 : 0;
    ::setsockopt(_sockfd, SOL_SOCKET, SO_KEEPALIVE, &op, sizeof(op));
}