#include "acceptor.h"
#include "logh"

class XDAcceptorHandleRead : public XDIOEventReadCallBack
{
public:
    explicit XDAcceptorHandleRead(XDAccept *accept) : accept_(accept)
    {
    }

    bool exec(uint64 timestamp)
    {
        accept_->loop_->checkInLoopThread();
        XDIpv4Addr peerAddr;
        FD connfd = accept_->acceptSocket_->accept(&peerAddr);
        if (-1 == connfd) {
            // 失败
            XDLOG_merror("[Acceptor] HandleRead 接受新连接失败");
            if (errno == EMFILE) {
                XDSocketOpt::close(accept_->idleFD_);
                accept_->idleFD_ = XDSocketOpt::accept(acceptSocket_.fd(), NULL);
                XDSOcketOpt::close(accept_->idleFD_);
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
    XDAccept *accept_;
};

XDAcceptor::XDAcceptor(XDIOEventLoop *loop,
                       const XDIpv4Addr &listenAddr,
                       bool reuseport)
          : loop_(loop)
          , acceptorSocket_(XDSocketOpt::createNonblockingOrDie())
          , acceptChannel_(loop, acceptorSocket_.getSocketID())
          , acceptorSocket_()
          , listenning_(false)
          , idleFD_(::open("/dev/null", O_RDONLY | O_CLOEXEC))
          , newConnectionCallBack_(NULL)
{
    check(idleFD_ != -1);
    acceptSocket_.setReuseAddr(true);
    acceptSocket_.setReusePort(reuseport);
    acceptSocket_.bindAddress(&listenAddr);
    XDSharePtr<XDAcceptHandleRead> handleRead(new XDAcceptHanleRead(this));
    acceptChannel_.setReadCallBack(handleRead);
}

XDAcceptor::~XDAcceptor()
{
    acceptChannel_.disableAll();
    acceptChannel_.remove();
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

void XDAcceptor::setNewConnectionCallBack(XDIOEventNewConnectionBackPtr cb)
{
    newConnectionCallBack_ = cb;
}
