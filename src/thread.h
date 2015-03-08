#ifndef XD_THREAD_H
#define XD_THREAD_H

#ifdef __unix__
    // unix or linux
    #include "thread_linux.h"
    typedef XDThreadLinuxImp XDThread;
#elif __APPLE__
    // mac os
#else
    // win
#endif


#endif // end xd_event_h
