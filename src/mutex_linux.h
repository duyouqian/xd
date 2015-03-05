#ifndef XD_MUTEXIMP_LINUX_H
#define XD_MUTEXIMP_LINUX_H

#include "base_mutex.h"
#include "types.h"

#include <pthread.h>

class XDMutexLinuxImp : public XDIMutex
{
public:
    XDMutexLinuxImp();
    ~XDMutexLinuxImp();
    XDErrorCode lock();
    XDErrorCode timedlock(uint32 millisecond);
    XDErrorCode unlock();
    pthread_mutex_t* getMutex();
private:
    pthread_mutex_t mutex_;
};

#endif // end xd_muteximp_linux_h
