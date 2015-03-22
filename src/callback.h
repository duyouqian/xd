#ifndef XD_CALLBACK_H
#define XD_CALLBACK_H

#include "shareable.h"
#include "shared_pointer.h"
#include "inetaddr.h"
#include "types.h"

class XDIOEventLoop;

class XDFunction : public XDShareable
{
public:
    virtual bool exec() = 0;
};

// 无参数回调
class XDIOEventCallBack : public XDFunction
{
public:
    virtual bool exec() = 0;
};

// 新连接回调
class XDIOEventNewConnectionCallBack : public XDShareable
{
public:
    virtual bool exec(FD fd, const XDIpv4Addr &addr) = 0;
};

// 读回调
class XDIOEventReadCallBack : public XDShareable
{
public:
    virtual bool exec(uint64 timestamp) = 0;
};

// IOEventLoop初始化回调
class XDIOEventLoopThreadInitCallBack : public XDShareable
{
public:
    virtual bool exec(XDIOEventLoop *loop) = 0;
};

typedef XDSharedPtr<XDIOEventCallBack> XDIOEventCallBackPtr;
typedef XDSharedPtr<XDIOEventNewConnectionCallBack> XDIOEventNewConnectionCallBackPtr;
typedef XDSharedPtr<XDIOEventReadCallBack> XDIOEventReadCallBackPtr;
typedef XDSharedPtr<XDIOEventLoopThreadInitCallBack> XDIOEventLoopThreadInitCallBackPtr;

#endif // end xd_callback_h
