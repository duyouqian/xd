#include "socket_linux.h"
#include "socket_util.h"
#include "log.h"

XDSocketLinuxImp::XDSocketLinuxImp(SOCKET fd /* = INVALID_SOCKET */)
                : fd_(fd)
                , isOpen_(INVALID_SOCKET != fd)
{
}

XDSocketLinuxImp::~XDSocketLinuxImp()
{
    if (isOpen_ ) {
        XDSocketOpt::close(fd_);
        fd_ = INVALID_SOCKET;
        isOpen_ = false;
    }
}


int32 XDSocketLinuxImp::open()
{
    check(INVALID_SOCKET == fd_);
    fd_ = XDSocketOpt::createNonblockingOrDie();
    check(INVALID_SOCKET != fd_);
    isOpen_ = true;
    return 0;
}

int32 XDSocketLinuxImp::close()
{
    check(isValid());
    XDSocketOpt::close(fd_);
    fd_ = INVALID_SOCKET;
    isOpen_ = false;
    return 0;
}

int32 XDSocketLinuxImp::read(void *buffer, int32 len)
{
    check(isValid());
    int32 ret = XDSocketOpt::read(fd_, buffer, len);
    if (-1 == ret) {
        XDLOG_merror("[Socket] fd:%d 读取数据失败", fd_);
    }
    return ret;
}

int32 XDSocketLinuxImp::write(const void *buffer, int32 len)
{
    check(isValid());
    int32 ret = XDSocketOpt::write(fd_, buffer, len);
    if (-1 == ret) {
        XDLOG_merror("[Socket] fd:%d 写数据:%s失败", fd_, buffer);
    }
    return ret;
}

bool XDSocketLinuxImp::isOpen()
{
    return isOpen_;
}

bool XDSocketLinuxImp::peek()
{
    check(isValid());
    return XDSocketOpt::peek(fd_);
}

TRANSPORTID XDSocketLinuxImp::getID()
{
    return fd_;
}

SOCKET XDSocketLinuxImp::getSocketID() const
{
    return fd_;
}

void XDSocketLinuxImp::getTcpInfo(tcpinfo *info)
{
}

std::string XDSocketLinuxImp::getTcpInfoToString()
{
    std::string tcpInfo("");
    return tcpInfo;
}

int32 XDSocketLinuxImp::bindAddress(const XDIpv4Addr *addr)
{
    check(isValid());
    int32 ret = XDSocketOpt::bind(fd_, addr->getSockAddr());
    if (-1 == ret) {
        XDLOG_merror("[Socket] fd:%d 绑定地址:%s失败", fd_, addr->getIpAndPort().c_str());
    }
    return ret;
}

int32 XDSocketLinuxImp::listen()
{
    check(isValid());
    int32 ret = XDSocketOpt::listen(fd_);
    if (-1 == ret) {
        XDLOG_merror("[Socket] fd:%d 监听失败", fd_);
    }
    return ret;
}

int32 XDSocketLinuxImp::accept(XDIpv4Addr *addr)
{
    check(isValid());
    int32 ret = XDSocketOpt::accept(fd_, addr->getSockAddr());
    if (-1 == ret) {
        XDLOG_merror("[Socket] fd:%d 接收新连接失败", fd_);
    }
    return ret;
}

void XDSocketLinuxImp::shutdownWrite()
{
    check(isValid());
    XDSocketOpt::shutdownWrite(fd_);
}

void XDSocketLinuxImp::setTcpNoDelay(bool on)
{
    check(isValid());
    int32 optVal = on ? 1 : 0;
    XDSocketOpt::setSockTcpNoDelay(fd_, &optVal, sizeof(optVal));
}

void XDSocketLinuxImp::setReuseAddr(bool on)
{
    check(isValid());
    int32 optVal = on ? 1 : 0;
    XDSocketOpt::setSockReuseAddr(fd_, &optVal, sizeof(optVal));
}

void XDSocketLinuxImp::setReusePort(bool on)
{
    check(isValid());
    int32 optVal = on ? 1 : 0;
    XDSocketOpt::setSockReusePort(fd_, &optVal, sizeof(optVal));
}

void XDSocketLinuxImp::setKeepAlive(bool on)
{
    check(isValid());
    int32 optVal = on ? 1 : 0;
    XDSocketOpt::setSockKeepAlive(fd_, &optVal, sizeof(optVal));
}
