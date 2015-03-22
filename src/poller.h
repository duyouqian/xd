#ifndef XD_POLLER_H
#define XD_POLLER_H

#ifdef __unix__
    // unix or linux
    #include "epoll.h"
    typedef XDEPoller XDPoller;
#elif __APPLE__
    // mac os
#else
    // win
#endif

#endif // end xd_poller_h
