#ifndef XD_SEMAIMP_LINUX_H
#define XD_SEMAIMP_LINUX_H

#include "base_sema.h"
#include "types.h"

#include <semaphore.h>

class XDSemaphoreLinuxImp : public XDISemaphore
{
public:
    explicit XDSemaphoreLinuxImp(int32 ninit = 0);
    ~XDSemaphoreLinuxImp();
    XDErrorCode wait();
    XDErrorCode timedwait(uint32 millisecond);
    XDErrorCode signal();
private:
    sem_t sem_;
};

#endif // end xd_semaimp_linux_h
