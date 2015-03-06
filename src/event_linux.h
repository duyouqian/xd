#ifndef XD_EVENTIMP_LINUX_H
#define XD_EVENTIMP_LINUX_H

#include "base_event.h"
#include "mutex.h"
#include "condition.h"

class XDSyncEventLinuxImp : public XDIEvent
{
public:
    bool create(bool isManualReset = false);
    void tigger();
    void reset();
    void wait();
    void wait(uint32 waitTime, bool ignoreThreadIdelState = false);
private:
    bool isManualReset_;
    bool isInit_;

};

#endif // end xd_eventimp_linux_h
