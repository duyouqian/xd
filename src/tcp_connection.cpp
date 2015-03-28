#include "tcp_connection.h"
#include "log.h"

// IO 事件处理
void XDTcpConnection::handleRead(int64 timestamp)
{
    loop_->checkInLoopThread();
    int32 error = 0;
    int32 n = inputBuffer_->readFD(channel_->fd(), &error);
    if (n > 0) {
        // have message
        messageCallback_(self_, &inputBuffer_, timestamp);
    } else if (n == 0) {
        // close
        handleClose();
    } else {
        // error
        errno = error;
        XDLOG_merror("[TcpConnection] handleRead error");
        handError();
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
                    loop_->queueInLoop(std::bind(writeCompleteCallBack_, self_));
                }
                if (state_ == disconnecting) {
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
    XDLOG_minfo("[TcpConnection] fd=%d state=%s", channel_->fd(), stateToStr());
    check(state_ == connected || state_ == disconnecting);
    setState(disconnected);
    channel_->disableAll();
    XDTcpConnectionPtr guardThis(self_);
    connectionCallback_(guardThis);
    closeCallback_(guardThis);
}

void XDTcpConnection::handleError()
{
    int32 err = XDSocketOpt::getSockError(channel_->fd());
    XDLOG_merror("[TcpConnection] handleError [%s] - SO_ERROR = %d:%s", name_->c_str(), err, strerror(err));
}
