#ifndef XD_SOCKETIMP_LINUX_H
#define XD_SOCKETIMP_LINUX_H

#include "transport.h"
#include "inetaddr.h"
#include "types.h"
#include <string>

class XDSocketLinuxImp : public XDITransport
{
public:
    explicit XDSocketLinuxImp(SOCKET fd = INVALID_SOCKET);
    ~XDSocketLinuxImp();
    int32 open();
    int32 close();
    int32 read(void *buffer, int32 len);
    int32 write(const void *buffer, int32 len);
    bool isOpen();
    bool peek();
    TRANSPORTID getID();
    SOCKET getSocketID() const;

    void getTcpInfo(struct tcpinfo *info);
    std::string getTcpInfoToString();
    // 绑定地址
    int32 bindAddress(const XDIpv4Addr *addr);
    // 监听
    int32 listen();
    // 接送连接 addr用于存放client地址
    int32 accept(XDIpv4Addr *addr);
    void shutdownWrite();
    // 设置开启/关闭 Nagle算法
    void setTcpNoDelay(bool on);
    // 设置地址是否重用
    void setReuseAddr(bool on);
    // 设置端口是否重用
    void setReusePort(bool on);
    // 设置是否保活
    void setKeepAlive(bool on);

private:
    FORCEINLINE bool isValid()
    {
        return INVALID_SOCKET != fd_;
    }

private:
    SOCKET fd_;
    bool isOpen_;
};

#endif // end xd_socketimp_linux_h
