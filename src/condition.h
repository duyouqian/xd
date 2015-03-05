#ifndef XD_CONDITION_H
#define XD_CONDITION_H

#ifdef __unix__
    // unix or linux
    #include "condition_linux.h"
    typedef XDConditionLinuxImp XDCondition;
#elif __APPLE__
    // mac os
#else
    // win
#endif

#endif // end xd_condition_h
