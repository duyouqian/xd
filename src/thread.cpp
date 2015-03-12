#include "thread.h"

XDIThread* XDThread::create(XDIRunnable *runner, const char *tName, bool isAutoDeleteSelf, bool isAutoDeleteRunnable /* = false */, XDThreadGroup *threadGroup /* = NULL */, uint32 stackSize /* = 0 */, XDEThreadPriority pri /* = XDTHREADPRI_NORMAL */, uint64 threadAffinityMask /* = 0 */)
{
    return create(runner, tName, threadGroup, stackSize, pri, threadAffinityMask);
}

XDIThread* XDThread::create(XDIRunnable* runner, const char *tName, XDThreadGroup *threadGroup, uint32 stackSize /* = 0 */, XDEThreadPriority pri /* = XDTHREADPRI_NORAML */, uint64 threadAffinityMask /* = 0xFFFFFFFFFFFFFFFF */)
{
    XDThread *thread = new XDThread();
    if (!thread->createInternal(runner, tName, threadGroup, stackSize, pri, threadAffinityMask)) {
        delete thread;
        thread = NULL;
    }
    if (thread) {
        // 添加到管理器中
    }
    return thread;
}
