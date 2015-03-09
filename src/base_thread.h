#ifndef XD_BASE_THREAD_H
#define XD_BASE_THREAD_H

#include "noncopyable.h"
#include "types.h"

// 线程优先级
enum XDEThreadPriority
{
};

// 线程执行体
class XDIRunable : public XDNoncopyable
{
};

// 线程接口
class XDIThread : public XDNoncopyable
{
};

#endif // end xd_base_thread_h
