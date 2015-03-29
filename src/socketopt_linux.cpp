#include "socketopt_linux.h"
#include "log.h"
#include <fcntl.h>
#include <sys/uio.h>
#include <unistd.h>
#include <netinet/tcp.h>
#include <netinet/in.h>

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
    //setNonblocking(fd);
    setCloseOnExec(fd);
    return fd;
}

int32 XDSocketOpt::connect(SOCKET fd, const sockaddr &addr)
{
    int32 ret = ::connect(fd, &addr, static_cast<socklen_t>(sizeof(addr)));
    if (-1 == ret) {
        XDLOG_merror("[XDSocketOpt] 连接fd:%d 失败 err:%d, %s", fd, errno, strerror(errno));
    }
    return ret;
}

int32 XDSocketOpt::bind(SOCKET fd, const sockaddr &addr)
{
    int32 ret = ::bind(fd, &addr, static_cast<socklen_t>(sizeof(addr)));
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
    SOCKET connfd;
    if (NULL == addr) {
        connfd = ::accept(fd, NULL, NULL);
    } else {
        socklen_t len = static_cast<socklen_t>(sizeof(sockaddr_in));
        connfd = ::accept(fd, (sockaddr *)addr, &len);
    }
    if (-1 == connfd) {
        XDLOG_merror("[XDSocketOpt] 接收fd:%d 失败 err:%d, %s", fd, errno, strerror(errno));
        int32 connError = errno;
        switch (connError) {
            case EAGAIN:
            case ECONNABORTED:
            case EINTR:
            case EPROTO:
            case EPERM:
            case EMFILE:
                errno = connError;
                break;
            case EBADF:
            case EFAULT:
            case EINVAL:
            case ENFILE:
            case ENOBUFS:
            case ENOMEM:
            case ENOTSOCK:
            case EOPNOTSUPP:
                XDLOG_merror("[XDSocketOpt] unexpected error err:%d, %s", connError,strerror(errno));
                break;
            default:
                XDLOG_merror("[XDSocketOpt] unkonw error err:%d, %s", connError, strerror(errno));
                break;
        }
    } else {
        setNonblocking(connfd);
        setCloseOnExec(connfd);
    }
    return connfd;
}

void XDSocketOpt::close(int32 fd)
{
    int32 ret = ::close(fd);
    if (-1 == ret) {
        XDLOG_merror("[XDSocketOpt] 关闭fd:%d 失败", fd);
    }
}

int32 XDSocketOpt::read(int32 fd, void *buffer, int32 len)
{
    int32 ret = ::read(fd, buffer, len);
    if (-1 == ret) {
        XDLOG_merror("[XDSocketOpt] fd:%d 读取失败", fd);
    }
    return ret;
}

int32 XDSocketOpt::write(int32 fd, const void *buffer, int32 len)
{
    int32 ret = ::write(fd, buffer, len);
    if (-1 == ret) {
        XDLOG_merror("[XDSocketOpt] fd:%d 写失败", fd);
    }
    return ret;
}

int32 XDSocketOpt::readv(int32 fd, const iovec *iov, int32 iovCnt)
{
    int32 ret = ::readv(fd, iov, iovCnt);
    if (-1 == ret) {
        XDLOG_merror("[XDSocketOpt] fd:%d 集中读取失败", fd);
    }
    return ret;
}

int32 XDSocketOpt::getSockError(SOCKET fd)
{
    int32 optVal;
    socklen_t len = static_cast<socklen_t>(sizeof(optVal));
    if (::getsockopt(fd, SOL_SOCKET, SO_ERROR, &optVal, &len)) {
        XDLOG_merror("[XDSocketOpt] 获取SockErroor error err:%d, %s", errno, strerror(errno));
        return errno;
    } else {
        return optVal;
    }
}

sockaddr_in XDSocketOpt::getLocalAddrWithSock(SOCKET fd)
{
    sockaddr_in addr;
    bzero(&addr, sizeof(addr));
    socklen_t len = static_cast<socklen_t>(sizeof(addr));
    if (-1 == ::getsockname(fd, (struct sockaddr *)&addr, &len)) {
        XDLOG_merror("[XDSocketOpt] 获取fd:%d 的本地地址失败 error err:%d, %s", fd, errno, strerror(errno));
    }
    return addr;
}

sockaddr_in XDSocketOpt::getPeerAddrWithSock(SOCKET fd)
{
    sockaddr_in addr;
    bzero(&addr, sizeof(addr));
    socklen_t len = static_cast<socklen_t>(sizeof(addr));
    if (-1 == ::getpeername(fd, (struct sockaddr *)&addr, &len)) {
        XDLOG_merror("[XDSocketOpt] 获取fd:%d 的对端地址失败 error err:%d, %s", fd, errno, strerror(errno));
    }
    return addr;
}

bool XDSocketOpt::isSelfConnect(SOCKET fd)
{
    struct sockaddr_in localAddr = getLocalAddrWithSock(fd);
    struct sockaddr_in peerAddr = getPeerAddrWithSock(fd);
    return localAddr.sin_port == peerAddr.sin_port &&
           localAddr.sin_addr.s_addr == peerAddr.sin_addr.s_addr;
}

bool XDSocketOpt::peek(SOCKET fd)
{
    char buffer[1];
    int32 ret = recv(fd, buffer, 1, MSG_PEEK | MSG_DONTWAIT);
    return ret > 0;
}

void XDSocketOpt::setSockTcpNoDelay(SOCKET fd, const void *optval, int32 optlen)
{
    int32 ret = ::setsockopt(fd, IPPROTO_TCP, TCP_NODELAY, optval, static_cast<socklen_t>(optlen));
    if (-1 == ret) {
        XDLOG_merror("[XDSocketOpt] fd:%d设置tcpNodelag value:%d error err:%d, %s", fd, optval, errno, strerror(errno));
    }
}

void XDSocketOpt::setSockReuseAddr(SOCKET fd, const void *optval, int32 optlen)
{
    int32 ret = ::setsockopt(fd, SOL_SOCKET, SO_REUSEADDR, optval, static_cast<socklen_t>(optlen));
    if (-1 == ret) {
        XDLOG_merror("[XDSocketOpt] fd:%d设置reuseAddr value:%d error err:%d, %s", fd, optval, errno, strerror(errno));
    }
}

void XDSocketOpt::setSockReusePort(SOCKET fd, const void *optval, int32 optlen)
{
    #ifdef SO_REUSEPORT
        int32 ret = ::setsockopt(fd, SOL_SOCKET, SO_REUSEPORT, optval, static_cast<socklen_t>(optlen));
        if (-1 == ret) {
            XDLOG_merror("[XDSocketOpt] fd:%d设置reuseport value:%d error err:%d, %s", fd, optval, errno, strerror(errno));
        }
    #else
        XDLOG_mwarn("[XDSocketOpt] SO_REUSEPORT 没定义");
    #endif
}
void XDSocketOpt::setSockKeepAlive(SOCKET fd, const void *optval, int32 optlen)
{
    int32 ret = ::setsockopt(fd, SOL_SOCKET, SO_KEEPALIVE, optval, static_cast<socklen_t>(optlen));
    if (-1 == ret) {
        XDLOG_merror("[XDSocketOpt] fd:%d设置keepalive value:%d error err:%d, %s", fd, optval, errno, strerror(errno));
    }
}

void XDSocketOpt::shutdownWrite(SOCKET fd)
{
    int32 ret = ::shutdown(fd, SHUT_WR);
    if (-1 == ret) {
        XDLOG_merror("[XDSocketOpt] fd:%dshutdownwrite failure error err:%d, %s", fd, errno, strerror(errno));
    }
}
