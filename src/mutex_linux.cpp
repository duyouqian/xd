#include "mutex_linux.h"
#include "error.h"
#include "timer.h"

XDMutexLinuxImp::XDMutexLinuxImp()
{
    int32 ret;
    pthread_mutexattr_t attr;
    ret = pthread_mutexattr_init(&attr);
    if (0 != ret) {
        // 设置互斥量属性失败
        ret = pthread_mutex_init(&mutex_, NULL);
        isValid_ = false;
    } else {
        // 开启互斥量错误检查
        ret = pthread_mutexattr_settype(&attr, PTHREAD_MUTEX_ERRORCHECK_NP);
        ret = pthread_mutex_init(&mutex_, &attr);
        pthread_mutexattr_destroy(&attr);
        isValid_ = true;
    }
}

XDMutexLinuxImp::~XDMutexLinuxImp()
{
    pthread_mutex_destroy(&mutex_);
}

XDErrorCode XDMutexLinuxImp::lock()
{
    int32 ret = pthread_mutex_lock(&mutex_);
    if (0 != ret) {
        // 加锁失败
        return XDError::E_XD_SYSERROR;
    }
    return XDError::E_XD_SUCCESS;
}

XDErrorCode XDMutexLinuxImp::timedlock(uint32 millisecond)
{
    if (0 == millisecond || (uint32)(-1) == millisecond) {
        // 无限lock
        return lock();
    }
    struct timespec ts;
    XDTimer::getAbsTimespec(&ts, millisecond);
    int32 ret = pthread_mutex_timedlock(&mutex_, &ts);
    if (0 != ret) {
        if (ETIMEDOUT == ret) {
            return XDError::E_XD_TIMEOUT;
        }
        return XDError::E_XD_SYSERROR;
    }
    return XDError::E_XD_SUCCESS;
}

XDErrorCode XDMutexLinuxImp::unlock()
{
    int32 ret = pthread_mutex_unlock(&mutex_);
    if (0 != ret) {
        // 加锁失败
        return XDError::E_XD_SYSERROR;
    }
    return XDError::E_XD_SUCCESS;
}

pthread_mutex_t* XDMutexLinuxImp::getMutex()
{
    return (&mutex_);
}

bool XDMutexLinuxImp::isValid() const
{
    return isValid_;
}
