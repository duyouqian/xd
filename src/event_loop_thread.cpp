#include "event_loop_thread.h"
#include "event_loop.h"
#include "log.h"

XDIOEventLoopThread::XDIOEventLoopThread(const XDIOEventLoopThreadInitCallBack &cb,
                                         const char *name)
                                         : thread_(NULL)
                                         , name_(name)
                                         , loop_(NULL)
                                         , isExiting_(false)
{
    initThreadCallBack_ = cb;
}

XDIOEventLoopThread::~XDIOEventLoopThread()
{
    isExiting_ = true;
    if (loop_) {
        loop_->quit();
        delete thread_;
        thread_ = NULL;
    }
    check(NULL == loop_);
}

XDIOEventLoop* XDIOEventLoopThread::startLoop()
{
    check(NULL == thread_);
    thread_ = XDThread::create(this, name_.c_str());
    return loop_;
}

bool XDIOEventLoopThread::init()
{
    check(NULL == loop_);
    loop_ = new XDIOEventLoop();
    XDLOG_mdebug("[IOEventLoopThread] 初始化");
    return true;
}

uint32 XDIOEventLoopThread::run()
{
    check(loop_);
    XDLOG_mdebug("[IOEventLoopThread] 运行");
    if (initThreadCallBack_) {
        initThreadCallBack_(loop_);
    }
    loop_->loop();
    delete loop_;
    loop_ = NULL;
    return 1;
}

void XDIOEventLoopThread::stop()
{
    XDLOG_mdebug("[IOEventLoopThread] 停止");
    exit();
}

void XDIOEventLoopThread::exit()
{
    XDLOG_mdebug("[IOEventLoopThread] 退出");
    isExiting_ = true;
}
