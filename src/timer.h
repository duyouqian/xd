#ifndef XD_TIMER_H
#define XD_TIMER_H

#ifdef __unix__
    // unix or linux
    #include "timer_linux.h"
    typedef XDTimerLinuxImp XDTimer;
#elif __APPLE__
    // mac os
#else
    // win
#endif


#endif // end xd_event_h
