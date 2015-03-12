#ifndef XD_BASE_THREAD_H
#define XD_BASE_THREAD_H

#include "mutex.h"
#include "noncopyable.h"
#include "types.h"
#include <string>
#include <map>

class XDThreadGroup;

// 线程优先级
enum XDEThreadPriority
{
    XDTHREADPRI_NORMAL,
    XDTHREADPRI_ABOVENORMAL,
    XDTHREADPRI_BELOWNORMAL,
    XDTHREADPRI_HIGEST
};

// 线程执行体
class XDIRunnable : public XDNoncopyable
{
public:
    // 初始化
    virtual bool init() { return true; }
    // 真实执行体
    virtual uint32 run() = 0;
    // 停止
    virtual void stop() { }
    // 退出
    virtual void exit() { }
};

// 线程接口
class XDIThread : public XDNoncopyable
{
public:
    // 获取线程ID
    static uint32 getCurrentThreadID();
    // 创建线程
    //static XDIThread* create(XDIRunnable *runner, const char *tName, XDThreadGroup *threadGroup = NULL, bool isAutoDeleteSelf, bool isAutoDeleteRunnable = false, uint32 stackSize = 0, XDEThreadPriority pri = XDTHREADPRI_NORMAL, uint64 threadAffinityMask = 0);
    //static XDIThread* create(XDIRunnable *runner, const char *tName, XDThreadGroup *threadGroup_, uint32 stackSize = 0, XDEThreadPriority pri = XDTHREADPRI_NORMAL, uint64 threadAffinityMask = 0xFFFFFFFFFFFFFFFF);
    // 设置优先级
    virtual bool setThreadPriority(XDEThreadPriority newPri) = 0;
    // 设置线程暂停和恢复 true:暂停 false:恢复
    virtual bool suspend(bool pause = true) = 0;
    // 杀死 wait 是否等待结束
    virtual bool kill(bool wait = false) = 0;
    // 等待完成
    virtual void waitForComplateion() = 0;
    // 获取线程ID
    virtual uint32 getThreadID() = 0;
    // 获取线程名
    virtual std::string getThreadName() = 0;
protected:
    virtual bool createInternal(XDIRunnable *runner, const char *tName, XDThreadGroup *threadGroup = NULL, uint32 stackSize = 0, XDEThreadPriority pri = XDTHREADPRI_NORMAL, uint64 threadAffinityMask = 0) = 0;
};


class XDThreadRegistry : public XDNoncopyable
{
public:
    // 添加
    void add(uint32 id, XDIThread *thread)
    {
        XDGuardMutex lock(&mutex_);
        std::map<uint32, XDIThread*>::iterator it;
        it = registry_.find(id);
        if (it != registry_.end()) {
            // replace
            it->second = thread;
        } else {
            // insert
            registry_.insert(std::make_pair(id, thread));
        }
        isUpdated_ = true;
    }
    // 移除
    void remove(uint32 id)
    {
        registry_.erase(id);
    }
    // 获取数量
    uint32 getCount()
    {
        uint32 num = 0;
        {
            XDGuardMutex lock(&mutex_);
            num = registry_.size();
        }
        return num;
    }
    // 获取更新状态
    bool isUpdated()
    {
        return isUpdated_;
    }
    // 清理更新状态
    void clearUpdated()
    {
        isUpdated_ = false;
    }
    // 获取线程
    XDIThread* getThread(uint32 id)
    {
        std::map<uint32, XDIThread*>::iterator it;
        it = registry_.find(id);
        if (it != registry_.end()) {
            return it->second;
        }
        return NULL;
    }

private:
    std::map<uint32, XDIThread*> registry_;
    bool isUpdated_;
    XDMutex mutex_;
};

#endif // end xd_base_thread_h
