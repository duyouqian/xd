#include "tcp_server.h"
#include "log.h"

XDTcpServer::XDTcpServer(XDEventLoop *loop,
                         const XDIpv4Addr &addr,
                         const char *name,
                         bool isReuseport)
           : name_(name)
           , host_(addr.getIpAndPort())
           , loop_(loop)
           , accept_(new XDAcceptor(loop, addr, isReuseport))
           , threadPool_(new XDIOEventLoopThreadPool(loop, name_))
           , connectionCallBack_(NULL)
           , messageCallBack_(NULL)
           , nextConnID_(1)
{
    accept_->setNewConnectionCallBack(std::bind(&XDTcpServer::newConnectionCallBack, this, std::placeholders::_1, std::placeholders::_2));
}

XDTcpServer::~XDTcpServer()
{
    loop_->checkInLoopThread();
    XDLOG_minfo("[TCPServer] 关闭 [%s] 自毁", name_.c_str());
    std::map<std::string, XDTcpConnectionPtr>::iterator it;
    for (it = connections_.begin(); it != connections_.end(); ++it) {
        XDTcpConnectionPtr conn = it->second;
        conn->getLoop()->runInLoop(std::bind(&XDTcpConnection::connectDestroyed,conn));
    }
}

void XDTcpServer::setNumThread(uint32 num)
{
    check(num >= 0);
    threadPool_->setNumThread(num);
}

void XDTcpServer::start()
{
    if (started_.set(1) == 0) {
        threadPool_->start(threadInitCallBack_);
        check(!accept_->listenning());
        loop_->runInLoop(accept_->listenCallBack_);
    }
}

void XDTcpServer::setThreadInitCallBack(const XDIOEventLoopThreadInitCallBack &cb)
{
    threadInitCallBack_ = cb;
}

void XDTcpServer::setConnectionCallBack(const XDIOEventConnectionCallBack &cb)
{
    connectionCallBack_ = cb;
}

void XDTcpServer::setMessageCallBack(const XDIOEventMessageCallBack &cb)
{
    messageCallBack_ = cb;
}

void XDTcpServer::setWriteCompleteCallBack(const XDIOEventWriteCompleteCallBack &cb)
{
    wirteCompleteCallBack_ = cb;
}

XDSharedPtr<XDIOEventLoopThreadPool> XDTcpServer::getThreadPool()
{
    return threadPool_;
}

void XDTcpServer::newConnectionCallBack(FD fd, const XDIpv4Addr &addr)
{
    loop_->checkInLoopThread();
    XDEventLoop *ioLoop = threadPool_->getNextLoop();
    char buff[32];
    snprintf(buff, sizeof(buff), ":%s#%d", host_, nextConnID_);
    ++nextConnID_;
    std::string connName = name_ + buff;
    XDLOG_minfo("[TcpServer] 新连接 %s 来自:%s", connName.c_str(), addr.getIpAndPort());
    XDIpv4Addr localAddr(XDSockOpt::getLocalAddrWithSock(fd));
    XDTcpConnectionPtr conn(new XDTcpConnection(ioloop,
                                                connName,
                                                fd,
                                                localAddr,
                                                addr));
    connections_[connName] = conn;
    conn->setConnectionCallBack(connectionCallBack_);
    conn->setMessageCallBack(messageCallBack_);
    conn->setWriteCompleteCallBack(writeCompleteCallBack_);
    conn->setCloseCallBack(std::bind(&XDTcpServer::removeConnectionCallBack, this, std::placeholders::_1));
    ioLoop->runInLoop(std::bind(&XDTcpConnection::connectEsEntablished, conn));
}

void XDTcpServer::removeConnectionCallBack(const XDTcpConnectionPtr &conn)
{
    loop_->runInLoop(std::bind(&XDTcpServer::removeConnectionInLoop, this, conn));
}

void XDTcpServer::removeConnectionInLoop(const XDTcpConnectionPtr &conn)
{
    loop_->checkInLoopThread();
    XDLOG_minfo("[TcpServer] removeConnectionInLoop [%s] - connection %s", name_.c_str(), conn->name().c_str());
    int32 n = connections_.erase(conn->name());
    check(1 == n);
    XDEventLoop *ioLoop = conn->getLoop();
    ioLoop->queueInLoop(std::bind(&XDTcpConnection::connectDestroyed, conn));
}
