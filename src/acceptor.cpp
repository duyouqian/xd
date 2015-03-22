#include "acceptor.h"
#include "log.h"
#include <fcntl.h>

class XDAcceptorHandleRead : public XDIOEventReadCallBack
{
public:
    explicit XDAcceptorHandleRead(XDAcceptor *accept) : accept_(accept)
    {
    }

    bool exec(uint64 timestamp)
    {
        accept_->loop_->checkInLoopThread();
        XDIpv4Addr peerAddr;
        FD connfd = accept_->acceptorSocket_.accept(&peerAddr);
        if (-1 == connfd) {
            // 失败
            XDLOG_merror("[Acceptor] HandleRead 接受新连接失败");
            if (errno == EMFILE) {
                XDSocketOpt::close(accept_->idleFD_);
                accept_->idleFD_ = XDSocketOpt::accept(accept_->acceptorSocket_.getSocketID(), NULL);
                XDSocketOpt::close(accept_->idleFD_);
                accept_->idleFD_ = (::open("/dev/null", O_RDONLY | O_CLOEXEC));
            }
        } else {
            // 成功
            if (accept_->newConnectionCallBack_.isValid()) {
                accept_->newConnectionCallBack_->exec(connfd, peerAddr);
            } else {
                XDSocketOpt::close(connfd);
            }
        }
        return true;
    }
private:
    XDAcceptor *accept_;
};

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
    XDSharedPtr<XDAcceptorHandleRead> handleRead(new XDAcceptorHandleRead(this));
    acceptorChannel_.setReadCallBack(handleRead);
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

void XDAcceptor::setNewConnectionCallBack(XDIOEventNewConnectionCallBackPtr &cb)
{
    newConnectionCallBack_ = cb;
}
