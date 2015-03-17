#include "socketopt_linux.h"
#include "log.h"
#include <fcntl.h>

int32 XDSocketOpt::setNonblocking(int32 fd)
{
    int flags = ::fcntl(fd, F_GETFL, 0);
    flags |= O_NONBLOCK;
    int ret = ::fcntl(fd, F_SETFL, flags);
    if (-1 == ret) {
        XDLOG_merror("[XDSocketOpt] 为fd:%d设置无堵塞fd失败", fd);
    }
    return ret;
}

int32 XDSocketOpt::setCloseOnExec(int32 fd)
{
    int flags = ::fcntl(fd, F_GETFL, 0);
    flags |= FD_CLOEXEC;
    int ret = ::fcntl(fd, F_SETFL, flags);
    if (-1 == ret) {
        XDLOG_merror("[XDSocketOpt] 为fd:%d设置fork时关闭fd失败", fd);
    }
    return ret;
}

SOCKET XDSocketOpt::createNonblockingOrDie()
{
    SOCKET fd = ::socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
    if (-1 == fd) {
        XDLOG_merror("[XDSocketOpt] 创建Ipv4 Tcp 连接失败 err:%d, %s", errno, strerror(errno));
        return INVALID_SOCKET;
    }
    // 成功
    setNonBlocking(fd);
    setCloseOnExec(fd);
    return fd;
}

int32 XDSocketOpt::connect(SOCKET fd, const sockaddr &addr)
{
    int32 ret = ::connect(fd, &addr, static_cast<socklen_t>(addr));
    if (-1 == ret) {
        XDLOG_merror("[XDSocketOpt] 连接fd:%d 失败 err:%d, %s", fd, errno, strerror(errno));
    }
    return ret;
}

int32 XDSocketOpt::bind(SOCKET fd, const sockaddr &addr)
{
    int32 ret = ::bind(fd, &addr, static_cast<socklen_t>(addr));
    if (-1 == ret) {
        XDLOG_merror("[XDSocketOpt] 绑定fd:%d 失败 err:%d, %s", fd, errno, strerror(errno));
    }
    return ret;
}

int32 XDSocketOpt::listen(SOCKET fd)
{
    int32 ret = ::listen(fd, SOMAXCONN);
    if (-1 == ret) {
        XDLOG_merror("[XDSocketOpt] 监听fd:%d count:%d 失败 err:%d, %s", fd, SOMAXCONN, errno, strerror(errno));
    }
    return ret;
}

SOCKET XDSocketOpt::accept(SOCKET fd, sockaddr_in *addr)
{
    SOCKET connfd = ::accept(fd, addr, static_cast<socklen_t>(sizeof(sockaddr_in)));
}

void XDSocketOpt::close(int32 fd)
{
    ::close(fd);
}
