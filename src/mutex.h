#ifndef XD_MUTEX_H
#define XD_MUTEX_H

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

// 栈上互斥量
class XDGuardMutex : public XDNoncopyable
{
public:
    explicit XDGuardMutex(XDIMutex *mutex);
    ~XDGuardMutex();
    bool isLocked() const;
private:
    XDIMutex *mutex_;
    bool isLocked_;
};

#ifdef __unix__
    #include "mutex_linux.h"
#endif

#endif // end xd_mutex_h
