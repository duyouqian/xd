#include "sema_linux.h"
#include "error.h"

XDSemaphoreLinuxImp::XDSemaphoreLinuxImp(int32 ninit /* = 0 */)
{
    int32 ret = sem_init(&sem_, 0, ninit);
    isValid_ = 0 == ret;
}

XDSemaphoreLinuxImp::~XDSemaphoreLinuxImp()
{
    sem_destroy(&sem_);
}

XDErrorCode XDSemaphoreLinuxImp::wait()
{
    int32 ret;
    while ((ret = sem_wait(&sem_)) == -1 && EINTR == XDGetLastError());
    if (0 != ret) {
        return XDError::E_XD_SYSERROR;
    }
    return XDError::E_XD_SUCCESS;
}

XDErrorCode XDSemaphoreLinuxImp::timedwait(uint32 millisecond)
{
    return wait();
}

XDErrorCode XDSemaphoreLinuxImp::signal()
{
    int32 ret;
    while ((ret = sem_post(&sem_)) == -1 && EINTR == XDGetLastError());
    if (0 != ret) {
        return XDError::E_XD_SYSERROR;
    }
    return XDError::E_XD_SUCCESS;
}

bool XDSemaphoreLinuxImp::isValid() const
{
    return isValid_;
}
