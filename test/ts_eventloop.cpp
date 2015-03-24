#include "common.h"

class Thread1Run : public XDIRunnable
{
public:
    explicit Thread1Run() : loop_(NULL)
                          , thread_(NULL)
    {
    }

    XDIOEventLoop* startInLoop()
    {
        thread_ = XDThread::create(this, "Thread1Run");
        return loop_;
    }

    bool init()
    {
        loop_ = new XDIOEventLoop();
        return true;
    }

    uint32 run()
    {
        loop_->loop();
        XDLOG_minfo("[Thread1] 暂停2s后退出 loop");
        XDTimer::safeSleepByS(2);
        //loop_->quit();
        return 0;
    }

    void exit()
    {
        delete loop_;
        loop_ = NULL;
    }

private:
    XDIOEventLoop *loop_;
    XDIThread *thread_;
};

int main(int argc, char **argv)
{
    XDLOG_open("log", 0);
    //XDIOEventLoop loop;
    Thread1Run tr1;
    tr1.startInLoop();
    //XDIThread *thread = XDThread::create(&tr1, "Thread1Run");
    //loop.loop();
    //thread->waitForComplateion();
    XDLOG_close();
    return 0;
}
