#ifndef XD_MUTEXIMP_LINUX_H
#define XD_MUTEXIMP_LINUX_H

#include "mutex.h"
#include "types.h"

#include <pthread.h>

class XDCondition;

class XDMutexLinuxImp : public XDIMutex
{
public:
    XDMutexLinuxImp();
    ~XDMutexLinuxImp();
    XDErrorCode lock();
    XDErrorCode timedlock(uint32 millisecond);
    XDErrorCode unlock();
private:
    pthread_mutex_t mutex_;
};

typedef XDMutexLinuxImp XDMutex;

#endif // end xd_muteximp_linux_h
