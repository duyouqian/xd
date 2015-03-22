#include "event_loop_thread_pool.h"
#include <stdio.h>

XDIOEventLoopThreadPool::XDIOEventLoopThreadPool(XDIOEventLoop *baseLoop,
                                                 const char *name)
                                                 : baseLoop_(baseLoop)
                                                 , name_(name)
                                                 , started_(false)
                                                 , numThread_(0)
                                                 , nextIndex_(0)
{
}

XDIOEventLoopThreadPool::~XDIOEventLoopThreadPool()
{
}

void XDIOEventLoopThreadPool::setNumThread(uint32 num)
{
    numThread_ = num;
}

void XDIOEventLoopThreadPool::start(const XDIOEventLoopThreadInitCallBackPtr &cb)
{
    check(!started_);
    baseLoop_->checkInLoopThread();
    started_ = true;
    for (int i = 0; i < numThread_; ++i) {
        char nameBuf[name_.size() + 32];
        snprintf(nameBuf, sizeof(nameBuf), "%s-%d", name_.c_str(), i);
        XDIOEventLoopThread *t = new XDIOEventLoopThread(cb, nameBuf);
        threads_.push_back(t);
        loops_.push_back(t->startLoop());
    }
    if (0 == numThread_ && cb.isValid()) {
        cb->exec(baseLoop_);
    }
}

XDIOEventLoop* XDIOEventLoopThreadPool::getNextLoop()
{
    baseLoop_->checkInLoopThread();
    check(started_);
    XDIOEventLoop* loop = baseLoop_;
    if (!loops_.empty()) {
        loop = loops_[nextIndex_];
        ++nextIndex_;
        if (nextIndex_ >= loops_.size()) {
            nextIndex_ = 0;
        }
    }
    return loop;
}

std::vector<XDIOEventLoop*> XDIOEventLoopThreadPool::getAllLoops()
{
    baseLoop_->checkInLoopThread();
    check(started_);
    if (loops_.empty()) {
        return std::vector<XDIOEventLoop*>(1, baseLoop_);
    }
    return loops_;
}

bool XDIOEventLoopThreadPool::started() const
{
    return started_;
}

const std::string& XDIOEventLoopThreadPool::name() const
{
    return name_;
}
