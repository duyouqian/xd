#include "condition_linux.h"
#include "error.h"
#include "timer.h"

XDConditionLinuxImp::XDConditionLinuxImp(XDMutex *mutex)
                   : mutex_(mutex)
{
    int32 ret = pthread_cond_init(&cond_, NULL);
    isValid_ = 0 == ret;
}

XDConditionLinuxImp::~XDConditionLinuxImp()
{
    pthread_cond_destroy(&cond_);
}

XDErrorCode XDConditionLinuxImp::wait()
{
    int32 ret =- pthread_cond_wait(&cond_, mutex_->getMutex());
    if (0 != ret) {
        return XDError::E_XD_SYSERROR;
    }
    return XDError::E_XD_SUCCESS;
}

XDErrorCode XDConditionLinuxImp::timedwait(uint32 millisecond)
{
    if (0 == millisecond || (uint32)(-1) == millisecond) {
        // 无限wait
        return wait();
    }
    struct timespec ts;
    XDTimer::getAbsTimespec(&ts, millisecond);
    int32 ret = pthread_cond_timedwait(&cond_, mutex_->getMutex(), &ts);
    if (0 != ret) {
        if (ETIMEDOUT == ret) {
            return XDError::E_XD_TIMEOUT;
        }
        return XDError::E_XD_SYSERROR;
    }
    return XDError::E_XD_SUCCESS;
}

XDErrorCode XDConditionLinuxImp::signal()
{
    int32 ret = pthread_cond_signal(&cond_);
    if (0 != ret) {
        return XDError::E_XD_SYSERROR;
    }
    return XDError::E_XD_SUCCESS;
}

XDErrorCode XDConditionLinuxImp::broadcast()
{
    int32 ret = pthread_cond_broadcast(&cond_);
    if (0 != ret) {
        return XDError::E_XD_SYSERROR;
    }
    return XDError::E_XD_SUCCESS;
}

bool XDConditionLinuxImp::isValid() const
{
    return isValid_;
}
