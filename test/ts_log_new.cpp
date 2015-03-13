#include <stdio.h>
#include <fcntl.h>
#include <string.h>
#include "common.h"

XDThreadGroup threadGroup("TS_group");

int32 count = 0;

class Thread1Run : public XDIRunnable
{
public:
    uint32 run()
    {
        while (1) {
            XDLOG_minfo("%d", count);
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
        XDIThread *thread = XDThread::create(&tr1, "Thread1Run", &threadGroup);
    }
    while (1) {
        XDTimer::safeSleepByS(1);
    }
    XDLOG_close();
    return 0;
}
