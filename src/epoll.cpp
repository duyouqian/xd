#include "base_poll.h"
#include "event_loop.h"
#include "channel.h"
#include "log.h"
#include "timer.h"
#include "socket_util.h"

#include <sys/epoll.h>

const uint32 INITEVENTLISTSIZE = 16;

const int32 NEW = -1;
const int32 ADD = 1;
const int32 DEL = 2;

XDEPoller::XDEPoller(XDIOEventLoop *loop)
         : XDBasePoller(loop)
         , epollfd_(::epoll_create(EPOLL_CLOEXEC))
         , events_(INITEVENTLISTSIZE)
{
    if (epollfd_ < 0) {
        XDLOG_mfatal("[EPoll] 创建EPoll失败");
    }
}

XDEPoller::~XDEPoller()
{
    XDSocketOpt::close(epollfd_);
}

uint64 XDEPoller::poll(int32 timeoutMS,
                       std::vector<XDChannel*> *activeChannels)
{
    XDLOG_minfo("[EPoll] [POLL] 总共有%dchnnel", channels_.size());
    int32 numEvents = ::epoll_wait(epollfd_,
                                   &*events_.begin(),
                                   static_cast<int32>(events_.size()),
                                   timeoutMS);
    int saveErrno = errno;
    uint64 now = XDTimer::now();
    if (numEvents > 0) {
        XDLOG_minfo("[EPoll] [POLL] 有事件发生");
        fillActiveChannels(numEvents, activeChannels);
        // 扩充接受事件容量
        if (static_cast<size_t>(numEvents) == events_.size()) {
            events_.resize(numEvents << 1);
        }
    } else if (numEvents == 0) {
        XDLOG_minfo("[EPoll] [POLL] 没有任何事件发生");
    } else {
        // 发生错误
        if (saveErrno != EINTR) {
            errno = saveErrno;
            XDLOG_merror("[EPoll] [POLL] poll 错误:%d %s", errno, strerror(errno));
        }
    }
    return now;
}

bool XDEPoller::addChannel(XDChannel *channel)
{
    XDLOG_mwarn("[EPOLL] addChannel 未实现");
    return true;
}

bool XDEPoller::updateChannel(XDChannel *channel)
{
    XDBasePoller::checkInLoopThread();
    const int32 index = channel->index();
    const FD fd = channel->fd();
    XDLOG_minfo("[EPOLL] fd=%d, events=%d, index=%d", fd, channel->events(), index);
    if (index == NEW || index == DEL) {
        if (NEW == index) {
            // 新增
            check(channels_.find(fd) == channels_.end());
            channels_[fd] = channel;
        } else {
            check(channels_.find(fd) != channels_.end());
            check(channels_[fd] == channel);
        }
        channel->setIndex(ADD);
        update(EPOLL_CTL_ADD, channel);
    } else {
        // modify
        check(channels_.find(fd) != channels_.end());
        check(channels_[fd] == channel);
        check(index == ADD);
        if (channel->isNoneEvent()) {
            // del
            update(EPOLL_CTL_DEL, channel);
            channel->setIndex(DEL);
        } else {
            update(EPOLL_CTL_MOD, channel);
        }
    }
    return true;
}

bool XDEPoller::removeChannel(XDChannel *channel)
{
    XDBasePoller::checkInLoopThread();
    const int32 index = channel->index();
    const FD fd = channel->fd();
    XDLOG_minfo("[EPOLL] fd=%d, events=%d, index=%d", fd, channel->events(), index);
    check(channels_.find(fd) != channels_.end());
    check(channels_[fd] == channel);
    check(index == ADD || index == DEL);
    int32 n = channels_.erase(fd);
    check(n == 1);
    if (index == ADD) {
        update(EPOLL_CTL_DEL, channel);
    }
    channel->setIndex(NEW);
}

void XDEPoller::fillActiveChannels(int32 numEvents,
                                   std::vector<XDChannel*> *activeChannels)
{
    check(static_cast<size_t>(numEvents) <= events_.size());
    for (int i = 0; i < numEvents; ++i) {
        XDChannel *channel = static_cast<XDChannel*>(events_[i].data.ptr);
        int32 event = XDIOEventType_NONE;
        int32 epollEvent = events_[i].events;
        if ((epollEvent & EPOLLHUP) && !(epollEvent & EPOLLIN)) {
            // 关闭
            event |= XDIOEventType_CLOSE;
        }
        if (epollEvent & EPOLLOUT) {
            // 写
            event |= XDIOEventType_WRITE;
        }
        if (epollEvent & (EPOLLIN | EPOLLPRI | EPOLLRDHUP)) {
            // 读
            event |= XDIOEventType_READ;
        }
        if (epollEvent & (EPOLLERR)) {
            // 错误
            event |= XDIOEventType_ERROR;
        }
        // check
        FD fd = channel->fd();
        std::map<FD, XDChannel*>::const_iterator it = channels_.find(fd);
        check(it != channels_.end());
        check(it->second == channel);
        channel->setRevents(event);
        activeChannels->push_back(channel);
    }
}

void XDEPoller::update(int32 oper, XDChannel *channel)
{
    struct epoll_event ev;
    bzero(&ev, sizeof(ev));
    int32 evt = XDIOEventType_NONE;
    int32 channelEvent = channel->events();
    if (channelEvent & XDIOEventType_READ) {
        evt |= (EPOLLIN | EPOLLPRI);
    }
    if (channelEvent & XDIOEventType_WRITE) {
        evt |= EPOLLOUT;
    }
    ev.events = evt;
    ev.data.ptr = channel;
    FD fd = channel->fd();
    XDLOG_minfo("[EPoll] [update] epoll_ctl op=%d fd=%d event=%d", oper, fd, channelEvent);
    if (-1 == ::epoll_ctl(epollfd_, oper, fd, &ev)) {
        // 注册epoll失败
        XDLOG_merror("[EPoll] [update] epoll_ctl op=%d fd=%d event=%d 注册失败", oper, fd, channelEvent);
    } else {
    }
}
