#include "thread_linux.h"
#include "timer.h"

#include <string.h>

XDThreadLinuxImp::XDThreadLinuxImp()
                : pthread_(PTHREAD_NULL)
                , runnable_(NULL)
                , syncEvent_(NULL)
                , threadPriority_(XDTHREADPRI_NORMAL)
                , threadAffinityMask_(0)
                , threadID_(0)
                , threadIsRunning_(false)
{
}

XDThreadLinuxImp::~XDThreadLinuxImp()
{
    if (PTHREAD_NULL != pthread_) {
        kill(true);
    }
    // remove to poll
    threadID_ = 0;
}

bool XDThreadLinuxImp::setThreadPriority(XDEThreadPriority newPri)
{
    if (newPri != threadPriority_) {
        threadPriority_ = newPri;
        setThreadPriority(pthread_, newPri);
    }
    return true;
}

bool XDThreadLinuxImp::suspend(bool pause)
{
    return true;
}

bool XDThreadLinuxImp::kill(bool wait)
{
    if (runnable_) {
        runnable_->stop();
    }
    if (wait) {
        while (threadIsRunning_) {
            XDTimer::safeSleepByMS(1);
        }
    }
    pthread_ = PTHREAD_NULL;
    return true;
}

void XDThreadLinuxImp::waitForComplateion()
{
    while(threadIsRunning_) {
        XDTimer::safeSleepByMS(1);
    }
}

uint32 XDThreadLinuxImp::getThreadID()
{
    return threadID_;
}

std::string XDThreadLinuxImp::getThreadName()
{
    return threadName_;
}

int32 XDThreadLinuxImp::translateThreadPriority(XDEThreadPriority pri)
{
    switch (pri) {
        case XDTHREADPRI_NORMAL:
            return 15;
        case XDTHREADPRI_ABOVENORMAL:
            return 25;
        case XDTHREADPRI_BELOWNORMAL:
            return 5;
        case XDTHREADPRI_HIGEST:
            return 30;
        default:
            return 15;
    }
}

void XDThreadLinuxImp::setThreadPriority(pthread_t thread, XDEThreadPriority newPri)
{
    struct sched_param sched;
    memset(&sched, 0, sizeof(sched));
    int32 policy = SCHED_RR;
    // 获取当前policy
    pthread_getschedparam(thread, &policy, &sched);
    // 设置policy
    sched.sched_priority = translateThreadPriority(newPri);
    pthread_setschedparam(thread, policy, &sched);
}

int XDThreadLinuxImp::createThreadWithName(pthread_t *threadPtr, pthread_attr_t *attr, ThreadEntryPoint proc, void *argv, const char* name)
{
    return pthread_create(threadPtr, attr, proc, argv);
}

uint32 XDThreadLinuxImp::getDefaultStackSize()
{
    return 128 * 1024; // 128k
}

uint32 XDThreadLinuxImp::adjustStackSize(uint32 stackSize)
{
    if (0 == stackSize) {
        stackSize = getDefaultStackSize();
    }
    return stackSize;
}

bool XDThreadLinuxImp::spinThread(pthread_t *threadPtr, ThreadEntryPoint proc, uint32 stackSize, void *argv)
{
    bool isCreate = false;
    pthread_attr_t *attr = NULL;
    pthread_attr_t stackAttr;
    stackSize = adjustStackSize(stackSize);
    if (0 != stackSize) {
        if (0 == pthread_attr_init(&stackAttr)) {
            const size_t size = (size_t)stackSize;
            if (0 == pthread_attr_setstacksize(&stackAttr, size)) {
                attr = &stackAttr;
            }
            if (NULL == attr) {
                // 设置栈大小失败
            }
        }
    }
    int ret = createThreadWithName(threadPtr, attr, proc, argv, threadName_.c_str());
    isCreate = 0 == ret;
    if (NULL != attr) {
        pthread_attr_destroy(attr);
    }
    if (!isCreate) {
        // 创建线程失败
    }
    return isCreate;
}

void* XDThreadLinuxImp::_ThreadProc(void *argv)
{
    check(argv);
    XDThreadLinuxImp *thread = (XDThreadLinuxImp*)argv;
    
    thread->preRun();
    thread->run();
    thread->postRun();
    pthread_exit(NULL);
    return NULL;

}

ThreadEntryPoint XDThreadLinuxImp::getThreadEntryPoint()
{
    return _ThreadProc;
}

void XDThreadLinuxImp::preRun()
{
    // 设置线程名
    int32 ret = pthread_setname_np(pthread_, threadName_.c_str());
    if (0 != ret) {
        // error
    }
}

uint32 XDThreadLinuxImp::run()
{
    threadIsRunning_ = true;
    check(runnable_);
    uint32 exitCode = 1;
    if (runnable_->init()) {
        syncEvent_->trigger();
        exitCode = runnable_->run();
        runnable_->exit();
    } else {
        // init failure
        syncEvent_->trigger();
    }
    threadIsRunning_ = false;
    return exitCode;
}

void XDThreadLinuxImp::postRun()
{
}

bool XDThreadLinuxImp::createInternal(XDIRunnable *runner, const char *tName, uint32 stackSize, XDEThreadPriority pri, uint64 threadAffinityMask)
{
    runnable_ = runner;
    bool eventCreate = false;
    syncEvent_ = new XDSyncEvent();
    eventCreate = syncEvent_->create(true);
    const char *tt = "111";
    std::string tmpName(tName ? tName : "XD Server V1.0.0");
    threadName_.assign(tmpName);
    threadAffinityMask_ = threadAffinityMask;
    bool threadCreated = spinThread(&pthread_, getThreadEntryPoint(), stackSize, this);
    if (threadCreated) {
        pthread_detach(pthread_);
        syncEvent_->wait();
        setThreadPriority(pri);
    } else {
        runnable_ = NULL;
    }
    delete syncEvent_;
    syncEvent_ = NULL;
    return PTHREAD_NULL != pthread_;
}
