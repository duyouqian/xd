#ifndef XD_SOCKET_H
#define XD_SOCKET_H

#ifdef __unix__
    // unix or linux
    #include "socket_linux.h"
    typedef XDSocketLinuxImp XDSocket;
#elif __APPLE__
    // mac os
#else
    // win
#endif

#endif // end xd_socket_h
