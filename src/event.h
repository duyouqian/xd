#ifndef XD_EVENT_H
#define XD_EVENT_H

#ifdef __unix__
    // unix or linux
    #include "event_linux.h"
    typedef XDSyncEventLinuxImp XDSyncEvent;
#elif __APPLE__
    // mac os
#else
    // win
#endif


#endif // end xd_event_h
