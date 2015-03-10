#include "event_linux.h"
#include "timer.h"

XDSyncEventLinuxImp::XDSyncEventLinuxImp()
                   : isManualReset_(false)
                   , isInit_(false)
                   , cond_(&mutex_)
                   , triggerType_(TRIGGERED_NONE)
{
}

XDSyncEventLinuxImp::~XDSyncEventLinuxImp()
{
}

bool XDSyncEventLinuxImp::create(bool isManualReset /* = false */)
{
    if (mutex_.isValid() && cond_.isValid()) {
        isInit_ = true;
        isManualReset_ = isManualReset;
    }
    return true;
}

void XDSyncEventLinuxImp::trigger()
{
    check(isInit_);
    XDGuardMutex lock(&mutex_);
    if (isManualReset_) {
        triggerType_ = TRIGGERED_ALL;
        cond_.broadcast();
    } else {
        triggerType_ = TRIGGERED_ONE;
        cond_.signal();
    }
}

void XDSyncEventLinuxImp::reset()
{
    check(isInit_);
    XDGuardMutex lock(&mutex_);
    triggerType_ = TRIGGERED_NONE;
}

void XDSyncEventLinuxImp::wait()
{
    wait((uint32)-1);
}

void XDSyncEventLinuxImp::wait(uint32 waitTime, bool ignoreThreadIdelState /* = false */)
{
    check(isInit_);
    XDGuardMutex lock(&mutex_);
    bool ret = false;
    struct timeval tv, now, tmp;
    XDTimer::getTimeOfDay(&tv, NULL);
    do {
        if (TRIGGERED_ONE == triggerType_) {
            triggerType_ = TRIGGERED_NONE;
            ret = true;
        } else if (TRIGGERED_ALL == triggerType_) {
            ret = true;
        } else if (waitTime != 0) {
            if ((uint32)-1 == waitTime) {
                cond_.wait();
            } else {
                XDErrorCode ec = cond_.timedwait(waitTime);
                check(XDError::E_XD_SUCCESS == ec || XDERROR::E_XD_TIMEOUT == ec);
                XDTimer::getTimeOfDay(&now, NULL);
                XDTimer::subtratTimeval(&now, &tv, &tmp);
                uint32 costMS = tmp.tv_sec * 1000 + tmp.tv_usec / 1000;
                if (costMS >= waitTime) {
                    waitTime = 0;
                } else {
                    waitTime -= costMS;
                }
                tv = now;
            }
        }
    } while (!ret && (0 != waitTime));
    if (0 == waitTime) {
        // timeout
    }
}
