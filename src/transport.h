#ifndef XD_TRANSPORT_H
#define XD_TRANSPORT_H

#include "shareable.h"
#include "types.h"

class XDITransport : public XDShareable
{
public:
    // 打开连接
    // return success:0 errno:other
    virtual int32 open() = 0;
    // 关闭连接
    // return success:0 errno:other
    virtual int32 close() = 0;
    // 读取
    virtual int32 read(void *buffer, int32 len) = 0;
    // 写
    virtual int32 write(const void *message, int32 len) = 0;
    // 是否打开
    virtual bool isOpen() = 0;
    // 判断缓冲区是否有可都数据
    virtual bool peek() = 0;
    // 获取ID
    virtual TRANSPORTID getID() = 0;
};

#endif // end xd_transport_h
