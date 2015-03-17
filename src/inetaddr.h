#ifndef XD_INET_ADDRESS_H
#define XD_INET_ADDRESS_H

#ifdef __unix__
    // unix or linux
    #include "inetaddr_linux.h"
    typedef XDIpv4AddrLinuxImp XDIpv4Addr;
#elif __APPLE__
    // mac os
#else
    // win
#endif

#endif // end xd_inet_address_h
