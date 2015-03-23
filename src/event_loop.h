#ifndef XD_EVENT_LOOP_H
#define XD_EVENT_LOOP_H

#include "noncopyable.h"
#include "thread.h"
#include "poller.h"
#include "shared_pointer.h"
#include "callback.h"
#include "types.h"

#include <vector>

class XDChannel;

// IO事件循环
class XDIOEventLoop : public XDNoncopyable
{
public:
    XDIOEventLoop();
    ~XDIOEventLoop();
    // 主循环 会调用poller->poll来获取IO事件
    void loop();
    // 退出
    void quit();
    
    
    // 更新channel
    void updateChannel(XDChannel *channel);
    // 移除channel
    void removeChannel(XDChannel *channel);
    // 判断是否还要channel
    bool hasChannel(XDChannel *channel);
    
    // 唤醒
    void wakeup();
    
    // 获取和设置context
    void setContext(void *value);
    void *getContext();
    
    // 来自相同线程中运行
    void runInLoop(XDSharedPtr<XDFunction> cb);
    // 来自其他线程
    void queueInLoop(XDSharedPtr<XDFunction> cb);
    // 判断是否在事件循环线程
    FORCEINLINE bool isInLoopThread()
    {
        return threadID_ == XDThread::getCurrentThreadID();
    }

    // 获取当前线程中EVENTLOOP对象
    static XDIOEventLoop* getEventLoopOfCurrentThread();

    void checkInLoopThread();
private:
    void doPendingFunctors();
    void printActiveChannels();
    void abortNotInLoopThread();
private:
    uint32 threadID_;
    // 事件循环
    XDSharedPtr<XDPoller> poller_;
    // 唤醒FD
    FD wakeupFd_;
    // 唤醒通道
    XDSharedPtr<XDChannel> wakeupChannel_;
    // 存储内容
    void *context_;
    // 在主循环中
    bool looping_;
    // 处理事件中
    bool eventHandleing_;
    // 是否要处理等待对了
    bool callingPendingFunctors_;
    // poll返回时间时间戳
    uint64 pollReturnTimestamp_;
    // 退出
    bool quit_;
    // 激活channel 集合
    std::vector<XDChannel*> activeChannels_;
    XDChannel* currentActiveChannel_;
    // mutex
    XDMutex mutex_;
    // 存储来自其他线程的处理器
    std::vector<XDSharedPtr<XDFunction> > pendingFunctors_;
};

#endif // end xd_event_loop_h
