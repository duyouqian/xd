#include "acceptor.h"
#include "log.h"
#include <fcntl.h>
#include <functional>

XDAcceptor::XDAcceptor(XDIOEventLoop *loop,
                       const XDIpv4Addr &listenAddr,
                       bool reuseport)
          : loop_(loop)
          , acceptorSocket_(XDSocketOpt::createNonblockingOrDie())
          , acceptorChannel_(loop, (FD)acceptorSocket_.getSocketID())
          , listenning_(false)
          , idleFD_(::open("/dev/null", O_RDONLY | O_CLOEXEC))
          , newConnectionCallBack_(NULL)
{
    check(idleFD_ != -1);
    acceptorSocket_.setReuseAddr(true);
    acceptorSocket_.setReusePort(reuseport);
    acceptorSocket_.bindAddress(&listenAddr);
    acceptorChannel_.setReadCallBack(std::bind(&XDAcceptor::handRead, this, std::placeholders::_1));
}

XDAcceptor::~XDAcceptor()
{
    acceptorChannel_.disableAll();
    acceptorChannel_.remove();
    XDSocketOpt::close(idleFD_);
}

bool XDAcceptor::listenning() const
{
    return listenning_;
}

void XDAcceptor::listen()
{
    loop_->checkInLoopThread();
    listenning_ = true;
    acceptorSocket_.listen();
    acceptorChannel_.setEvent(XDIOEventType_READ, true);
}

void XDAcceptor::setNewConnectionCallBack(const XDIOEventNewConnectionCallBack &cb)
{
    newConnectionCallBack_ = cb;
}

void XDAcceptor::handRead(uint64 timestamp)
{
    loop_->checkInLoopThread();
    XDIpv4Addr peerAddr;
    FD connfd = acceptorSocket_.accept(&peerAddr);
    if (-1 == connfd) {
        // 失败
        XDLOG_merror("[Acceptor] HandleRead 接受新连接失败");
        if (errno == EMFILE) {
            XDSocketOpt::close(idleFD_);
            idleFD_ = XDSocketOpt::accept(acceptorSocket_.getSocketID(), NULL);
            XDSocketOpt::close(idleFD_);
            idleFD_ = (::open("/dev/null", O_RDONLY | O_CLOEXEC));
        }
    } else {
        // 成功
        if (newConnectionCallBack_) {
            newConnectionCallBack_(connfd, peerAddr);
        } else {
            XDSocketOpt::close(connfd);
        }
    }
}
