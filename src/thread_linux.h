#ifndef XD_THREADIMP_LINUX_H
#define XD_THREADIMP_LINUX_H

#include <pthread.h>
#include <string>

#include "base_thread.h"
#include "event.h"

class XDThreadLinuxImp : public XDIThread
{
public:
    XDThreadLinuxImp();
    ~XDThreadLinuxImp();

    virtual bool setThreadPriority(XDEThreadPriority newPri);
    virtual bool suspend(bool pause = true);
    virtual bool kill(bool wait = false);
    virtual void waitForComplateion();
    virtual uint32 getThreadID();
    virtual std::string getThreadName();

protected:
    // 线程优先级转换成数字
    virtual int32 translateThreadPriority(XDEThreadPriority pri);
    // 设置线程优先级
    virtual void setThreadPriority(pthread_t thread, XDEThreadPriority newPri);
    // 创建线程
    virtual int createThreadWithName(pthread_t *threadPtr, pthread_attr_t *attr, ThreadEntryPoint proc, void *argv, const char* name = NULL);
    // 获取默认栈大小
    virtual uint32 getDefaultStackSize();
    // 调整栈大小
    virtual uint32 adjustStackSize(uint32 stackSize);
    // 创建线程
    virtual bool spinThread(pthread_t *threadPtr, ThreadEntryPoint proc, uint32 stackSize, void *argv);
    // 线程执行体
    static void* STDCALL _ThreadProc(void *argv);
    // 获取线程执行体
    virtual ThreadEntryPoint getThreadEntryPoint();
    // 
    virtual void preRun();
    //
    virtual uint32 run();
    // 
    virtual void postRun();
    // 创建线程内部用
    virtual bool createInternal(XDIRunnable *runner, const char*tName, uint32 stackSize = 0, XDEThreadPriority pri = XDTHREADPRI_NORMAL, uint64 threadAffinityMask = 0);

protected:
    // 线程对象
    pthread_t pthread_;
    // 执行对象
    XDIRunnable *runnable_;
    // 线程同步事件
    XDSyncEvent *syncEvent_;
    // 优先级
    XDEThreadPriority threadPriority_;
    // 
    uint64 threadAffinityMask_;
    // 线程ID
    uint32 threadID_;
    // 线程名
    std::string threadName_;
    // 是否在运行
    volatile bool threadIsRunning_;
};

#endif // end xd_threadimp_linux_h
