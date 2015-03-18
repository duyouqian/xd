#include "inetaddr_linux.h"
#include "socket_util.h"
#include "log.h"
#include <arpa/inet.h>
#include <stdio.h>

static const in_addr_t anyAddr = INADDR_ANY;
static const in_addr_t loopbackAddr = INADDR_LOOPBACK;

XDIpv4AddrLinuxImp::XDIpv4AddrLinuxImp()
{
}

XDIpv4AddrLinuxImp::~XDIpv4AddrLinuxImp()
{
}

XDIpv4AddrLinuxImp::XDIpv4AddrLinuxImp(uint16 port, bool loopbackOnly /* = false */)
{
    bzero(&addr_, sizeof(addr_));
    addr_.sin_family = AF_INET; // 地址族
    in_addr_t ip = loopbackOnly ? loopbackAddr : anyAddr;
    addr_.sin_addr.s_addr = XDSocketOpt::hostToNetwork32(ip);
    addr_.sin_port = XDSocketOpt::hostToNetwork16(port);
}

XDIpv4AddrLinuxImp::XDIpv4AddrLinuxImp(const char *ip, uint16 port)
{
    bzero(&addr_, sizeof(addr_));
    addr_.sin_family = AF_INET; // 地址族
    addr_.sin_port = XDSocketOpt::hostToNetwork16(port);
    if (-1 == ::inet_pton(AF_INET, ip, &addr_.sin_addr)) {
        XDLOG_merror("[IPV4Address] ip:%s为无效IP", ip);
    }
}

XDIpv4AddrLinuxImp::XDIpv4AddrLinuxImp(struct sockaddr_in& addr)
                  : addr_(addr)
{
}

std::string XDIpv4AddrLinuxImp::getIp() const
{
    char buff[32];
    ::inet_ntop(AF_INET, &addr_.sin_addr, buff, static_cast<socklen_t>(sizeof(buff)));
    std::string ip(buff);
    return ip;
}

std::string XDIpv4AddrLinuxImp::getIpAndPort() const
{
    char buff[32];
    ::inet_ntop(AF_INET, &addr_.sin_addr, buff, static_cast<socklen_t>(sizeof(buff)));
    uint16 port = XDSocketOpt::networkToHost16(addr_.sin_port);
    size_t end = strlen(buff);
    snprintf(buff + end, sizeof(buff) - end, ":%u", port);
    std::string ip(buff);
    return ip;
}

uint16 XDIpv4AddrLinuxImp::getPort() const
{
    return XDSocketOpt::networkToHost16(addr_.sin_port);
}

const sockaddr& XDIpv4AddrLinuxImp::getSockAddr() const
{
    return (sockaddr&)addr_;
}

sockaddr_in* XDIpv4AddrLinuxImp::getSockAddr()
{
    return &addr_;
}

void XDIpv4AddrLinuxImp::setSockAddr(const sockaddr_in &addr)
{
    addr_ = addr;
}

uint32 XDIpv4AddrLinuxImp::ipNetEndian() const
{
    return addr_.sin_addr.s_addr;
}

uint16 XDIpv4AddrLinuxImp::portNetEndian() const
{
    return addr_.sin_port;
}

bool XDIpv4AddrLinuxImp::resolve(const char *hostName, XDIpv4AddrLinuxImp *addr)
{
    // 后续再写
    return false;
}
