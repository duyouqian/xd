#ifndef XD_SEMA_H
#define XD_SEMA_H

#ifdef __unix__
    // unix or linux
    #include "sema_linux.h"
    typedef XDSemaphoreLinuxImp XDSema;
#elif __APPLE__
    // mac os
#else
    // win
#endif

#endif // end xd_sema_h
