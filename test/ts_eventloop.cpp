#include "common.h"

    void exec(XDIOEventLoop *loop)
    {
        XDLOG_minfo("[InitThread]");
    }

int main(int argc, char **argv)
{
    XDLOG_open("log", 0);
    XDIOEventLoop loop;
    XDIOEventLoopThreadPool pool(&loop, "IOEventLoopThreadPool");
    pool.setNumThread(10);
    pool.start(std::bind(exec, std::placeholders::_1));
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
