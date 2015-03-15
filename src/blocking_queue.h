#ifndef XD_BLOCKING_QUEUE_H
#define XD_BLOCKING_QUEUE_H

#include "noncopyable.h"
#include "mutex.h"
#include "condition.h"
#include "error.h"
#include "types.h"

#include <string.h>

#define MAX_QUEUE 2048
#define MASK_QUEUE 2047 // MAX_QUEUE - 1

// 有锁队列 用于并发
// 不管理节点内存的生命周期 有调用着管理
template <typename T>
class XDBlockingQueue : public XDNoncopyable
{
public:
    XDBlockingQueue() : writeCount_(0)
                      , readCount_(0)
                      , mutex_()
                      , notEmpty_(&mutex_)
                      , notFull_(&mutex_)
                      , data_(NULL)
    {
        data_ = new T[MAX_QUEUE];
    }

    ~XDBlockingQueue()
    {
        XDGuardMutex lock(&mutex_);
        if (data_) {
            delete []data_;
        }
    }

    void push(const T& item)
    {
        XDGuardMutex lock(&mutex_);
        while (isFull()) {
            notFull_.wait();
        }
        data_[getIndex(writeCount_)] = item;
        ++writeCount_;
        notEmpty_.signal();
    }

    T get(uint32 millisecond = (uint32)-1)
    {
        XDGuardMutex lock(&mutex_);
        XDErrorCode err = XDError::E_XD_SUCCESS;
        while (isEmpty()) {
            err = notEmpty_.timedwait(millisecond);
            if (XDError::E_XD_SUCCESS != err) {
                break;
            }
        }
        if (XDError::E_XD_SUCCESS == err) {
            T item = data_[getIndex(readCount_)];
            ++readCount_;
            notFull_.signal();
            return item;
        } else {
            // error
        }
        return NULL;
    }

    std::string dump()
    {
        char info[255];
        snprintf(info, sizeof(info), "[MQ Dump]:[writeCount:%d, readCount:%d, writePos:%d, readCount:%d]", writeCount_, readCount_, getIndex(writeCount_), getIndex(readCount_));
        std::string infoStr(info);
        return infoStr;
    }

public:
    FORCEINLINE uint32 getIndex(uint32 pos)
    {
        return pos & MASK_QUEUE;
    }

    FORCEINLINE bool isEmpty()
    {
        return writeCount_ == readCount_;
    }

    FORCEINLINE bool isFull()
    {
        return writeCount_ - readCount_ >= MAX_QUEUE;
    }

    FORCEINLINE int32 size()
    {
        return writeCount_ - readCount_;
    }

private:
    uint32 writeCount_;
    uint32 readCount_;
    mutable XDMutex mutex_;
    XDCondition notEmpty_;
    XDCondition notFull_;
    T* data_;
};

#endif // end xd_blocking_queue_h
