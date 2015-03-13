#ifndef XD_BASE_FILE_H
#define XD_BASE_FILE_H

#include "noncopyable.h"
#include "types.h"

class XDIFileHandle : public XDNoncopyable
{
public:
    // 获取当前读写位置
    virtual int64 tell() = 0;
    // 设置读写位置
    virtual bool seek(int64 newPosition) = 0;
    // 设置读写位置至文件末尾
    virtual bool seekFromEnd() = 0;
    // 读取文件
    virtual bool read(uint8 *buffer, uint32 len) = 0;
    // 写文件
    virtual bool write(const uint8 *msg, uint32 len) = 0;
    // 文件大小
    virtual uint64 size() = 0;
};

class XDIFile : public XDNoncopyable
{
public:
    // 只读打开文件
    virtual bool readOpen(const char *name) = 0;
    // 只写/写打开文件
    virtual bool writeOpen(const char *name, bool isAppend, bool isAllowRead) = 0;
    // 关闭文件
    virtual bool close() = 0;
    // 读取文件
    virtual bool read(uint8 *buffer, uint32 len) = 0;
    // 写文件
    virtual bool write(const uint8 *msg, uint32 len) = 0;
};

#endif // end xd_base_file_h
