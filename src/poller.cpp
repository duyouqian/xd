#include "poller.h"
#include "event_loop.h"
#include "channel.h"

XDBasePoller::XDBasePoller(XDIOEventLoop *loop)
        : loop_(loop)
{
}

XDBasePoller::~XDBasePoller()
{
}

bool XDBasePoller::hasChannel(XDChannel *channel) const
{
    checkInLoopThread();
    std::map<int32, XDChannel*>::const_iterator it = channels_.find(channel->fd());
    return channels_.end() != it && it->second == channel;
}

void XDBasePoller::checkInLoopThread() const
{
    loop_->checkInLoopThread();
}
