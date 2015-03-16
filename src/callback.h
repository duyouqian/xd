#ifndef XD_CALLBACK_H
#define XD_CALLBACK_H

#include "shareable.h"
#include "shared_pointer.h"

class XDBaseCallBack : public XDShareable
{
public:
    virtual bool exec() = 0; // IO事件回调执行接口
};

class XDIOEventCallBack : public XDBaseCallBack
{
};

typedef XDSharedPtr<XDIOEventCallBack> XDIOEventCallBackPtr;

#endif // end xd_callback_h
