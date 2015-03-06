#ifndef XD_BASE_EVENT_H
#define XD_BASE_EVENT_h

#include "noncopyable.h"
#include "types.h"

class XDIEvent : public XDNoncopyable
{
public:
    virtual bool create(bool isManualReset = false) = 0;
    virtual void tigger() = 0;
    virtual void reset() = 0;
    virtual void wait() = 0;
    virtual void wait(uint32 waitTime, bool ignoreThreadIdelState = false) = 0;
protected:
    typedef enum {
        TIGGERED_NONE,
        TIGGERED_ONE,
        TIGGERED_ALL
    } TiggerType;
};

#endif // end xd_base_event_h
