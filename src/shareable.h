#ifndef XD_SHAREABLE_H
#define XD_SHAREABLE_H

#include "noncopyable.h"
#include "reference_count.h"
#include "types.h"

// 智能指针基础设施
// 提供引用技术

class XDIShareable : public XDNoncopyable
{
public:
    virtual void inc() = 0;
    virtual int32 dec() = 0;
    virtual int32 getCount() = 0;
};

class XDShareable : public XDIShareable
{
public:
    virtual ~XDShareable();
    void inc();
    int32 dec();
    int32 getCount();
protected:
    XDRefCounter ref_;
};

#endif // end xd_shareable_h
