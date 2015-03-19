#ifndef XD_CHANNEL_H
#define XD_CHANNEL_H

#include "noncopyable.h"
#include "event_loop.h"
#include "callback.h"
#include "types.h"

// 处理FD上的IO事件和改变
class XDChannel : public XDNoncopyable
{
public:
    XDChannel(XDIOEventLoop *loop,
              FD fd);
    ~XDChannel();

    // 设置IO 读写关闭错误回调处理
    void setReadCallBack(const XDIOEventReadCallBackPtr &cb);
    void setWriteCallBack(const XDIOEventCallBackPtr &cb);
    void setCloseCallBack(const XDIOEventCallBackPtr &cb);
    void setErrorCallBack(const XDIOEventCallBackPtr &cb);

    // 设置 读写关闭错误事件
    void setEvent(uint32 type, bool on);
    // 处理事件
    void hasEvent(uint64 timestamp);

    const FD getFd() const;

private:
    void update();


private:
    XDIOEventLoop *loop_;
    const FD fd_;
    uint32 event_;
    XDIOEventReadCallBackPtr readCallBack_;
    XDIOEventCallBackPtr writeCallBack_;
    XDIOEventCallBackPtr closeCallBack_;
    XDIOEventCallBackPtr errorCallBack_;
};

#endif // end xd_channel_h
