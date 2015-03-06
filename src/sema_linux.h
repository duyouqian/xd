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
    bool isValid() const;
private:
    sem_t sem_;
    bool isValid_;
};

#endif // end xd_semaimp_linux_h
