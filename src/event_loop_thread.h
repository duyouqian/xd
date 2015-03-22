#ifndef XD_EVENT_LOOP_THREAD_H
#define XD_EVENT_LOOP_THREAD_H

#include "thread.h"
#include "event_loop.h"
#include "types.h"

class XDIOEventLoopThread : public XDIRunnable
{
public:
    XDIOEventLoopThread(XDIOEventLoopThreadInitCallBackPtr &cb,
                        const char *name);
    ~XDIOEventLoopThread();
    XDIOEventLoop* startLoop();
    // thread run interface
    bool init();
    uint32 run();
    void stop();
    void exit();
private:
    XDIThread *thread_;
    std::string name_;
    XDIOEventLoop *loop_;
    bool isExiting_;
    XDIOEventLoopThreadInitCallBackPtr initThreadCallBack_;
};

#endif // end xd_event_loop_thread_h
