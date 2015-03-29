#ifndef XD_EVENT_LOOP_THREAD_H
#define XD_EVENT_LOOP_THREAD_H

#include "thread.h"
#include "types.h"
#include <string>
#include <functional>

class XDIOEventLoop;

class XDIOEventLoopThread : public XDIRunnable
{
typedef std::function<void(XDIOEventLoop*)> XDIOEventLoopThreadInitCallBack;
public:
    XDIOEventLoopThread(const XDIOEventLoopThreadInitCallBack &cb,
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
    XDIOEventLoopThreadInitCallBack initThreadCallBack_;
};

#endif // end xd_event_loop_thread_h
