#ifndef XD_BASE_EVENT_H
#define XD_BASE_EVENT_h

#include "noncopyable.h"
#include "callback.h"
#include "types.h"

class XDIEvent : public XDNoncopyable
{
public:
    virtual bool create(bool isManualReset = false) = 0;
    virtual void trigger() = 0;
    virtual void reset() = 0;
    virtual void wait() = 0;
    virtual void wait(uint32 waitTime, bool ignoreThreadIdelState = false) = 0;
protected:
    typedef enum {
        TRIGGERED_NONE,
        TRIGGERED_ONE,
        TRIGGERED_ALL
    } TriggerType;
};



// 基础IOEvent 只提供读写错误类型事件
// 要想定义其他类型的事件 自己去扩张
class XDIOEvent : public XDNoncopyable
{
public:
    XDIOEvent();
    virtual ~XDIOEvent();
    virtual bool changeEvent(XDObjectOper oper, XDIOEventType event, XDIOEventCallBackPtr func);
    virtual bool handleEvent();
protected:
    uint32 event_;
    XDIOEventCallBackPtr readFunc_;
    XDIOEventCallBackPtr writeFunc_;
    XDIOEventCallBackPtr errorFunc_;
};

#endif // end xd_base_event_h
