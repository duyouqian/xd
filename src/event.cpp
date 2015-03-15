#include "event.h"

XDSyncEvent* XDSyncEvent::createEvent(bool isManualReset)
{
    XDSyncEvent *event = new XDSyncEvent();
    if (!event->create(isManualReset)) {
        // 创建同步事件失败
        delete event;
        event = NULL;
    }
    return event;
}
