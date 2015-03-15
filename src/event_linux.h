#ifndef XD_EVENTIMP_LINUX_H
#define XD_EVENTIMP_LINUX_H

#include "base_event.h"
#include "mutex.h"
#include "condition.h"

class XDSyncEventLinuxImp : public XDIEvent
{
public:
    static XDSyncEventLinuxImp* createEvent(bool isManualReset = false);
    XDSyncEventLinuxImp();
    ~XDSyncEventLinuxImp();
    // 创建
    bool create(bool isManualReset = false);
    // 触发
    void trigger();
    // 重置
    void reset();
    // 等待
    void wait();
    // 等待 waitTime = uint32(-1) or 0 无限等待
    void wait(uint32 waitTime, bool ignoreThreadIdelState = false);
private:
    bool isManualReset_;
    bool isInit_;
    XDMutex mutex_;
    XDCondition cond_;
    TriggerType triggerType_;
};

#endif // end xd_eventimp_linux_h
