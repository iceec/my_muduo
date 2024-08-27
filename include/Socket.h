#pragma once
#include <unistd.h>
#include <InetAddress.h>
class Socket
{
public:
    explicit Socket(int fd) : _sockfd(fd) {}
    ~Socket() { ::close(_sockfd); }

    int fd() const { return _sockfd; }

    void bind_address(InetAddress &local_addr);

    void listen();
    int accept(InetAddress &peer_addr);

    void shut_down_write();

    void set_tcp_no_delay(bool);
    void set_reuse_addr(bool);
    void set_reuse_port(bool);
    void set_keep_alive(bool);

private:
    const int _sockfd;
};