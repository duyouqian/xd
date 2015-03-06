#ifndef XD_BASE_CONDITION_H
#define XD_BASE_CONDITION_H

#include "noncopyable.h"
#include "types.h"

class XDICondition : public XDNoncopyable
{
public:
    // 等待
    virtual XDErrorCode wait() = 0;
    // 等待 超时(毫米)
    virtual XDErrorCode timedwait(uint32 millisecond) = 0;
    // 通知
    virtual XDErrorCode signal() = 0;
    // 广播
    virtual XDErrorCode broadcast() = 0;
    // 是否有效
    virtual bool isValid() const = 0;
};

#endif // xd_condition_h
