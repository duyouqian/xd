#include "tcp_connection.h"
#include "log.h"
#include "event_loop.h"
#include "channel.h"
#include "socket.h"
#include "socket_util.h"

#include <functional>

void defaultConnectionCallBack(const XDTcpConnectionPtr &conn)
{
    XDLOG_minfo("[TcpConnection] 新连接:%s -> %s is %s", conn->localAddress().getIpAndPort().c_str(), conn->peerAddress().getIpAndPort().c_str(), (conn->connected() ? "UP" : "DOWN"));
}

void defaultMessageCallBack(const XDTcpConnectionPtr &conn, XDBuffer *buffer, uint64 timestamp)
{
    uint32 len = buffer->readableBytes();
    std::string msg = buffer->retrieveAsString(len);
    XDLOG_minfo("[TcpConnection] 消息:%s -> %s msg: %s", conn->localAddress().getIpAndPort().c_str(), conn->peerAddress().getIpAndPort().c_str(), msg.c_str());
}


XDTcpConnection::XDTcpConnection(XDIOEventLoop *loop,
                                 std::string& name,
                                 SOCKET fd,
                                 const XDIpv4Addr &localAddr,
                                 const XDIpv4Addr &peerAddr)
: name_(name)
, loop_(loop)
, state_(state_connecting)
, socket_(new XDSocket(fd))
, channel_(new XDChannel(loop, fd))
, localAddr_(localAddr)
, peerAddr_(peerAddr)
, highWaterMark_(64 * 1024 * 1024)

{
    channel_->setReadCallBack(std::bind(&XDTcpConnection::handleRead, this, std::placeholders::_1));
    channel_->setWriteCallBack(std::bind(&XDTcpConnection::handleWrite, this));
    channel_->setCloseCallBack(std::bind(&XDTcpConnection::handleClose, this));
    channel_->setErrorCallBack(std::bind(&XDTcpConnection::handleError, this));
    
    socket_->setKeepAlive(true);
    XDLOG_mdebug("[TcpConnection] create [%s] at fd=%d", name_.c_str(), fd);
}

XDTcpConnection::~XDTcpConnection()
{
    XDLOG_mdebug("[TcpConnection] delete [%s] at fd=%d state=%s ", name_.c_str(), channel_->fd(), stateToStr());
    check(state_ == state_disconnected);
}

XDIOEventLoop* XDTcpConnection::getLoop() const
{
    return loop_;
}

const std::string& XDTcpConnection::name() const
{
    return name_;
}

const XDIpv4Addr& XDTcpConnection::localAddress() const
{
    return localAddr_;
}

const XDIpv4Addr& XDTcpConnection::peerAddress() const
{
    return peerAddr_;
}

bool XDTcpConnection::connected() const
{
    return state_ == state_connected;
}

bool XDTcpConnection::disconnected() const
{
    return state_ == state_disconnected;
}

void XDTcpConnection::setContext(void *context)
{
    context_ = context;
}

const void* XDTcpConnection::getContext() const
{
    return context_;
}

void* XDTcpConnection::getMutableContext()
{
    return context_;
}

XDBuffer* XDTcpConnection::getOutputBuffer()
{
    return &outputBuffer_;
}

XDBuffer* XDTcpConnection::getInputBuffer()
{
    return &inputBuffer_;
}

void XDTcpConnection::setConnectionCallBack(const XDIOEventConnectionCallBack &cb)
{
    connectionCallBack_ = cb;
}

void XDTcpConnection::setMessageCallBack(const XDIOEventMessageCallBack &cb)
{
    messageCallBack_ = cb;
}

void XDTcpConnection::setWriteCompleteCallBack(const XDIOEventWriteCompleteCallBack &cb)
{
    writeCompleteCallBack_ = cb;
}

void XDTcpConnection::setHighWaterMarkCallBack(const XDIOEventHighWateMarkCallBack &cb, int32 highWaterMark)
{
    highWateMarkCallBack_ = cb;
    highWaterMark_ = highWaterMark;
}

void XDTcpConnection::setCloseCallBack(const XDIOEventCloseCallBack &cb)
{
    closeCallBack_ = cb;
}

void XDTcpConnection::setTcpNoDelay(bool on)
{
    socket_->setTcpNoDelay(on);
}

void XDTcpConnection::connectEsEntablished()
{
    loop_->checkInLoopThread();
    check(state_connecting == state_);
    setState(state_connected);
    channel_->setEvent(XDIOEventType_READ, true);
    //connectionCallBack_(self_);
    connectionCallBack_(shared_from_this());
}

//void XDTcpConnection::setSelf(const XDTcpConnectionPtr &self)
//{
//    self_ = self;
//}

void XDTcpConnection::connectDestroyed()
{
    loop_->checkInLoopThread();
    XDLOG_minfo("[TcpConnection] destroy");
    if (state_connected == state_) {
        setState(state_connected);
        channel_->disableAll();
        //connectionCallBack_(self_);
        connectionCallBack_(shared_from_this());
    }
    channel_->remove();
}

void XDTcpConnection::send(const void *message, int32 len)
{
    if (state_connected == state_) {
        if (loop_->isInLoopThread()) {
            sendInLoop(message, len);
        } else {
            loop_->queueInLoop(std::bind(&XDTcpConnection::sendInLoop, this, message, len));
        }
    }
}

void XDTcpConnection::sendInLoop(const void *message, int32 len)
{
    loop_->checkInLoopThread();
    int32 nwrote = 0;
    int32 remaining = len;
    bool faultError = false;
    if (state_ == state_disconnected) {
        XDLOG_merror("[TcpConnection] 连接已经关闭, 无法发送");
        return;
    }
    if (!channel_->isWriting() && outputBuffer_.readableBytes() == 0) {
        // 发送缓冲区没有数据
        // 直接想socket 写
        nwrote = XDSocketOpt::write(channel_->fd(), message, len);
        if (nwrote >= 0) {
            remaining = len - nwrote;
            if (0 == remaining && writeCompleteCallBack_) {
                // 发送完毕
               // loop_->queueInLoop(std::bind(writeCompleteCallBack_, self_));
                loop_->queueInLoop(std::bind(writeCompleteCallBack_, shared_from_this()));
            }
        } else {
            nwrote = 0;
            if (errno != EWOULDBLOCK) {
                XDLOG_merror("[TcpConnection] sendInLoop");
                if (errno == EPIPE || errno == ECONNRESET) {
                    faultError = true;
                }
            }
        }
    }
    check(remaining <= len);
    if (!faultError && remaining > 0) {
        // 还有数据没发送
        // 写入发送缓冲区
        int oldLen = outputBuffer_.readableBytes();
        if (oldLen + remaining >= highWaterMark_ &&
            oldLen < highWaterMark_ &&
            highWateMarkCallBack_) {
            //loop_->queueInLoop(std::bind(highWateMarkCallBack_, self_, oldLen + remaining));
            loop_->queueInLoop(std::bind(highWateMarkCallBack_, shared_from_this(), oldLen + remaining));
        }
        outputBuffer_.append(static_cast<const char*>(message) + nwrote, remaining);
        if (!channel_->isWriting()) {
            channel_->setEvent(XDIOEventType_WRITE, true);
        }
    }
}

void XDTcpConnection::shutdown()
{
    if (state_connected == state_) {
        setState(state_disconnecting);
        loop_->runInLoop(std::bind(&XDTcpConnection::shutdownInLoop, this));
    }
}

void XDTcpConnection::shutdownInLoop()
{
    loop_->checkInLoopThread();
    if (!channel_->isWriting()) {
        socket_->shutdownWrite();
    }
}

void XDTcpConnection::forceClose()
{
    if (state_connected == state_ || state_disconnecting == state_) {
        setState(state_disconnecting);
        loop_->runInLoop(std::bind(&XDTcpConnection::shutdownInLoop, this));
    }
}

void XDTcpConnection::forceCloseInLoop()
{
    loop_->checkInLoopThread();
    if (state_connected == state_ || state_disconnecting == state_) {
        handleClose();
    }
}

const char* XDTcpConnection::stateToStr() const
{
    switch (state_) {
        case state_disconnected:
            return "disconnected";
            break;
        case state_connecting:
            return "connecting";
            break;
        case state_connected:
            return "connected";
            break;
        case state_disconnecting:
            return "disconnecting";
            break;
        default:
            return "unknow state";
            break;
    }
}

void XDTcpConnection::setState(XDTcpConnection::XDState state)
{
    state_ = state;
}

// IO 事件处理
void XDTcpConnection::handleRead(uint64 timestamp)
{
    loop_->checkInLoopThread();
    int32 error = 0;
    int32 n = inputBuffer_.readFD(channel_->fd(), &error);
    if (n > 0) {
        // have message
       // messageCallBack_(self_, &inputBuffer_, timestamp);
        messageCallBack_(shared_from_this(), &inputBuffer_, timestamp);
    } else if (n == 0) {
        // close
        handleClose();
    } else {
        // error
        errno = error;
        XDLOG_merror("[TcpConnection] handleRead error");
        handleError();
    }
}

void XDTcpConnection::handleWrite()
{
    loop_->checkInLoopThread();
    if (channel_->isWriting()) {
        int32 n = XDSocketOpt::write(channel_->fd(), outputBuffer_.peek(), outputBuffer_.readableBytes());
        if (n > 0) {
            // write ok
            outputBuffer_.retrieve(n);
            if (0 == outputBuffer_.readableBytes()) {
                channel_->setEvent(XDIOEventType_WRITE, false);
                if (writeCompleteCallBack_) {
                    //loop_->queueInLoop(std::bind(writeCompleteCallBack_, self_));
                    loop_->queueInLoop(std::bind(writeCompleteCallBack_, shared_from_this()));
                }
                if (state_ == state_disconnecting) {
                    shutdownInLoop();
                }
            }
        } else {
            XDLOG_merror("[TcpConnection] handleWrite error");
        }
    } else {
        XDLOG_merror("[TcpConnection] connection id=%d 已经关闭或者不能写", channel_->fd());
    }
}

void XDTcpConnection::handleClose()
{
    loop_->checkInLoopThread();
    XDLOG_minfo("[TcpConnection] close fd=%d state=%s", channel_->fd(), stateToStr());
    check(state_ == state_connected || state_ == state_disconnecting);
    setState(state_disconnected);
    channel_->disableAll();
    //XDTcpConnectionPtr guardThis(self_);
    XDTcpConnectionPtr guardThis(shared_from_this());
    connectionCallBack_(guardThis);
    closeCallBack_(guardThis);
}

void XDTcpConnection::handleError()
{
    int32 err = XDSocketOpt::getSockError(channel_->fd());
    XDLOG_merror("[TcpConnection] handleError [%s] - SO_ERROR = %d:%s", name_.c_str(), err, strerror(err));
}
