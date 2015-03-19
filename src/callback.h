#ifndef XD_CALLBACK_H
#define XD_CALLBACK_H

#include "shareable.h"
#include "shared_pointer.h"
#include "types.h"

// 无参数回调
class XDIOEventCallBack : public XDShareable
{
public:
    virtual bool exec() = 0;
};

// 新连接回调
class XDIOEventConnectionCallBack : public XDShareable
{
public:
    virtual bool exec(/*const XDTcpConnectionPtr &conn*/) = 0;
};

// 读回调
class XDIOEventReadCallBack : public XDShareable
{
public:
    virtual bool exec(uint64 timestamp) = 0;
};

typedef XDSharedPtr<XDIOEventCallBack> XDIOEventCallBackPtr;
typedef XDSharedPtr<XDIOEventConnectionCallBack> XDIOEventConnectionCallBackPtr;
typedef XDSharedPtr<XDIOEventReadCallBack> XDIOEventReadCallBackPtr;

#endif // end xd_callback_h
