#include "log.h"
#include "timer.h"
#include "poller.h"
#include "channel.h"
#include "event_loop.h"
#include "callback.h"
#include "socket_util.h"
#include <algorithm>
#include <sys/eventfd.h>

__thread XDIOEventLoop* loopInThisThread = NULL;
const int32 POLLTIMEOUTMS = 1000;

FD createEventfd()
{
    FD fd = ::eventfd(0, EFD_NONBLOCK);
    if (-1 == fd) {
        XDLOG_merror("[EPOLL] 创建eventfd失败");
        abort();
    }
    return fd;
}

class WakeupHandleRead : public XDIOEventReadCallBack
{
public:
    explicit WakeupHandleRead(FD fd) : fd_(fd)
    {
    }
    virtual ~WakeupHandleRead()
    {
    }
    virtual bool exec(uint64 timestamp)
    {
        uint64 one = 1;
        int32 n = XDSocketOpt::read(fd_, &one, sizeof(one));
        if (n != sizeof(one)) {
            XDLOG_merror("[WakeupHandRead] reads:%d bytes instead of 8", n);
        }
    }
private:
    FD fd_;
};

XDIOEventLoop* XDIOEventLoop::getEventLoopOfCurrentThread()
{
    return loopInThisThread;
}

XDIOEventLoop::XDIOEventLoop()
             : threadID_(XDThread::getCurrentThreadID())
             , poller_(new XDPoller(this))
             , wakeupFd_(createEventfd())
             , wakeupChannel_(new XDChannel(this, wakeupFd_))
             , looping_(false)
             , eventHandleing_(false)
             , quit_(false)
{
    XDLOG_minfo("[IOEventLoop] 创建IOEventLoop");
    if (loopInThisThread) {
        XDLOG_mfatal("[IOEventLoop] 已经创建了IOEventLoop");
    } else {
        loopInThisThread = this;
    }
    XDSharedPtr<XDIOEventReadCallBack> wakeupHandRead(new WakeupHandleRead(wakeupFd_));
    wakeupChannel_->setReadCallBack(wakeupHandRead);
    wakeupChannel_->setEvent(XDIOEventType_READ, true);
}

XDIOEventLoop::~XDIOEventLoop()
{
    XDLOG_minfo("[IOEventLoop] IOEventLoop");
    wakeupChannel_->setEvent(XDIOEventType_READ, false);
    wakeupChannel_->remove();
    XDSocketOpt::close(wakeupFd_);
    delete wakeupChannel_;
    delete poller_;
    loopInThisThread = NULL;
}

void XDIOEventLoop::loop()
{
    check(!looping_);
    XDLOG_minfo("[IOEventLoop] loop run threadID:%u", threadID_);
    looping_ = true;
    while (!quit_) {
        activeChannels_.clear();
        pollReturnTimestamp_ = poller_->poll(POLLTIMEOUTMS, &activeChannels_);
        // printf debug info
        eventHandleing_ = true;
        for (std::vector<XDChannel*>::iterator it = activeChannels_.begin();
        it != activeChannels_.end();
        ++it) {
            currentActiveChannel_ = *it;
            currentActiveChannel_->handleEvent(pollReturnTimestamp_);
        }
        currentActiveChannel_ = NULL;
        eventHandleing_ = false;
    }
    XDLOG_minfo("[IOEventLoop] loop stop threadID:%u", threadID_);
    looping_ = false;
}

void XDIOEventLoop::quit()
{
    quit_ = true;
    XDLOG_minfo("[IOEventLoop] 退出 threadID:%u", XDThread::getCurrentThreadID());
    if (!isInLoopThread()) {
        weakup();
    }
}

void XDIOEventLoop::updateChannel(XDChannel *channel)
{
    check(channel->ownerLoop() == this);
    checkInLoopThread();
    poller_->updateChannel(channel);
}

void XDIOEventLoop::removeChannel(XDChannel *channel)
{
    check(channel->ownerLoop() == this);
    checkInLoopThread();
    if (eventHandleing_) {
        check(channel == currentActiveChannel_ ||
              std::find(activeChannels_.begin(),
                        activeChannels_.end(),
                        channel)
                        == activeChannels_.end());
    }
    poller_->removeChannel(channel);
}

bool XDIOEventLoop::hasChannel(XDChannel *channel)
{
    check(channel->ownerLoop() == this);
    checkInLoopThread();
    return poller_->hasChannel(channel);
}

void XDIOEventLoop::weakup()
{
    XDLOG_minfo("[IOEventLoop] 唤醒");
}

void XDIOEventLoop::checkInLoopThread()
{
}
