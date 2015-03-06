#ifndef XD_BASE_SEMA_H
#define XD_BASE_SEMA_H

#include "noncopyable.h"
#include "types.h"

class XDISemaphore : public XDNoncopyable
{
public:
    // 等待
    virtual XDErrorCode wait() = 0;
    // 等待 超时(毫米)
    virtual XDErrorCode timedwait(uint32 millisecond) = 0;
    // 通知
    virtual XDErrorCode signal() = 0;
    // 是否有效
    virtual bool isValid() const = 0;
};

#endif // end xd_base_sema_h
