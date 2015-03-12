#ifndef XD_FILE_H
#define XD_FILE_H

#ifdef __unix__
    // unix or linux
    #include "file_linux.h"
    typedef XDFileHandleLinuxImp XDFileHandle;
#elif __APPLE__
    // mac os
#else
    // win
#endif


#endif // end xd_event_h
