#include "channel.h"
#include "event_loop.h"
#include <sstream>

XDChannel::XDChannel(XDIOEventLoop *loop,
                     FD fd)
         : loop_(loop)
         , fd_(fd)
         , index_(-1)
         , event_(XDIOEventType_NONE)
         , revent_(XDIOEventType_NONE)
         , addToLoop_(false)
         , eventHandleing_(false)
{
}

XDChannel::~XDChannel()
{
    check(!eventHandleing_);
    check(!addToLoop_);
    if (loop_->isInLoopThread()) {
        check(!loop_->hasChannel(this));
    }
}

const FD XDChannel::fd() const
{
    return fd_;
}

const int32 XDChannel::index() const
{
    return index_;
}

const int32 XDChannel::events() const
{
    return event_;
}

void XDChannel::setIndex(int32 newIndex)
{
    index_ = newIndex;
}

void XDChannel::setRevents(int32 value)
{
    revent_ = value;
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

void XDChannel::disableAll()
{
    event_ = XDIOEventType_NONE;
    update();
}

void XDChannel::handleEvent(uint64 timestamp)
{
    eventHandleing_ = true;
    if ((revent_& XDIOEventType_READ) && readCallBack_.isValid()) {
        // 读事件
        readCallBack_->exec(timestamp);
    }
    if ((revent_ & XDIOEventType_WRITE) && writeCallBack_.isValid()) {
        // 写事件
        writeCallBack_->exec();
    }
    if ((revent_& XDIOEventType_ERROR) && errorCallBack_.isValid()) {
        // 错误事件
        errorCallBack_->exec();
    }
    if ((revent_& XDIOEventType_CLOSE) && closeCallBack_.isValid()) {
        // 关闭事件
        closeCallBack_->exec();
    }
    eventHandleing_ = false;
}

void XDChannel::update()
{
    addToLoop_ = true;
    loop_->updateChannel(this);
}

void XDChannel::remove()
{
    check(isNoneEvent());
    addToLoop_ = false;
    loop_->removeChannel(this);
}

XDIOEventLoop* XDChannel::ownerLoop()
{
    return loop_;
}

std::string XDChannel::reventsToString() const
{
    return eventsToString(fd_, revent_);
}

std::string XDChannel::eventsToString() const
{
    return eventsToString(fd_, event_);
}

std::string XDChannel::eventsToString(int32 fd, int32 ev)
{
    std::ostringstream oss;
    oss << "fd=" << fd << ": ";
    if (ev & XDIOEventType_READ)
        oss << "READ ";
    if (ev & XDIOEventType_WRITE)
        oss << "WRITE";
    if (ev & XDIOEventType_ERROR)
        oss << "ERROR";
    if (ev & XDIOEventType_CLOSE)
        oss << "CLOSE";
    return oss.str().c_str();
}
