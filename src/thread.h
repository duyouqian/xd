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


<<<<<<< HEAD
#endif // end xd_thread_h
=======
#endif // end xd_event_h
>>>>>>> 4000395e364e5e1e1f57df94210ab5b07fc53527
