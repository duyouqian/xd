#include "sema_linux.h"
#include "error.h"
#include "timer.h"

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
    if (0 == millisecond || (uint32)(-1) == millisecond) {
        // 无限wait
        return wait();
    }
    struct timespec ts;
    XDTimer::getAbsTimespec(&ts, millisecond);
    int32 ret;
    while ((ret = sem_timedwait(&sem_, &ts)) == -1 && EINTR == XDGetLastError());
    if (0 != ret) {
        int32 err = XDGetLastError();
        if (ETIMEDOUT == err) {
            return XDError::E_XD_TIMEOUT;
        }
        return XDError::E_XD_SYSERROR;
    }
    return XDError::E_XD_SUCCESS;
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
