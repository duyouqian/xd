#ifndef XD_CONDITIONIMP_LINUX_H
#define XD_CONDITIONIMP_LINUX_H

#include "base_condition.h"
#include "mutex.h"
#include "types.h"

#include <pthread.h>

class XDConditionLinuxImp : public XDICondition
{
public:
    explicit XDConditionLinuxImp(XDMutex *mutex);
    ~XDConditionLinuxImp();

    XDErrorCode wait();
    XDErrorCode timedwait(uint32 millisecond);
    XDErrorCode signal();
    XDErrorCode broadcast();
private:
    XDMutex *mutex_;
    pthread_cond_t cond_;
};

#endif // end xd_condition_h
