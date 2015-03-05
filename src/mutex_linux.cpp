#include "mutex_linux.h"
#include "error.h"

XDMutexLinuxImp::XDMutexLinuxImp()
{
    int32 ret;
    pthread_mutexattr_t attr;
    ret = pthread_mutexattr_init(&attr);
    if (0 != ret) {
        // 设置互斥量属性失败
        ret = pthread_mutex_init(&mutex_, NULL);
    } else {
        // 开启互斥量错误检查
        ret = pthread_mutexattr_settype(&attr, PTHREAD_MUTEX_ERRORCHECK_NP);
        ret = pthread_mutex_init(&mutex_, &attr);
        pthread_mutexattr_destroy(&attr);
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
    // 暂时用lock
    return lock();
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
