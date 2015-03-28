#include "log.h"
#include "timer.h"
#include "poller.h"
#include "channel.h"
#include "event_loop.h"
#include "callback.h"
#include "socket_util.h"
#include <algorithm>
#include <functional>
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

XDIOEventLoop* XDIOEventLoop::getEventLoopOfCurrentThread()
{
    return loopInThisThread;
}

XDIOEventLoop::XDIOEventLoop()
             : threadID_(XDThread::getCurrentThreadID())
             , poller_(new XDPoller(this))
             , wakeupFd_(createEventfd())
             , wakeupChannel_(new XDChannel(this, wakeupFd_))
             , context_(NULL)
             , looping_(false)
             , eventHandleing_(false)
             , callingPendingFunctors_(false)
             , quit_(false)
{
    XDLOG_minfo("[IOEventLoop] 创建IOEventLoop");
    if (loopInThisThread) {
        XDLOG_mfatal("[IOEventLoop] 已经创建了IOEventLoop");
    } else {
        loopInThisThread = this;
    }
    wakeupChannel_->setReadCallBack(std::bind(&XDIOEventLoop::weakupHandleRead, this, std::placeholders::_1));
    wakeupChannel_->setEvent(XDIOEventType_READ, true);
}

XDIOEventLoop::~XDIOEventLoop()
{
    XDLOG_minfo("[IOEventLoop] IOEventLoop");
    wakeupChannel_->disableAll();
    wakeupChannel_->remove();
    XDSocketOpt::close(wakeupFd_);
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
        printActiveChannels();
        eventHandleing_ = true;
        for (std::vector<XDChannel*>::iterator it = activeChannels_.begin();
        it != activeChannels_.end();
        ++it) {
            currentActiveChannel_ = *it;
            currentActiveChannel_->handleEvent(pollReturnTimestamp_);
        }
        currentActiveChannel_ = NULL;
        eventHandleing_ = false;
        // 执行其他线程过来的
        doPendingFunctors();
    }
    XDLOG_minfo("[IOEventLoop] loop stop threadID:%u", threadID_);
    looping_ = false;
}

void XDIOEventLoop::quit()
{
    quit_ = true;
    XDLOG_minfo("[IOEventLoop] 退出 threadID:%u", XDThread::getCurrentThreadID());
    if (!isInLoopThread()) {
        wakeup();
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

void XDIOEventLoop::wakeup()
{
    XDLOG_minfo("[IOEventLoop] 唤醒");
    uint64 one = 1;
    int32 n = XDSocketOpt::write(wakeupFd_, &one, sizeof(one));
    if (n != sizeof(one)) {
        XDLOG_merror("[WakeupHandRead] reads:%d bytes instead of 8", n);
    }
}

void XDIOEventLoop::runInLoop(const XDIOEventCallBack &cb)
{
    if (isInLoopThread()) {
        if (cb) {
            cb();
        }
    } else {
        queueInLoop(cb);
    }
}

void XDIOEventLoop::queueInLoop(const XDIOEventCallBack &cb)
{
    {
        XDGuardMutex lock(&mutex_);
        pendingFunctors_.push_back(cb);
    }
    if (!isInLoopThread() || callingPendingFunctors_) {
        wakeup();
    }
}

void XDIOEventLoop::printActiveChannels()
{
    for (std::vector<XDChannel*>::const_iterator it = activeChannels_.begin();
         it != activeChannels_.end(); ++it)
    {
        const XDChannel* ch = *it;
        XDLOG_minfo("{%s}", ch->reventsToString().c_str());
    }
}

void XDIOEventLoop::doPendingFunctors()
{
    std::vector<XDIOEventCallBack> temp;
    callingPendingFunctors_ = true;
    {
        XDGuardMutex lock(&mutex_);
        temp.swap(pendingFunctors_);
    }
    for (int32 i = 0; i < temp.size(); ++i) {
        temp[i]();
    }
    callingPendingFunctors_ = false;
}

void XDIOEventLoop::checkInLoopThread()
{
    if (!isInLoopThread()) {
        abortNotInLoopThread();
    }
}

void XDIOEventLoop::setContext(void *value)
{
    context_ = value;
}

void* XDIOEventLoop::getContext()
{
    return context_;
}

void XDIOEventLoop::weakupHandleRead(uint64 timestamp)
{
    XDLOG_minfo("[WakeupHandleRead] 有线程唤醒IOEventLoop线程");
    uint64 one = 1;
    int32 n = XDSocketOpt::read(wakeupFd_, &one, sizeof(one));
    if (n != sizeof(one)) {
        XDLOG_merror("[WakeupHandleRead] reads:%d bytes instead of 8", n);
    }
}

void XDIOEventLoop::abortNotInLoopThread()
{
    XDLOG_mfatal("IOEventLoop::abortNotInLoopThread - IOEventLoop 已经创建 线程ID=%u, 当前线程ID=%u", threadID_, XDThread::getCurrentThreadID());
    abort();
}
