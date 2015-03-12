#include <stdio.h>
#include "common.h"

XDThreadGroup threadGroup("TS_group");

class Thread1Run : public XDIRunnable
{
public:
    uint32 run()
    {
        printf("[Thread1Run] run threadGroup:%d\n", threadGroup.getCount());
    }
};

int main(int argc, char **argv)
{
    int i, len = 10;
    for (i = 0; i < len; ++i) {
        Thread1Run tr1;
        XDIThread *thread = XDThread::create(&tr1, "Thread1Run", &threadGroup);
        thread->waitForComplateion();
        delete thread;
    }
//    XDTimer::safeSleepByS(100);
    return 0;
}
