#ifndef XD_SOCKET_UTIL_H
#define XD_SOCKET_UTIL_H

#ifdef __unix__
    // unix or linux
    #include "socketopt_linux.h"
#elif __APPLE__
    // mac os
#else
    // win
#endif

#endif // end xd_socket_util_h
