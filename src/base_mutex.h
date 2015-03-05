#ifndef XD_BASE_MUTEX_H
#define XD_BASE_MUTEX_H

#include "noncopyable.h"
#include "types.h"

// 互斥量
class XDIMutex : public XDNoncopyable
{
public:
    // 加锁
    virtual XDErrorCode lock() = 0;
    // 加锁 超时(毫秒)
    virtual XDErrorCode timedlock(uint32 millisecond) = 0;
    // 解锁
    virtual XDErrorCode unlock() = 0;
};

#endif // end xd_mutex_h
