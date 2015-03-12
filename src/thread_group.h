#ifndef XD_THREAD_GROUP_H
#define XD_THREAD_GROUP_H

#include "noncopyable.h"
#include "base_thread.h"
#include "types.h"

// 线程组
class XDThreadGroup : public XDNoncopyable
{
public:
    explicit XDThreadGroup(const char *name);
    virtual ~XDThreadGroup();
    void addThread(uint32 id, XDIThread* thread);
    void removeThread(uint32 id);
    XDIThread* getThreadByID(uint32 id);
    uint32 getCount();
private:
    std::string name_;
    XDThreadRegistry allThreads_;
};

#endif // end xd_thread_group_h
