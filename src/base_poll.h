#ifndef XD_BASE_POLL_H
#define XD_BASE_POLL_H

#include "noncopyable.h"
//#include "event_loop.h"
//#include "channel.h"
#include "types.h"
#include <vector>
#include <map>

class XDIOEventLoop;
class XDChannel;
// IO多路复用接口
class XDBasePoller : public XDNoncopyable
{
public:
    explicit XDBasePoller(XDIOEventLoop *loop);
    virtual ~XDBasePoller();

    virtual uint64 poll(int32 timeoutMs, 
                        std::vector<XDChannel*> *activeChannels) = 0;
    virtual bool addChannel(XDChannel *channel) = 0;
    virtual bool updateChannel(XDChannel *channel) = 0;
    virtual bool removeChannel(XDChannel *channel) = 0;
    virtual bool hasChannel(XDChannel *channel) const;

    void checkInLoopThread() const;

protected:
    XDIOEventLoop *loop_;
    std::map<FD, XDChannel*> channels_;
};


#endif // end xd_base_poll_h
