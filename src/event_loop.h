#ifndef XD_EVENT_LOOP_H
#define XD_EVENT_LOOP_H

#include "noncopyable.h"
//#include "poller.h"
#include "types.h"

// IO事件循环
class XDIOEventLoop : public XDNoncopyable
{
public:
    XDIOEventLoop();
    ~XDIOEventLoop();
    // 主循环 会调用poller->poll来获取IO事件
    void loop();
    void checkInLoopThread();
private:
    //XDPoller *poller_;
    uint32 threadID_;
};

#endif // end xd_event_loop_h
