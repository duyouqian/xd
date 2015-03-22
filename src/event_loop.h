#ifndef XD_EVENT_LOOP_H
#define XD_EVENT_LOOP_H

#include "noncopyable.h"
#include "thread.h"
#include "poller.h"
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
    void weakup();
    // 判断是否在事件循环线程
    FORCEINLINE bool isInLoopThread()
    {
        return threadID_ == XDThread::getCurrentThreadID();
    }

    // 获取当前线程中EVENTLOOP对象
    static XDIOEventLoop* getEventLoopOfCurrentThread();

    void checkInLoopThread();
private:
    uint32 threadID_;
    // 事件循环
    XDPoller *poller_;
    // 唤醒FD
    FD wakeupFd_;
    // 唤醒通道
    XDChannel *wakeupChannel_;
    // 在主循环中
    bool looping_;
    // 处理事件中
    bool eventHandleing_;
    // poll返回时间时间戳
    uint64 pollReturnTimestamp_;
    // 退出
    bool quit_;
    // 激活channel 集合
    std::vector<XDChannel*> activeChannels_;
    XDChannel* currentActiveChannel_;
};

#endif // end xd_event_loop_h
