#ifndef XD_EPOLL_H
#define XD_EPOLL_H

#include "base_poll.h"

class XDIOEventLoop;
class XDEPoller : public XDBasePoller
{
public:
    explicit XDEPoller(XDIOEventLoop *loop);
    ~XDEPoller();
    uint64 poll(int32 timeoutMS,
                std::vector<XDChannel*> *activeChannels);
    bool addChannel(XDChannel *channel);
    bool updateChannel(XDChannel *channel);
    bool removeChannel(XDChannel *channel);
private:
    void fillActiveChannels(int32 numEvents,
                            std::vector<XDChannel*> *activeChannels);
    void update(int32 oper, XDChannel *channel);
    static const char* operToString(int32 op);
private:
    int epollfd_;
    std::vector<struct epoll_event> events_;
};

#endif // end xd_epoll_h
