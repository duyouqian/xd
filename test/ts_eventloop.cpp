#include "common.h"

class IOEventLoopInitThread : public XDIOEventLoopThreadInitCallBack
{
public:
    bool exec(XDIOEventLoop *loop)
    {
        XDLOG_minfo("[InitThread]");
        return true;
    }
};

int main(int argc, char **argv)
{
    XDLOG_open("log", 0);
    XDIOEventLoopThreadInitCallBackPtr cb(new IOEventLoopInitThread());
    XDIOEventLoop loop;
    XDIOEventLoopThreadPool pool(&loop, "IOEventLoopThreadPool");
    pool.setNumThread(10);
    pool.start(cb);
    loop.loop();
    //XDIOEventLoopThread thread(cb, "IOEventLoopThread");
    //XDIOEventLoop* loop = thread.startLoop();
    //XDTimer::safeSleepByS(5);
    //loop->quit();
    //XDIOEventLoop loop;
    //XDIThread *thread = XDThread::create(&tr1, "Thread1Run");
    //loop.loop();
    //thread->waitForComplateion();
    XDLOG_close();
    return 0;
}
