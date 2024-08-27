#pragma once
#include <arpa/inet.h>
#include <netinet/in.h>
#include <string>
// 封装  sockaddress
class InetAddress
{
public:
    explicit InetAddress(uint16_t prot, std::string ip = "127.0.0.1");
    explicit InetAddress(sockaddr_in addr) : _addr(addr) {}

    std::string to_ip();
    std::string to_ip_and_port();
    uint16_t to_port();


    const sockaddr_in * get_sockaddr(){return &_addr;}

    void set_sockaddr(sockaddr_in addr){_addr = addr;}

private:
    sockaddr_in _addr;
};