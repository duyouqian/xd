#ifndef XD_INETADDRIMP_LINUX_H
#define XD_INETADDRIMP_LINUX_H

#include "noncopyable.h"
#include "types.h"

#include <string>
#include <netinet/in.h>

class XDIpv4AddrLinuxImp : XDNoncopyable
{
public:
    XDIpv4AddrLinuxImp();
    explicit XDIpv4AddrLinuxImp(uint16 port, bool loopbackOnly = false);
    XDIpv4AddrLinuxImp(const char *ip, uint16 port);
    explicit XDIpv4AddrLinuxImp(struct sockaddr_in& addr);
    ~XDIpv4AddrLinuxImp();

    std::string getIp() const;
    std::string getIpAndPort() const;
    uint16 getPort() const;

    const struct sockaddr& getSockAddr() const;
    struct sockaddr_in* getSockAddr();
    void setSockAddr(const struct sockaddr_in &addr);

    uint32 ipNetEndian() const;
    uint16 portNetEndian() const;

    // 根据主机名解析出ipv4地址
    static bool resolve(const char *hostName, XDIpv4AddrLinuxImp *addr);

private:
    struct sockaddr_in addr_;
};

#endif // end xd_inetaddrimp_linux_h
