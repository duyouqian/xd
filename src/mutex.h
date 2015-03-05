#ifndef XD_MUTEX_H
#define XD_MUTEX_H

#ifdef __unix__
    // unix or linux
    #include "mutex_linux.h"
    typedef XDMutexLinuxImp XDMutex;
#elif __APPLE__
    // mac os
#else
    // win
#endif

// 栈上互斥量
class XDGuardMutex : public XDNoncopyable
{
public:
    explicit XDGuardMutex(XDMutex *mutex);
    ~XDGuardMutex();
    bool isLocked() const;
private:
    XDMutex *mutex_;
    bool isLocked_;
};

#endif // end xd_mutex_h
