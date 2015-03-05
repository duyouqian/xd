#include "sema_linux.h"
#include "error.h"

XDSemaphoreLinuxImp::XDSemaphoreLinuxImp(int32 ninit /* = 0 */)
{
    sem_init(&sem_, 0, ninit);
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
