#include "event_loop.h"
#include "log.h"
#include "thread.h"

XDIOEventLoop::XDIOEventLoop()
             : threadID_(XDThread::getCurrentThreadID())
{
}

XDIOEventLoop::~XDIOEventLoop()
{
}

void XDIOEventLoop::loop()
{
    XDLOG_minfo("[IOEventLoop] loop run threadID:%u", threadID_);
}

void XDIOEventLoop::checkInLoopThread()
{
}
