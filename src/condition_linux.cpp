#include "condition_linux.h"
#include "error.h"

XDConditionLinuxImp::XDConditionLinuxImp(XDMutex *mutex)
                   : mutex_(mutex)
{
    pthread_cond_init(&cond_, NULL);
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
    return wait();
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
