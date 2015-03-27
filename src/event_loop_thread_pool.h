#ifndef XD_EVENT_LOOP_THREAD_POOL_H
#define XD_EVENT_LOOP_THREAD_POOL_H

#include "noncopyable.h"
#include "callback.h"
#include "shareable.h"
#include "event_loop_thread.h"
#include "types.h"
#include <string>
#include <vector>

class XDIOEventLoopThreadPool : public XDShareable
{
public:
    XDIOEventLoopThreadPool(XDIOEventLoop *baseLoop,
                            const char *name);
    ~XDIOEventLoopThreadPool();
    void setNumThread(uint32 num);
    void start(const XDIOEventLoopThreadInitCallBack &cb);
    XDIOEventLoop* getNextLoop();
    XDIOEventLoop* getLoopForHash(int32 hash);
    std::vector<XDIOEventLoop*> getAllLoops();
    bool started() const;
    const std::string& name() const;

private:
    XDIOEventLoop *baseLoop_;
    std::string name_;
    bool started_;
    uint32 numThread_;
    int32 nextIndex_;
    std::vector<XDIOEventLoopThread*> threads_;
    std::vector<XDIOEventLoop*> loops_;
};

#endif // end xd_event_loop_thread_pool_h
