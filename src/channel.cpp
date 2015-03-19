#include "channel.h"

XDChannel::XDChannel(XDIOEventLoop *loop,
                     FD fd)
         : loop_(loop)
         , fd_(fd)
         , event_(XDIOEventType_NONE)
{
}

XDChannel::~XDChannel()
{
}

const FD XDChannel::getFd() const
{
    return fd_;
}

void XDChannel::setReadCallBack(const XDIOEventReadCallBackPtr &cb)
{
    readCallBack_ = cb;
}

void XDChannel::setWriteCallBack(const XDIOEventCallBackPtr &cb)
{
    writeCallBack_ = cb;
}

void XDChannel::setCloseCallBack(const XDIOEventCallBackPtr &cb)
{
    closeCallBack_ = cb;
}

void XDChannel::setErrorCallBack(const XDIOEventCallBackPtr &cb)
{
    errorCallBack_ = cb;
}

void XDChannel::setEvent(uint32 type, bool on)
{
    if (on) {
        if (type & XDIOEventType_READ) {
            // 读事件
            event_ |= XDIOEventType_READ;
        }
        if (type & XDIOEventType_WRITE) {
            // 写事件
            event_ |= XDIOEventType_WRITE;
        }
        if (type & XDIOEventType_ERROR) {
            // 错误事件
            event_ |= XDIOEventType_ERROR;
        }
        if (type & XDIOEventType_CLOSE) {
            // 关闭事件
            event_ |= XDIOEventType_CLOSE;
        }
    } else {
        if (type & XDIOEventType_READ) {
            // 读事件
            event_ &= ~XDIOEventType_READ;
        }
        if (type & XDIOEventType_WRITE) {
            // 写事件
            event_ &= ~XDIOEventType_WRITE;
        }
        if (type & XDIOEventType_ERROR) {
            // 错误事件
            event_ &= ~XDIOEventType_ERROR;
        }
        if (type & XDIOEventType_CLOSE) {
            // 关闭事件
            event_ &= ~XDIOEventType_CLOSE;
        }
    }
    update();
}

void XDChannel::hasEvent(uint64 timestamp)
{
    if ((event_& XDIOEventType_READ) && readCallBack_.isValid()) {
        // 读事件
        readCallBack_->exec(timestamp);
    }
    if ((event_ & XDIOEventType_WRITE) && writeCallBack_.isValid()) {
        // 写事件
        writeCallBack_->exec();
    }
    if ((event_& XDIOEventType_ERROR) && errorCallBack_.isValid()) {
        // 错误事件
        errorCallBack_->exec();
    }
    if ((event_& XDIOEventType_CLOSE) && closeCallBack_.isValid()) {
        // 关闭事件
        closeCallBack_->exec();
    }
}

void XDChannel::update()
{
}
