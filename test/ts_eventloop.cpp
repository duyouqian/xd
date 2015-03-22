#include "common.h"

class Thread1Run : public XDIRunnable
{
public:
    explicit Thread1Run(XDIOEventLoop *loop) : loop_(loop)
    {
    }

    uint32 run()
    {
        XDLOG_minfo("[Thread1] 暂停2s后退出 loop");
        XDTimer::safeSleepByS(2);
        loop_->quit();
        return 0;
    }
private:
    XDIOEventLoop *loop_;
};

int main(int argc, char **argv)
{
    XDLOG_open("log", 0);
    XDIOEventLoop loop;
    Thread1Run tr1(&loop);
    XDIThread *thread = XDThread::create(&tr1, "Thread1Run");
    loop.loop();
    thread->waitForComplateion();
    //XDLOG_close();
    return 0;
}
