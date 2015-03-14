#include <stdio.h>
#include <fcntl.h>
#include <string.h>
#include "common.h"

XDThreadGroup threadGroup("TS_group");

int32 count = 0;

volatile bool isStop = false;

class Thread1Run : public XDIRunnable
{
public:
    uint32 run()
    {
        while (!isStop) {
            XDLOG_minfo("%u, %d", XDThread::getCurrentThreadID(), count);
            ++count;
            XDTimer::safeSleepByS(1);
        }
    }
};

int main(int argc, char **argv)
{
    XDLOG_open("log", 0);
    int i, len = 10;
    Thread1Run tr1;
    for (i = 0; i < len; ++i) {
        //XDLOG_minfo("%ld, %d", XDThread::getCurrentThreadID(), count);
        XDIThread *thread = XDThread::create(&tr1, "Thread1Run", &threadGroup);
    }
    XDTimer::safeSleepByS(2);
    isStop = true;
    XDTimer::safeSleepByS(2);
    XDLOG_close();
    return 0;
}
