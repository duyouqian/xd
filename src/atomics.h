#ifndef XD_ATOMICS_H
#define XD_ATOMICS_H

#ifdef __unix__
    // unix or linux
    #include "atomics_linux.h"
    typedef XDAtomicsLinuxImp XDAtomics;
#elif __APPLE__
    // mac os
#else
    // win
#endif


#endif // end xd_atomics_h
