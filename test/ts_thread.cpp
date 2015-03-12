#include <stdio.h>
#include "common.h"

class Thread1Run : public XDIRunnable
{
public:
    uint32 run()
    {
        printf("[Thread1Run] run \n");
    }
};

int main(int argc, char **argv)
{
    Thread1Run tr1;
    XDIThread *thread = XDThread::create(&tr1, "Thread1Run");
    thread->waitForComplateion();
    delete thread;
    return 0;
}
