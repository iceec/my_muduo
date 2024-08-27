#include <InetAddress.h>
#include <strings.h>
#include <string.h>
std::string InetAddress::to_ip()
{
    char buf[64]{0};

    ::inet_ntop(AF_INET, &_addr, buf, sizeof(buf));
    return buf;
}
std::string InetAddress::to_ip_and_port()
{
    char buf[64]{0};
    ::inet_ntop(AF_INET, &_addr, buf, sizeof(buf));
    int end = strlen(buf);
    uint16_t port = ::ntohs(_addr.sin_port);
    snprintf(buf + end, 10, "%u", port);

    return buf;
}
uint16_t InetAddress::to_port()
{
    return ::ntohs(_addr.sin_port);
}

InetAddress::InetAddress(uint16_t prot, std::string ip = "127.0.0.1")
{
    sockaddr_in addr;
    bzero(&addr, sizeof(addr));

    addr.sin_family = AF_INET;
    addr.sin_addr.s_addr = ::inet_addr(ip.c_str()); // 转换成为 s.addr
    addr.sin_port = htons(prot);
    _addr = addr;
}