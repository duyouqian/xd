#include "common.h"

XDIOEventLoop loop;

class ReadCallBack : public XDIOEventReadCallBack
{
public:
    bool exec()
    {
        XDLOG_minfo("exec");
    }
};

class Thread1Run : public XDIRunnable
{
public:
    uint32 run()
    {
        loop.loop();
    }
};

int main(int argvc, char **argv)
{
    XDLOG_open("log", 0);
    loop.loop();
    XDSharedPtr<ReadCallBack> read(new ReadCallBack());
    Thread1Run tr1;
    XDIThread *thread = XDThread::create(&tr1, "Thread1Run");
    thread->waitForComplateion();
    XDLOG_close();
    return 0;
}
