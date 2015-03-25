#ifndef XD_CHANNEL_H
#define XD_CHANNEL_H

#include "noncopyable.h"
//#include "event_loop.h"
#include "callback.h"
#include "shareable.h"
#include "types.h"
#include <string>

class XDIOEventLoop;
// 处理FD上的IO事件和改变
class XDChannel : public XDShareable
{
public:
    XDChannel(XDIOEventLoop *loop,
              FD fd);
    ~XDChannel();

    // 设置IO 读写关闭错误回调处理
    void setIOEventCallBack(const XDIOEventCallBackPtr &cb);
    //void setReadCallBack(const XDIOEventReadCallBackPtr &cb);
    //void setWriteCallBack(const XDIOEventCallBackPtr &cb);
    //void setCloseCallBack(const XDIOEventCallBackPtr &cb);
    //void setErrorCallBack(const XDIOEventCallBackPtr &cb);

    // 设置 读写关闭错误事件
    void setEvent(uint32 type, bool on);
    // 取消全部事件
    void disableAll();
    // 是否为空事件
    FORCEINLINE bool isNoneEvent()
    {
        return XDIOEventType_NONE == event_;
    }
    // 处理事件
    void handleEvent(uint64 timestamp);
    // 移除LOOP中的自己
    void remove();
    // 回去自己的eventloop
    XDIOEventLoop* ownerLoop();

    const FD fd() const;
    const int32 index() const;
    void setIndex(int32 newIndex);

    const int32 events() const;
    void setRevents(int32 value);
    
    std::string reventsToString() const;
    std::string eventsToString() const;

private:
    void update();
    static std::string eventsToString(int32 fd, int32 ev);

private:
    XDIOEventLoop *loop_;
    const FD fd_;
    int32 index_;
    uint32 event_;
    uint32 revent_;
    bool addToLoop_;
    bool eventHandleing_;
    // 回调接口
    XDIOEventCallBackPtr ioeventCallBack_;
//    XDIOEventReadCallBackPtr readCallBack_;
//    XDIOEventCallBackPtr writeCallBack_;
//    XDIOEventCallBackPtr closeCallBack_;
//    XDIOEventCallBackPtr errorCallBack_;
};

#endif // end xd_channel_h
